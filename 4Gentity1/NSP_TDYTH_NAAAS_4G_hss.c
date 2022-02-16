/*
 * 文件功能：4G接入认证HSS服务端主函数
 * 作者：丁潇 寇飞翔
 * 创建日期：2020/02/10
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_anet.h"
#include "NSP_TDYTH_NAAAS_4G_gen.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"
#include "NSP_TDYTH_NAAAS_4G_utils.h"
#include "NSP_TDYTH_NAAAS_4G_db.h"
#include "NSP_TDYTH_NAAAS_4G_debug.h"

unsigned char gs_NAAAS_Body[500];
/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST
* 结构体用途：                HSS端所属单链表，用来存储用户ip和端口
* @param  iCount         用户注册的个数
* @param  iport          用户的端口号
* @param  aucIp          用户IP
************************************************************/
typedef struct St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST
{
    int iCount;
    int iport;
    unsigned char aucIp[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    struct St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST *psNext;
} St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST;

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuHSS
* 结构体用途：				4G注册HSS端存储消息结构体
* @param  uiID_HSS         HSS的ID
* @param  aucSNID           SNID
* @param  aucIMSI	 		IMSI
************************************************************/
typedef struct {
    unsigned int uiID_HSS;
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucSNID[D_NSP_TDYTH_NAAAS_4G_SNID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuHSS;

typedef struct {
    unsigned char CardID[12];
    unsigned char IMSI[16];
    unsigned char HSSID[15];
    unsigned char PIN[5];
    unsigned char PUK[9];
    unsigned char userName[40];
    unsigned char Sex[5];
    unsigned char IdentityCard[18];
    unsigned char Category[10];
    unsigned char Country[20];
    unsigned char Province[20];
    unsigned char City[20];
    unsigned char DetailAddress[30];
    unsigned char ServicePSW[7];
    time_t Timestamp;
}__attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_ToNAAAS;


static St_NSP_TDYTH_NAAAS_4G_AuHSS s_stAuHSS;



# define D_NSP_TDYTH_NAAAS_4G_stAuHSS_LEN 14
/*
 * 备注：1、在认证过程中，HSS端需要保存一些信息，按照文档中数据库对应表，暂时保存这些信息，HSS端保存信息结构体自行定义和保存。
 *      2、HSS端在4G接入认证协议处理过程中，只接收来自MME的认证数据请求，然后处理，之后向MME发送认证数据响应
 */

/* 
*功能：HSS给MME发送认证向量
*参数：无
*返回值： 0成功
*        <0失败
*/
int sender_Client() {
    int iSockfd;
    // Ready to send HSS AVs
    unsigned char aucBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN] = "Here is HSS AVs data.";
    char *pcIP = D_NSP_TDYTH_NAAAS_MME1_IP;
    unsigned short usPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    // Creating socket file descriptor
    iSockfd = NSP_TDYTH_4G_ANET_ipv4_send_init();
    // Filling server information
    // Send a datagram
    NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, aucBuf, D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN, pcIP, usPort, 1);
    printf("HSS#Client: AVs sent.\n");
    return 0;
}

/* 
*功能：HSS接收MME认证数据
*参数：无
*返回值： 0成功
*        <0失败
*/
int receiver_Server() {
    int iSockfd;
    int iRet;
    unsigned char aucBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    // Register MME#Client
    struct sockaddr_in mmeSin4;
    // Creating socket file descriptor
    // Filling server information
    // Bind the socket with the server address
    iSockfd = NSP_TDYTH_4G_ANET_ipv4_recv_init(D_NSP_TDYTH_NAAAS_HSS_PORT, &mmeSin4);
    if (iSockfd < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "UDP ipv4 socket err", __LINE__, errno, strerror(errno));
    }
    while (1) {
        printf("HSS#Server: Waiting for data.\n");
        // Receive a datagram
        socklen_t mmeSin4Len = sizeof(mmeSin4);
        iRet = NSP_TDYTH_4G_ANET_ipv4_recv(iSockfd, aucBuf, D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN, &mmeSin4, mmeSin4Len, 1);
        if (iRet < 0) {
            printf("Failed to receive data.\n");
        }
        // Received MME IMSI and SN ID
        aucBuf[iRet] = '\0';
        printf("MME#Client: %s\n", aucBuf);
        return 0;
    }
}

/*
 * 功能：HSS端将用户ip及端口号存入单链表
 * 参数：
 * 返回值：   0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_UserInfo_Store(struct St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST **linklist, char *pcIp, int port, int *count)
{
    struct St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST *node, *temp;
    node = (struct St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST));
    if(node == NULL)
    {
        printf("add error\n");
        return -1;
    }

    node->iport = port;
    memcpy(node->aucIp, pcIp, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    node->iCount = *count+1;
    *count = *count + 1;

    if(*linklist != NULL)
    {
        temp = *linklist;
        *linklist = node;
        node->psNext = temp;
    } else
    {
        *linklist = node;
        node->psNext = NULL;
    }
    return 0;

}

/*
 * 功能：   HSS端对认证数据进行转发
 * 参数：	@pcRecvBuf [in]	    接收缓冲区
 *          @pcSendBuf [out]	发送缓冲区
 * 返回值：   0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_accessAuth_Auth_Data_Forward(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;
    printf("进入HSS接入认证处理流程!\n");
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS *stAuBodyFifthMMEToHSS = (St_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyFifthMMEToHSS->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }


/***************************************************************************************************************
    MYSQL *conn_ptr = NSP_TDYTH_4G_DB_connect();

    struct DBQueryInfo *pstDbQueryInfo = (struct DBQueryInfo *) malloc(sizeof(struct DBQueryInfo));

    //验证 IMSI 合法性，从数据库中查找是否存在等操作来判断合法性
    pstDbQueryInfo->keyName = "IMSI";
    if ((NSP_TDYTH_4G_DB_find(conn_ptr, pstDbQueryInfo)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_IMSI err", __LINE__, errno, strerror(errno));
        return -1;
    }

    //验证 SNID 合法性，从数据库中查找是否存在等操作来判断合法性
    pstDbQueryInfo->keyName = "SNID";
    if ((NSP_TDYTH_4G_DB_find(conn_ptr, pstDbQueryInfo)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_SNID err", __LINE__, errno, strerror(errno));
        return -1;
    }
***************************************************************************************************************/
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x06;

    //对认证数据进行转发
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodySixthHSSToAUC_LEN);

    //HSS端对用户信息进行保存
    memcpy(&s_stAuHSS, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_stAuHSS_LEN);

    return 0;
}
/*
 * 功能：   HSS端对认证数据进行响应
 * 参数：	@pcRecvBuf [in]	    接收缓冲区
 *          @pcSendBuf [out]	发送缓冲区
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_accessAuth_Auth_Data_Response(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    printf("进入HSS接入认证处理流程!\n");

    unsigned char *pucAV;
    unsigned char aucKasme[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];

    unsigned char aucAubody_hss[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME stAuBodyEighthHSSToMME;

    // 处理头部信息
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x08;

    // 为认证向量分配空间
    pucAV = (unsigned char*)malloc(sizeof(unsigned char) * D_NSP_TDYTH_NAAAS_4G_AV_LEN);
    if(pucAV == NULL)
    {
        printf("%s,[line=%d],errno=%d:%s\n","HSS:malloc err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 解析从AUC收到的包体内容
    memcpy(aucAubody_hss, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS *stAuBodySeventhAUCToHSS = (St_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS *)aucAubody_hss;
    if(NSP_TDYTH_4G_verification_timestamp(stAuBodySeventhAUCToHSS->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    // 计算KASME=KDF(SN ID, CK||IK)
    if ((iRet = NSP_TDYTH_4G_gen_Kamse(s_stAuHSS.aucSNID, stAuBodySeventhAUCToHSS->aucCK, stAuBodySeventhAUCToHSS->aucIK, stAuBodySeventhAUCToHSS->aucSQN, stAuBodySeventhAUCToHSS->aucAK, aucKasme)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_KASME err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 生成认证向量AV≔RAND∥XRES∥KASME∥AUTN
    memcpy(pucAV, stAuBodySeventhAUCToHSS->aucRAND, D_NSP_TDYTH_NAAAS_4G_RAND_LEN);
    memcpy(pucAV + D_NSP_TDYTH_NAAAS_4G_RAND_LEN, stAuBodySeventhAUCToHSS->aucXRES, D_NSP_TDYTH_NAAAS_4G_RES_LEN);
    memcpy(pucAV + D_NSP_TDYTH_NAAAS_4G_RAND_LEN + D_NSP_TDYTH_NAAAS_4G_RES_LEN, aucKasme, D_NSP_TDYTH_NAAAS_4G_KASME_LEN);
    memcpy(pucAV + D_NSP_TDYTH_NAAAS_4G_RAND_LEN + D_NSP_TDYTH_NAAAS_4G_RES_LEN + D_NSP_TDYTH_NAAAS_4G_KASME_LEN, stAuBodySeventhAUCToHSS->aucAUTN, D_NSP_TDYTH_NAAAS_4G_AUTN_LEN);

    // 构造第8步发送数据包结构体
    stAuBodyEighthHSSToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAuBodyEighthHSSToMME.aucIdNumber = 0x0;
    memcpy(stAuBodyEighthHSSToMME.aucDstId, stAuBodySeventhAUCToHSS->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAuBodyEighthHSSToMME.aucSrcId, stAuBodySeventhAUCToHSS->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAuBodyEighthHSSToMME.aucDstPort = stAuBodySeventhAUCToHSS->aucDstPort;
    stAuBodyEighthHSSToMME.aucSrcPort = stAuBodySeventhAUCToHSS->aucSrcPort;
    memcpy(stAuBodyEighthHSSToMME.aucAV, pucAV, D_NSP_TDYTH_NAAAS_4G_RAND_LEN+
                     D_NSP_TDYTH_NAAAS_4G_RES_LEN + D_NSP_TDYTH_NAAAS_4G_KASME_LEN+D_NSP_TDYTH_NAAAS_4G_AUTN_LEN);
    stAuBodyEighthHSSToMME.iDataLen = 0;

    memcpy(stAuBodyEighthHSSToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    // 构造第8步发送数据缓冲区
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodyEighthHSSToMME, D_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME_LEN);

    return 0;
}

/*
 * 功能：HSS端注册流程回复
 * 参数：  @pcRecvBuf [in] 接收缓冲区
 *          @pcSendBuf [out] 发送缓冲区
 * 返回值：   0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_Register_HSSSendtoUE(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    St_NSP_TDYTH_NAAAS_4G_ReBodySecondHSSToUE stRebodySecondHSSToUE;

    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_ReBodyFirstUEToHSS_LEN);

    St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS *stRebodyFirstUEToHSS = (St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS *)aucbody;
    if(NSP_TDYTH_4G_verification_timestamp(stRebodyFirstUEToHSS->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x00;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x02;

    stRebodySecondHSSToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stRebodySecondHSSToUE.aucIdNumber = 0x0;
    memcpy(stRebodySecondHSSToUE.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stRebodySecondHSSToUE.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stRebodySecondHSSToUE.iDataLen = 0;
    stRebodySecondHSSToUE.aucReqReg = 0x01;
    memcpy(stRebodySecondHSSToUE.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("注册回复数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    printf("the send step is register %x\n",stHeader.ucStep);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stRebodySecondHSSToUE, D_NSP_TDYTH_NAAAS_4G_ReBodySecondHSSToUE_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：请求用户信息第2步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_UserInfo_Request_Second_HSSSendToMME(char *pcRecvBuf, char *pcSendBuf, struct St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST *linklist)
{
    if(linklist == NULL)
    {
        printf("当前无注册用户\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME stURBodySecondHSSToMME;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS *stURBodyFirstMMEToHSS = (St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stURBodyFirstMMEToHSS->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    char acIps[500];
    int aiPorts[50];
    struct St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST *list = linklist;
    for (int i = 0; i < linklist->iCount; ++i)
    {
        memcpy(acIps+(i*16), list->aucIp, 16);
        aiPorts[i] = list->iport;
        list = list->psNext;
    }
    printf("%s\n",acIps);
    printf("%d\n",aiPorts[0]);


    int iRet;
    //   printf("------------------------Start to UserInfo Request------------------------\n");

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x02;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x02;

    stURBodySecondHSSToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stURBodySecondHSSToMME.aucIdNumber = 0x1;
    memcpy(stURBodySecondHSSToMME.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stURBodySecondHSSToMME.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stURBodySecondHSSToMME.iDataLen = 0;

    memcpy(stURBodySecondHSSToMME.aucIPs, acIps, 500);
    for (int j = 0; j < linklist->iCount; ++j) {
        stURBodySecondHSSToMME.aiPorts[j] = aiPorts[j];
    }
    stURBodySecondHSSToMME.iCount = linklist->iCount;

    memcpy(stURBodySecondHSSToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memset(pcSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stURBodySecondHSSToMME, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

int NSP_TDYTH_4G_DB_Left_Join_Select_IMSI(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_DBJoinQueryInfo *pstDbQueryInfo)
{
#ifdef DATABASE
    int iRet;
    char sql[500];

    char *select = "SELECT";
    char *from = "FROM";
    char *key = "NSP_TDYTH_HSS_USER_INFORMATION.IMSI,userName,Sex,IdentityCard,Category,DetailAddress,cardDate,Telephone";//Telephone
    char *left="LEFT JOIN";
    char *on="ON";
    char *where="WHERE";
    char *IMSI="NSP_TDYTH_SIM_INFORMATION.IMSI";

    sprintf(sql, "%s %s %s %s %s %s %s %s=%s %s %s=%s;", select,key,from,pstDbQueryInfo->tableName1,left,pstDbQueryInfo->tableName2,on,pstDbQueryInfo->limitName1,
            pstDbQueryInfo->limitName2,where,IMSI,pstDbQueryInfo->limitValue);

    MYSQL_RES *res;

    MYSQL_ROW row;

    iRet = mysql_real_query(conn_ptr, sql, strlen(sql));

    if (iRet)
    {
        printf("Failed to query: %s\n", mysql_error(conn_ptr));
        return -1;
    }

    res = mysql_store_result(conn_ptr);

    row = mysql_fetch_row(res);

    if(row == NULL)
    {
        printf("can not find result\n");
    }


    while (row)
    {
        int i;
        for (i = 0; i < mysql_num_fields(res); i++)
        {
            //printf("%s\t", row[i]);

        }
        //printf("\n");
        row = mysql_fetch_row(res);

    }
    mysql_free_result(res);
#endif
    return 0;
}

/*将结构体转换成CJSON文件
 *
 *
 *
 * */
int NSP_TDYTH_4G_CreatCJSON(St_NSP_TDYTH_NAAAS_4G_ToNAAAS stNoticeNAAAS, char *aubody){
    //创建root节点
    cJSON *root;
    root = cJSON_CreateObject();
    if(root == NULL){
        return -1;
    }
    //增加字符串数据
    cJSON_AddStringToObject(root,"cardID",stNoticeNAAAS.CardID);
    cJSON_AddStringToObject(root,"IMSI",stNoticeNAAAS.IMSI);
    cJSON_AddStringToObject(root,"HSSID",stNoticeNAAAS.HSSID);
    cJSON_AddStringToObject(root,"PIN",stNoticeNAAAS.PIN);
    cJSON_AddStringToObject(root,"PUK",stNoticeNAAAS.PUK);
    cJSON_AddStringToObject(root,"userName",stNoticeNAAAS.userName);
    cJSON_AddStringToObject(root, "sex", stNoticeNAAAS.Sex);
    cJSON_AddStringToObject(root,"identityCard",stNoticeNAAAS.IdentityCard);
    cJSON_AddStringToObject(root,"category",stNoticeNAAAS.Category);
    cJSON_AddStringToObject(root, "Country", stNoticeNAAAS.Country);
    cJSON_AddStringToObject(root,"province",stNoticeNAAAS.Province);
    cJSON_AddStringToObject(root,"city",stNoticeNAAAS.City);
    cJSON_AddStringToObject(root,"detailAddress",stNoticeNAAAS.DetailAddress);
    cJSON_AddStringToObject(root,"servicePasswd",stNoticeNAAAS.ServicePSW);
    //cJSON_AddStringToObject(root,"accountTime",TimeStamp);

    //将json结构转换成字符串
    char *NoticeNAAASJson = cJSON_Print(root);

    memcpy(aubody, NoticeNAAASJson, strlen(NoticeNAAASJson));

    //释放资源
    cJSON_Delete(root);

    return 0;
}

int NSP_TDYTH_4G_REGISTER_NOTICE(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_HSSDBInsertInfo *pstDbQueryInfo, char *aubody){

#ifdef DATABASE
    int iRet;
    char sql[500];
    //查询数据库
    char *select = "SELECT";
    char *from = "FROM";
    char *key = "cardID,IMSI,HSS_ID,PIN,PUK,userName,Sex,IdentityCard,Category,Country,Province,DetailAddress,City,servicePasswd";//

    sprintf(sql, "%s %s %s %s",select,key,from,pstDbQueryInfo->tableName);

    MYSQL_RES *res;
    MYSQL_ROW row;
    iRet = mysql_real_query(conn_ptr, sql, strlen(sql));
    if (iRet)
    {
        printf("Failed to query: %s\n", mysql_error(conn_ptr));
        return -1;
    }
    //保存数据库内容
    res = mysql_store_result(conn_ptr);
    row = mysql_fetch_row(res);

    if(row == NULL)
    {
        printf("can not find result\n");
        return -1;
    }
    //将数据读入结构体
    int flag = 1;
    St_NSP_TDYTH_NAAAS_4G_ToNAAAS stNoticeNAAAS ;
    memset(&stNoticeNAAAS, 0, sizeof(stNoticeNAAAS));
    while (flag)
    {
        int i;
        for (i = 0; i < mysql_num_fields(res); i++)
        {
            //printf("%s\t", row[i]);
        }
        //printf("\n");
        flag =0;
    }
    //保存字段
    memcpy(stNoticeNAAAS.CardID, row[0], 11);
    memcpy(stNoticeNAAAS.IMSI, row[1],strlen((row[1])));
    memcpy(stNoticeNAAAS.HSSID, row[2],strlen(row[2]));
    memcpy(stNoticeNAAAS.PIN, row[3],4);
    memcpy(stNoticeNAAAS.PUK, row[4],8);
    memcpy(stNoticeNAAAS.userName, row[5],strlen(row[5]));
    memcpy(stNoticeNAAAS.Sex, row[6],strlen(row[6]));
    memcpy(stNoticeNAAAS.IdentityCard,row[7],strlen(row[7]));
    memcpy(stNoticeNAAAS.Category, row[8], strlen(row[8]));
    memcpy(stNoticeNAAAS.Country, row[9], strlen(row[9]));
    memcpy(stNoticeNAAAS.Province,row[10], strlen(row[10]));
    memcpy(stNoticeNAAAS.City, row[11], strlen(row[11]));
    memcpy(stNoticeNAAAS.DetailAddress, row[12], strlen(row[12]));
    memcpy(stNoticeNAAAS.ServicePSW,row[13],strlen(row[13]));

//释放ROW
    row = mysql_fetch_row(res);
    //转换为CJSON字符串
    NSP_TDYTH_4G_CreatCJSON(stNoticeNAAAS, aubody);
    //释放res
    mysql_free_result(res);


#endif
    return 0;
}

int NSP_TDYTH_4G_DB_HSSinsert(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_HSSDBInsertInfo *pstDbInsertInfo)
{
#ifdef DATABASE
    int iRet = -1;
    char insertSQLQuery[500];
    char *insertSQLHead = "INSERT INTO ";
    char *insertSQLLeft = "(";
    char *insertSQLRight = ")";
    char *insertSQLValues = "VALUES";
    memset(insertSQLQuery,0,sizeof(insertSQLQuery));
/*    sprintf(insertSQLQuery, "%s%s %s%s%s %s %s%s%s;\n",
            insertSQLHead, pstDbInsertInfo->tableName, insertSQLLeft, pstDbInsertInfo->fieldKey,
            insertSQLRight, insertSQLValues, insertSQLLeft, pstDbInsertInfo->fieldValue, insertSQLRight);*/
    sprintf(insertSQLQuery, "%s %s %s"
                             "cardID,IMSI,HSS_ID,PIN,PUK,userName,Sex,IdentityCard,Category,Country,Province,DetailAddress,City,servicePasswd"
                             "%s %s %s"
                             "%s %s;\n",
            insertSQLHead,  pstDbInsertInfo->tableName,insertSQLLeft,insertSQLRight,insertSQLValues,
            insertSQLLeft, pstDbInsertInfo->fieldValue, insertSQLRight);

    iRet = mysql_real_query(conn_ptr, &insertSQLQuery, strlen(insertSQLQuery));

    return iRet;
#else
    return -1;
#endif
}

int NSP_TDYTH_4G_StoreUserinfo(MYSQL *conn_ptr, char *pcRecvBuf)
{
#ifdef DATABASE
    int iRet;
    unsigned char aucbody[500];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_ReBodyFirstUEToHSS_LEN);
    char *fieldStringValue = malloc(200);
    char woman[4] = "女";
    char man[4] = "男";

    St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS *stRebodyFirstUEToHSS = (St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS *)aucbody;
//    char acimsi[15];
//    memcpy(acimsi,(char *)stRebodyFirstUEToHSS->aucIMSI,15);
    typedef struct NSP_TDYTH_HSS_REGISTERFieldValue{
        char CardID[11];
        char IMSI[15];
        char HSSID[15];
        char PIN[4];
        char PUK[8];
        char userName[40];
        char Sex[5];
        char IdentityCard[18];
        char Category;
        char Country[20];
        char Province[20];
        char City[20];
        char DetailAddress[30];
        char ServicePSW[6];
        time_t Timestamp;
    }test;

    struct NSP_TDYTH_HSS_REGISTERFieldValue stfieldValue;
    memset(&stfieldValue,0,sizeof(test));
    memcpy(stfieldValue.CardID, stRebodyFirstUEToHSS->aucCardID, 11);
    memcpy(stfieldValue.IMSI, stRebodyFirstUEToHSS->aucIMSI,15);
    memcpy(stfieldValue.HSSID, stRebodyFirstUEToHSS->aucHSSID, 15);
    memcpy(stfieldValue.PIN, stRebodyFirstUEToHSS->aucPIN, 4);
    memcpy(stfieldValue.PUK, stRebodyFirstUEToHSS->aucPUK, 8);
    memcpy(stfieldValue.userName, stRebodyFirstUEToHSS->aucUserId,40);
    if(stRebodyFirstUEToHSS->aucSex == '1')
    {
        memcpy(stfieldValue.Sex,woman,strlen(woman));
    } else
    {
        memcpy(stfieldValue.Sex,man,strlen(man));
    }
    memcpy(stfieldValue.IdentityCard, stRebodyFirstUEToHSS->aucIdNum,18);
    stfieldValue.Category = stRebodyFirstUEToHSS->aucKind;
    memcpy(stfieldValue.Country, stRebodyFirstUEToHSS->aucCountry, 20);
    memcpy(stfieldValue.Province, stRebodyFirstUEToHSS->aucProvince, 20);
    memcpy(stfieldValue.City, stRebodyFirstUEToHSS->aucCity, 20);
    memcpy(stfieldValue.DetailAddress, stRebodyFirstUEToHSS->aucDetailAddr,30);
    memcpy(stfieldValue.ServicePSW, stRebodyFirstUEToHSS->aucServicePasswd, 6);
    stfieldValue.Timestamp = stRebodyFirstUEToHSS->TimeStamp;

    char *tableName = "NSP_TDYTH_HSS_USER_INFORMATION";
    //char *fieldKey = "IMSI,userName,Sex,IdentityCard,Category,DetailAddress";
     sprintf(fieldStringValue, "'%.*s','%.*s','%s','%.*s','%c','%.*s'",
            15,stfieldValue.IMSI, 15,stfieldValue.userName,stfieldValue.Sex, 20,
            stfieldValue.IdentityCard,stfieldValue.Category, 10,stfieldValue.DetailAddress);

    char *fieldKey = "cardID,IMSI,HSS_ID,PIN,PUK,userName,Sex,IdentityCard,Category,Country,Province,DetailAddress,City,servicePasswd";
    sprintf(fieldStringValue,"'%.*s','%.*s','%.*s','%.*s','%.*s','%.*s','%.*s','%.*s','%c','%.*s','%.*s','%.*s','%.*s','%.*s'",
            11,stfieldValue.CardID,15,stfieldValue.IMSI,15,stfieldValue.HSSID,4,stfieldValue.PIN,
            8,stfieldValue.PUK,40,stfieldValue.userName,5,stfieldValue.Sex,18,stfieldValue.IdentityCard,
            stfieldValue.Category,20,stfieldValue.Country,20,stfieldValue.Province,
            20,stfieldValue.City,30,stfieldValue.DetailAddress,6,stfieldValue.ServicePSW);

    St_NSP_TDYTH_NAAAS_4G_HSSDBInsertInfo *stHSSDBInsertInfo = (St_NSP_TDYTH_NAAAS_4G_HSSDBInsertInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_HSSDBInsertInfo));
    stHSSDBInsertInfo->tableName = tableName;
    stHSSDBInsertInfo->fieldKey = fieldKey;
    stHSSDBInsertInfo->fieldValue = fieldStringValue;
    //iRet = NSP_TDYTH_4G_DB_Newinsert(conn_ptr, stHSSDBInsertInfo);
    iRet = NSP_TDYTH_4G_DB_HSSinsert(conn_ptr, stHSSDBInsertInfo);
    if(iRet < 0)
    {
        printf("插入注册信息失败！\n");
    } else
    {
        printf("插入注册信息成功！\n");
    }


    char aubody[500];
    NSP_TDYTH_4G_REGISTER_NOTICE(conn_ptr,stHSSDBInsertInfo,aubody);
    if (aubody != NULL){
        printf("%s\n",aubody);
    }
    memcpy(gs_NAAAS_Body, aubody, strlen(aubody));

#endif
    return 0;

}

int NSP_TDYTH_4G_DB_selectIMSI(MYSQL *conn_ptr,char * IMSI)
{
#ifdef DATABASE
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *query;
    query = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));

    query->tableName = "NSP_TDYTH_SIM_INFORMATION";
    query->fieldKey = "IMSI";
    query->limitValue=(char *)malloc(D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(query->limitValue,IMSI,D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);


    int iRet;
    char sql[500];
    char *header = "select *";
    char *FROM = "FROM";
    char *WHERE = "WHERE";

    sprintf(sql, "%s %s %s %s %s ='%s';", header, FROM, query->tableName, WHERE, query->fieldKey, query->limitValue);

    MYSQL_RES *res;


    MYSQL_ROW row;

    iRet = mysql_real_query(conn_ptr, sql, strlen(sql));

    if (iRet)
    {
        printf("Failed to query: %s\n", mysql_error(conn_ptr));
        return -1;
    }

    res = mysql_store_result(conn_ptr);
    row = mysql_fetch_row(res);

    if(row != NULL)
    {
        return 0;
    } else
    {
        return -1;
    }

    mysql_free_result(res);
    return iRet;
#else
    return 0;
#endif
}

int NSP_TDYTH_4G_HSS_IMSI_HandlerRegister(char *pcRecvBuf)
{
#ifdef DATABASE
    int iRet;
    char aucBody[D_NSP_TDYTH_NAAAS_4G_ReBodyFirstUEToHSS_LEN];
    MYSQL *mysql;
    mysql = NSP_TDYTH_4G_DB_connect();
    char sql[500];
    sprintf(sql, "truncate table NSP_TDYTH_HSS_USER_INFORMATION;");
    mysql_real_query(mysql,sql ,strlen(sql));
    memcpy(aucBody,pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN,D_NSP_TDYTH_NAAAS_4G_ReBodyFirstUEToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS *pstBody = (St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS*)aucBody;


    // pstBody->aucIMSI  D_NSP_TDYTH_NAAAS_4G_IMSI_LEN
    if(NSP_TDYTH_4G_DB_selectIMSI(mysql,pstBody->aucIMSI) == 0)
    {
        printf("SIM卡信息正确！\n");
    }
    else
    {
        printf("SIM卡信息错误！\n");
        return -1;
    }
    if(NSP_TDYTH_4G_StoreUserinfo(mysql,pcRecvBuf)==0)
    {
        printf("用户注册成功\n");
    }
    else
    {
        printf("用户注册失败\n");
        return -1;
    }

    //left join
    St_NSP_TDYTH_NAAAS_4G_DBJoinQueryInfo *query;
    query = (St_NSP_TDYTH_NAAAS_4G_DBJoinQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBJoinQueryInfo));

    query->tableName1 = "NSP_TDYTH_HSS_USER_INFORMATION";
    query->tableName2 = "NSP_TDYTH_SIM_INFORMATION";
    query->limitName1 = "NSP_TDYTH_HSS_USER_INFORMATION.IMSI";
    query->limitName2 = "NSP_TDYTH_SIM_INFORMATION.IMSI";
    query->limitValue=(char *)malloc(D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(query->limitValue, pstBody->aucIMSI,D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    if(NSP_TDYTH_4G_DB_Left_Join_Select_IMSI(mysql, query)==0)
    {
        printf("left join query succeed\n");
    }
    else
    {
        printf("left join query faild\n");
        return -1;
    }

    NSP_TDYTH_4G_DB_close(mysql);
#endif
    return 0;
}

int NSP_TDYTH_4G_NOTICE_NAAAS(char *pcSendBuf){
    //构建包头
    St_NSP_TDYTH_NAAAS_4G_NAAASHEADER stNAAASHeader;
    stNAAASHeader.aucVersion = 0x0;
    stNAAASHeader.aucMode = 0x0;
    stNAAASHeader.aucReserved= 0x0;
    memcpy(stNAAASHeader.aucSQN ,"0000", sizeof(stNAAASHeader.aucSQN));
    //构建包体
    St_NSP_TDYTH_NAAAS_4G_NoticeNAAAS stNoticeNAAAS;
    stNoticeNAAAS.aucVersion = 0x0;
    memcpy(stNoticeNAAAS.aucCommand ,"00", sizeof(stNoticeNAAAS.aucCommand));
    memcpy(stNoticeNAAAS.aucBody, gs_NAAAS_Body, strlen(gs_NAAAS_Body));
    stNoticeNAAAS.aucDataLength = sizeof(stNoticeNAAAS);
    //计算总长度
    stNAAASHeader.aucLength = sizeof(stNAAASHeader) + stNoticeNAAAS.aucDataLength;
    //发送
    printf("推送至NAAAS完成\n");
    memcpy(pcSendBuf, &stNoticeNAAAS , sizeof(St_NSP_TDYTH_NAAAS_4G_NAAASHEADER));
    memcpy(pcSendBuf+sizeof(St_NSP_TDYTH_NAAAS_4G_NAAASHEADER), &stNoticeNAAAS, sizeof(St_NSP_TDYTH_NAAAS_4G_NoticeNAAAS));
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

int NSP_TDYTH_4G_IMSRegister_Fourth_HSSToICSCF(char *pcRecvBuf, char *pcSendBuf){
    unsigned char header[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    unsigned char diabody[D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN];
    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    memcpy(&header, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);

    pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_HEADER_LEN;
    memcpy(secbody, pcRecvBuf , D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader gSecHeader ={0};
    memcpy(&gSecHeader, secbody,D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);

    pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN;
    memcpy(diabody, pcRecvBuf ,D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    St_NSP_TDYTH_NAAAS_4G_Diameter_Header gDiaHeadereader = {0};
    memcpy(&gDiaHeadereader, diabody, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);

    pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN;
    memcpy(imsbody, pcRecvBuf ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS stImsBodyThirdICSCFToHSS = {0};
    memcpy(&stImsBodyThirdICSCFToHSS, imsbody,D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS_LEN);

    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader.TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //todo
    //检查data合法性
    if(stImsBodyThirdICSCFToHSS.iDataLen <= 0){
        printf("IMS第三步出错,数据包长度非法");
        return -1;
    }

    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, &header, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x04;
    stHeader.ucReqType = 0x0c;
    printf("ReqType:%d\n",stHeader.ucReqType);

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, &gSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

    //构建第三层头
    St_NSP_TDYTH_NAAAS_4G_Diameter_Header stDiaHeader;
    memcpy(&stDiaHeader,&gDiaHeadereader,D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    stDiaHeader.ucCommandFlags = 0x00;//回应


    //构建包体
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF stImsbodyFourthHSSToICSCF;
    stImsbodyFourthHSSToICSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF_LEN;
    memcpy(stImsbodyFourthHSSToICSCF.aucSCSCFName,"SCSCF1", D_NSP_TDYTH_NAAAS_4G_SCSCFName_LEN);
    memcpy(stImsbodyFourthHSSToICSCF.aucEnd, stImsBodyThirdICSCFToHSS.aucEnd, D_NSP_TDYTH_NAAAS_4G_END_LEN);


    // printf("S-CSCF的名字：%s\n","SCSCF1");
    printf("IMS-UE第四步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stDiaHeader, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN,
           &stImsbodyFourthHSSToICSCF,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

int NSP_TDYTH_4G_IMSRegister_Seventh_HSSToSCSCF(char *pcRecvBuf, char *pcSendBuf){



    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    unsigned char diabody[D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN];
    printf("recv!\n");
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

    memcpy(diabody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
           D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    St_NSP_TDYTH_NAAAS_4G_Diameter_Header  *gDiaHeadereader = (St_NSP_TDYTH_NAAAS_4G_Diameter_Header *)diabody;

    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN ,
           D_NSP_TDYTH_NAAAS_4G_IMSRegisterSixthSCSCFToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixthSCSCFToHSS *stImsBodySixthSCSCFToHSS = (St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixthSCSCFToHSS * )imsbody;
    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //todo
    //检查data合法性
    if(stImsBodySixthSCSCFToHSS->iDataLen <= 0){
        printf("IMS第一步出错,数据包长度非法");
        return -1;
    }

    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucK[D_NSP_TDYTH_NAAAS_4G_KEY_LEN];
    unsigned char aucSQN[D_NSP_TDYTH_NAAAS_4G_SQN_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAMF[D_NSP_TDYTH_NAAAS_4G_AMF_LEN];
    unsigned char aucMAC[D_NSP_TDYTH_NAAAS_4G_MAC_LEN];
    unsigned char aucXRES[D_NSP_TDYTH_NAAAS_4G_RES_LEN];
    unsigned char aucCK[D_NSP_TDYTH_NAAAS_4G_CK_LEN];
    unsigned char aucIK[D_NSP_TDYTH_NAAAS_4G_IK_LEN];
    unsigned char aucAK[D_NSP_TDYTH_NAAAS_4G_AK_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];
    unsigned char aucSNID[D_NSP_TDYTH_NAAAS_4G_SNID_LEN]= {0};
    unsigned char aucKasme[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];
    unsigned char aucAV[D_NSP_TDYTH_NAAAS_4G_AV_LEN];

    unsigned char aucAubody_auc[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char aucSQNAK[D_NSP_TDYTH_NAAAS_4G_AK_LEN];

    memset(aucAMF,0,D_NSP_TDYTH_NAAAS_4G_AMF_LEN);
    memset(aucK,0,D_NSP_TDYTH_NAAAS_4G_KEY_LEN);

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x07;

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();


    //构建第三层头
    St_NSP_TDYTH_NAAAS_4G_Diameter_Header stDiaHeader;
    memcpy(&stDiaHeader,pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN+D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    stDiaHeader.ucCommandFlags = 0x00;//回应
    /*stDiaHeader.ucVersion = 0x01;
    stDiaHeader.ucCommandFlags = 0x80;
    memcpy(stDiaHeader.aucCommandCode, "000001",D_NSP_TDYTH_NAAAS_4G_CommandCode_LEN);
    stDiaHeader.uiApplicationID = 0;
    stDiaHeader.uiHopbyHopID = random()%65535;
    stDiaHeader.uiEndtoEndID = 100000000;*/
    //构建包体

    printf("开始计算用户的AV向量");
    int iRet;
    //生成随机数RAND
    if ((iRet = NSP_TDYTH_4G_gen_RAND(aucRAND, D_NSP_TDYTH_NAAAS_4G_RAND_LEN)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_RAND err", __LINE__, errno, strerror(errno));
        return -1;
    }
    // 生成随机数SQN和RAND
    printf("开始计算认证向量.\n");
    if ((iRet = NSP_TDYTH_4G_gen_SQN(aucSQN, 0, D_NSP_TDYTH_NAAAS_4G_SQN_LEN)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_SQN err", __LINE__, errno, strerror(errno));
        return -1;
    }
    // 计算消息认证码 MAC=f1(K,SQN,RAND,AMF)
    if ((iRet = NSP_TDYTH_4G_gen_MAC(aucRAND, aucK, aucSQN, aucAMF, aucMAC)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_MAC err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算认证数据 XRES=f2(K,RAND)
    if ((iRet = NSP_TDYTH_4G_gen_RES(aucK, aucRAND, aucXRES)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_RES err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算密文加密密钥 CK=f3(K,RAND)
    if ((iRet = NSP_TDYTH_4G_gen_CK(aucK, aucRAND, aucCK)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_CK err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算完整性保护密钥 IK=f4(K,RAND)
    if ((iRet = NSP_TDYTH_4G_gen_IK(aucK, aucRAND, aucIK)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_CK err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算序列号保护密钥 AK=f5(K,RAND)
    if ((iRet = NSP_TDYTH_4G_gen_AK(aucK, aucRAND, aucAK)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_AK err", __LINE__, errno, strerror(errno));
        return -1;
    }
    // 计算SQN与AK异或
    if ((iRet = NSP_TDYTH_4G_XOR(aucSQN, aucAK, D_NSP_TDYTH_NAAAS_4G_AK_LEN, aucSQNAK)) != 0) //计算SQN^AK
    {
        printf("%s,[line=%d],errno=%d:%s\n", "xor err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }
    // 生成认证令牌 AUTN∶=SQN ⊕ AK∥AMF∥MAC
    if ((iRet = NSP_TDYTH_4G_gen_AUTN(aucSQN, aucAK, aucAMF, aucMAC, aucAUTN)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_AUTN err", __LINE__, errno, strerror(errno));
        return -1;
    }
    //计算Kasme
    if ((iRet = NSP_TDYTH_4G_gen_Kamse(aucSNID, aucCK, aucIK, aucSQN, aucAK ,aucKasme)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kasme err", __LINE__, errno, strerror(errno));
        return -1;
    }
    //拼接AV向量
    unsigned char *pAvi = aucAV;
    memcpy(pAvi, aucRAND, D_NSP_TDYTH_NAAAS_4G_RAND_LEN);
    pAvi += D_NSP_TDYTH_NAAAS_4G_RAND_LEN;
    memcpy(pAvi, aucXRES, D_NSP_TDYTH_NAAAS_4G_RES_LEN);
    pAvi += D_NSP_TDYTH_NAAAS_4G_RES_LEN;
    memcpy(pAvi, aucKasme, D_NSP_TDYTH_NAAAS_4G_KASME_LEN);
    pAvi += D_NSP_TDYTH_NAAAS_4G_KASME_LEN;
    memcpy(pAvi, aucAUTN,D_NSP_TDYTH_NAAAS_4G_AUTN_LEN);


    //构建包体
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventhHSSToSCSCF stImsbodySeventhHSSToSCSCF;
    memcpy(stImsbodySeventhHSSToSCSCF.aucAUTN, aucAUTN,D_NSP_TDYTH_NAAAS_4G_AUTN_LEN);
    memcpy(stImsbodySeventhHSSToSCSCF.aucXRES, aucXRES, D_NSP_TDYTH_NAAAS_4G_RES_LEN);
    memcpy(stImsbodySeventhHSSToSCSCF.aucRAND, aucRAND, D_NSP_TDYTH_NAAAS_4G_RAND_LEN);
    memcpy(stImsbodySeventhHSSToSCSCF.aucCK, aucCK, D_NSP_TDYTH_NAAAS_4G_CK_LEN);
    memcpy(stImsbodySeventhHSSToSCSCF.aucIK, aucIK,D_NSP_TDYTH_NAAAS_4G_IK_LEN);
    memcpy(stImsbodySeventhHSSToSCSCF.aucEnd, D_NSP_TDYTH_NAAAS_4G_END, D_NSP_TDYTH_NAAAS_4G_END_LEN);
    stImsbodySeventhHSSToSCSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventhHSSToSCSCF_LEN;

    printf("IMS-UE第7步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stDiaHeader, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN,
           &stImsbodySeventhHSSToSCSCF,D_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventhHSSToSCSCF_LEN);


    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

int NSP_TDYTH_4G_IMSRegister_Fourteenth_HSSToICSCF(char *pcRecvBuf, char *pcSendBuf){
    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    unsigned char diabody[D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN];
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

    memcpy(diabody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
           D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    St_NSP_TDYTH_NAAAS_4G_Diameter_Header *gDiaHeadereader = (St_NSP_TDYTH_NAAAS_4G_Diameter_Header *)diabody;

    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN ,
           D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS *stImsBodyThirteenthICSCFToHSS = (St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS * )imsbody;
    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //todo
    //检查data合法性
    if(stImsBodyThirteenthICSCFToHSS->iDataLen <= 0){
        printf("IMS第十三步出错,数据包长度非法");
        return -1;
    }

    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0e;

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

    //构建第三层头
    St_NSP_TDYTH_NAAAS_4G_Diameter_Header stDiaHeader;
    memcpy(&stDiaHeader,pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN+D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    stDiaHeader.ucCommandFlags = 0x00;//回应
    /*stDiaHeader.ucVersion = 0x01;
    stDiaHeader.ucCommandFlags = 0x80;
    memcpy(stDiaHeader.aucCommandCode, "300",D_NSP_TDYTH_NAAAS_4G_CommandCode_LEN);
    stDiaHeader.uiApplicationID = 0;
    stDiaHeader.uiHopbyHopID = random()%65535;
    stDiaHeader.uiEndtoEndID = 100000000;*/

    //构建包体
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF stImsbodyFourteenthHSSToICSCF;
    stImsbodyFourteenthHSSToICSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF_LEN;
    memcpy(stImsbodyFourteenthHSSToICSCF.aucSCSCFName,"SCSCF1", D_NSP_TDYTH_NAAAS_4G_SCSCFName_LEN);
    memcpy(stImsbodyFourteenthHSSToICSCF.aucEnd, stImsBodyThirteenthICSCFToHSS->aucEnd, D_NSP_TDYTH_NAAAS_4G_END_LEN);


    // printf("S-CSCF的名字：%s\n","SCSCF1");
    printf("IMS-UE第十四步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stDiaHeader, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN,
           &stImsbodyFourteenthHSSToICSCF,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

int NSP_TDYTH_4G_IMSRegister_Seventeenth_HSSToSCSCF(char *pcRecvBuf, char *pcSendBuf){
    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    unsigned char diabody[D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN];
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;
    printf("success\n");

    memcpy(diabody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
           D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    St_NSP_TDYTH_NAAAS_4G_SIPHeader *gDiaHeadereader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *)diabody;

    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN ,
           D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixteenthSCSCFToHSS *stImsBodySixteenthSCSCFToHSS = (St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixteenthSCSCFToHSS * )imsbody;
    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //todo
    //检查data合法性
    if(stImsBodySixteenthSCSCFToHSS->iDataLen <= 0){
        printf("IMS第16步出错,数据包长度非法");
        return -1;
    }

    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x11;//17

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

    //构建第三层头
    St_NSP_TDYTH_NAAAS_4G_Diameter_Header stDiaHeader;
    memcpy(&stDiaHeader,pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN+D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    stDiaHeader.ucCommandFlags = 0x00;//回应
    /*stDiaHeader.ucVersion = 0x01;
    stDiaHeader.ucCommandFlags = 0x80;
    memcpy(stDiaHeader.aucCommandCode, "300",D_NSP_TDYTH_NAAAS_4G_CommandCode_LEN);
    stDiaHeader.uiApplicationID = 0;
    stDiaHeader.uiHopbyHopID = random()%65535;
    stDiaHeader.uiEndtoEndID = 100000000;*/

    //构建包体
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventeenthHSSToSCSCF stImsbodySeventeenthHSSToSCSCF;
    stImsbodySeventeenthHSSToSCSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF_LEN;
    //todo
    //通知HSS用户注册成功，怎么在HSS体现出来?现在只是单纯的printf一下
    printf("S-CSCF通知HSS用户注册成功，并要求HSS下发该用户的签约数据\n");
    memcpy(stImsbodySeventeenthHSSToSCSCF.aucEnd, stImsBodySixteenthSCSCFToHSS->aucEnd, D_NSP_TDYTH_NAAAS_4G_END_LEN);

    printf("IMS-UE第17步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stDiaHeader, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN,
           &stImsbodySeventeenthHSSToSCSCF,D_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventeenthHSSToSCSCF_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/***********************************************************************TAU***********************************************************************/
/*
 * 功能：  TAU流程第12步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Twelfth_HSSToOldMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME stTauBodyTwelfthHSSToOldMME;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS *stTauBodyEleventhNewMMEToHSS = (St_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyEleventhNewMMEToHSS->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0c;

    stTauBodyTwelfthHSSToOldMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    //此处应该查询数据库配置目标Old MME的IP和端口号，因为我没怎么看HSS的数据库操作，这里简化处理。By：ZCL
    if(stTauBodyEleventhNewMMEToHSS->uiSrcPort == D_NSP_TDYTH_NAAAS_MME1_PORT)
    {
        memcpy(stTauBodyTwelfthHSSToOldMME.aucDstId, D_NSP_TDYTH_NAAAS_MME2_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
        stTauBodyTwelfthHSSToOldMME.uiDstPort = D_NSP_TDYTH_NAAAS_MME2_PORT;
    }else if(stTauBodyEleventhNewMMEToHSS->uiSrcPort == D_NSP_TDYTH_NAAAS_MME2_PORT){
        memcpy(stTauBodyTwelfthHSSToOldMME.aucDstId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
        stTauBodyTwelfthHSSToOldMME.uiDstPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    }else{
        printf("Step 11. 收到的Update Location Request包中的New MME地址和端口有误\n");
        return -1;
    }
    memcpy(stTauBodyTwelfthHSSToOldMME.aucSrcId, D_NSP_TDYTH_NAAAS_HSS_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyTwelfthHSSToOldMME.uiSrcPort = D_NSP_TDYTH_NAAAS_HSS_PORT;
    stTauBodyTwelfthHSSToOldMME.uiDataLen = 0;

    memcpy(stTauBodyTwelfthHSSToOldMME.aucTerminalId, stTauBodyEleventhNewMMEToHSS->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodyTwelfthHSSToOldMME.aucIMSI, stTauBodyEleventhNewMMEToHSS->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyTwelfthHSSToOldMME, D_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyTwelfthHSSToOldMME.aucDstId, stTauBodyTwelfthHSSToOldMME.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  TAU流程第14步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Fourteenth_HSSToNewMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME stTauBodyFourteenthHSSToNewMME;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS *stTauBodyThirteenthOldMMEToHSS = (St_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyThirteenthOldMMEToHSS->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0e;

    stTauBodyFourteenthHSSToNewMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    //此处应该查询数据库配置目标Old MME的IP和端口号，因为我没怎么看HSS的数据库操作，这里简化处理。By：ZCL
    if(stTauBodyThirteenthOldMMEToHSS->uiSrcPort == D_NSP_TDYTH_NAAAS_MME1_PORT)
    {
        memcpy(stTauBodyFourteenthHSSToNewMME.aucDstId, D_NSP_TDYTH_NAAAS_MME2_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
        stTauBodyFourteenthHSSToNewMME.uiDstPort = D_NSP_TDYTH_NAAAS_MME2_PORT;
    }else if(stTauBodyThirteenthOldMMEToHSS->uiSrcPort == D_NSP_TDYTH_NAAAS_MME2_PORT){
        memcpy(stTauBodyFourteenthHSSToNewMME.aucDstId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
        stTauBodyFourteenthHSSToNewMME.uiDstPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    }else{
        printf("Step 11. 收到的Update Location Request包中的New MME地址和端口有误\n");
        return -1;
    }
    memcpy(stTauBodyFourteenthHSSToNewMME.aucSrcId, D_NSP_TDYTH_NAAAS_HSS_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyFourteenthHSSToNewMME.uiSrcPort = D_NSP_TDYTH_NAAAS_HSS_PORT;
    stTauBodyFourteenthHSSToNewMME.uiDataLen = 0;

    memcpy(stTauBodyFourteenthHSSToNewMME.aucTerminalId, stTauBodyThirteenthOldMMEToHSS->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodyFourteenthHSSToNewMME.aucIMSI, stTauBodyThirteenthOldMMEToHSS->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyFourteenthHSSToNewMME, D_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyFourteenthHSSToNewMME.aucDstId, stTauBodyFourteenthHSSToNewMME.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/***********************************************************************TAU结束***********************************************************************/

/*
 * 功能：HSS端接收监听
 * 参数：  
 * 返回值：   0   成功
 *          < 0 失败
 */
void * NSP_TDYTH_4G_ListenAccept_HSS_Function(int * iSockfd)
{
    //一直接收数据
    int iRet;
    static struct St_NSP_TDYTH_NAAAS_4G_HSS_LINKLIST *linklist = NULL;
    static int count = 0;

    /* -------------------------------------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];

    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();

    printf(" 当前线程信息为 :pid %u tid %u (0x%x)\n", (unsigned int) pid,
           (unsigned int) tid, (unsigned int) tid);
    // UE
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    // MME
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    // AUC
    struct sockaddr_in AUCSockaddr;
    bzero(&AUCSockaddr, sizeof(AUCSockaddr));
    AUCSockaddr.sin_family = AF_INET;
    AUCSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_AUC_PORT);
    AUCSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_AUC_IP);

    //ICSCF
    struct sockaddr_in ICSCFSockaddr;
    bzero(&ICSCFSockaddr, sizeof(ICSCFSockaddr));
    ICSCFSockaddr.sin_family = AF_INET;
    ICSCFSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ICSCF_PORT);
    ICSCFSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ICSCF_IP);

    //SCSCF
    struct sockaddr_in SCSCFSockaddr;
    bzero(&SCSCFSockaddr, sizeof(SCSCFSockaddr));
    SCSCFSockaddr.sin_family = AF_INET;
    SCSCFSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_SCSCF_PORT);
    SCSCFSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_SCSCF_IP);

    // HSS
    struct sockaddr_in HSSSockaddr;
    bzero(&HSSSockaddr, sizeof(HSSSockaddr));
    HSSSockaddr.sin_family = AF_INET;
    HSSSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_HSS_PORT);
    HSSSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_HSS_IP);

    //NAAAS
    struct sockaddr_in NAAASSockaddr;
    bzero(&NAAASSockaddr, sizeof(NAAASSockaddr));
    NAAASSockaddr.sin_family = AF_INET;
    NAAASSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PORT);
    NAAASSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_IP);

    int iHSSAddrLen = sizeof(HSSSockaddr);

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    iRet = recvfrom(*iSockfd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN, 0,
                    (struct sockaddr *) &HSSSockaddr, &iHSSAddrLen);

    if (iRet > 0)
    {
        memcpy(cHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
        St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *)cHeader;             // 解析从mme发来的包头
        //printf("step: %d\n", pstHeader->ucStep);
        switch (pstHeader->ucReqType)   // 判断请求类别
        {
            case 0x00:          // 请求注册流程
                printf("----------------------目前处于注册流程！---------------------\n");
                switch (pstHeader->ucStep)     // 判断步骤码
                {   case 0x01:      // 第 1 步  接收从UE发送的用户注册请求

                        printf("------------------------step1. HSS#Receiver <- UE：0x01------------------------\n");
                       // NSP_TDYTH_4G_UserInfo_Store(&linklist, inet_ntoa(HSSSockaddr.sin_addr), ntohs(HSSSockaddr.sin_port), &count);

                        #ifdef DATABASE
                        if(NSP_TDYTH_4G_HSS_IMSI_HandlerRegister(cRecvBuf) < 0) break;
                        if(NSP_TDYTH_4G_NOTICE_NAAAS(cSendBuf) < 0) break;
                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                           (struct sockaddr *) &NAAASSockaddr, sizeof(NAAASSockaddr))) < 0) {
                            printf("1\n");
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        }
                        #endif

                        if(NSP_TDYTH_4G_Register_HSSSendtoUE(cRecvBuf, cSendBuf) < 0) break;
                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN, 0, (struct sockaddr *) &HSSSockaddr, sizeof(UESockaddr))) < 0)
                        {
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        }
                        else
                        {
                            printf("用户注册响应发送完毕,step: 2 完成.\n");
                        }

                        printf("------------------------step2. HSS#Sender -> UE：0x02------------------------\n");
                        break;
                    default:
                        printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                        break;
                }

                break;

            case 0x01:          // 请求认证流程
                printf("----------------------目前处于认证流程！---------------------\n");
                switch (pstHeader->ucStep)      // 判断步骤码
                {
                    case 0x05:      // 第 5 步  接收从MME发送的用户认证数据请求
                        printf("------------------------step5. HSS#Receiver <- MME：0x05------------------------\n");

                        if(NSP_TDYTH_4G_accessAuth_Auth_Data_Forward(cRecvBuf, cSendBuf) < 0) break;

                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN, 0,
                                (struct sockaddr *) &AUCSockaddr, sizeof(AUCSockaddr))) < 0)
                        {
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        }
                        else
                        {
                            printf("认证数据转发完毕,step: 6 完成.\n");
                        }
                        printf("------------------------step6. HSS#Sender -> AUC：0x06------------------------\n");

                        break;

                    case 0x07:      // 第 7 步 接收从AUC发送的用户认证数据响应
                        printf("------------------------step7. HSS#Receiver <- AUC：0x07------------------------\n");

                        if(NSP_TDYTH_4G_accessAuth_Auth_Data_Response(cRecvBuf, cSendBuf) < 0) break;

                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN, 0,
                                (struct sockaddr *) &MMESockaddr, sizeof(MMESockaddr))) < 0)
                        {
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        }
                        else
                        {
                            printf("认证数据响应完毕,step: 8 完成.\n");
                        }
                        printf("------------------------step8. HSS#Sender -> MME：0x08------------------------\n");

                        break;

                    default:
                        printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                        break;

                }

                break;
                
            case 0x02:          // 请求用户信息流程
                printf("----------------------目前处于请求用户信息流程！---------------------\n");
                switch (pstHeader->ucStep)      // 判断步骤码
                {
                    case 0x01:      // 第 1 步  接收从MME发送的用户信息数据请求
                        printf("------------------------step1. HSS#Receiver <- MME：0x01------------------------\n");
                        if(NSP_TDYTH_4G_UserInfo_Request_Second_HSSSendToMME(cRecvBuf, cSendBuf, linklist) < 0)
                        {
                            break;
                        }

                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN, 0, (struct sockaddr *) &MMESockaddr, sizeof(MMESockaddr))) < 0)
                        {
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        }
                        else
                        {
                            printf("UE信息发送完毕,step: 2 完成.\n");
                        }
                        printf("------------------------step2. HSS#Sender -> MME：0x02------------------------\n");

                        break;

                    default:
                        printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                        break;

                }

                break;
            case 0x0c:         //IMS注册流程
                printf("----------------------目前处于IMS流程！---------------------\n");
                switch (pstHeader->ucStep) {
                    case 0x03:
                        printf("------------------------step3. HSS#Receiver ->ICSCF:0x03------------------------\n");
                        if(NSP_TDYTH_4G_IMSRegister_Fourth_HSSToICSCF(cRecvBuf ,cSendBuf) < 0) break;
                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                           (struct sockaddr *) &ICSCFSockaddr, sizeof(ICSCFSockaddr))) < 0) {
                            printf("1\n");
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        } else
                        {
                            printf("------------------------step4. HSS#Sender -> ICSCF：0x04------------------------\n");
                        }
                        break;
                    case 0x06:
                        printf("------------------------step6. HSS#Receiver ->SCSCF:0x06------------------------\n");
                        if(NSP_TDYTH_4G_IMSRegister_Seventh_HSSToSCSCF(cRecvBuf ,cSendBuf) < 0) break;
                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                           (struct sockaddr *) &SCSCFSockaddr, sizeof(SCSCFSockaddr))) < 0) {
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        } else
                        {
                            printf("------------------------step7. HSS#Sender -> SCSCF：0x07------------------------\n");
                        }
                        break;
                    case 0x0d:
                        printf("------------------------step13. HSS#Receiver ->ICSCF:0x06------------------------\n");
                        if(NSP_TDYTH_4G_IMSRegister_Fourteenth_HSSToICSCF(cRecvBuf ,cSendBuf) < 0) break;
                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                           (struct sockaddr *) &ICSCFSockaddr, sizeof(ICSCFSockaddr))) < 0) {
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        } else
                        {
                            printf("------------------------step14. HSS#Sender -> ICSCF：0x0e------------------------\n");
                        }
                        break;
                    case 0x10:
                        printf("------------------------step16. HSS#Receiver ->SCSCF:0x10------------------------\n");
                        if(NSP_TDYTH_4G_IMSRegister_Seventeenth_HSSToSCSCF(cRecvBuf ,cSendBuf) < 0) break;
                        if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                           (struct sockaddr *) &SCSCFSockaddr, sizeof(SCSCFSockaddr))) < 0) {
                            printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                            return NULL;
                        } else
                        {
                            printf("------------------------step17. HSS#Sender -> SCSCF：0x11------------------------\n");
                        }
                        break;
                    default:
                        printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                        break;

                }
                break;
            case 0x13:
                printf("\n----------目前处于TAU流程----------\n\n");
                switch (pstHeader->ucStep)
                {
                    case 0x0b:
                        printf("----------TAU: step11. HSS#Receiver -> New MME：Update Location Request----------\n");
                        if(NSP_TDYTH_4G_TAU_Twelfth_HSSToOldMME(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                            printf("----------TAU: step12. SEND ERROR----------\n");
                            break;
                        }else {
                            printf("----------TAU: step12. HSS#Sender -> Old MME：Update Location Request----------\n");
                        }
                        break;
                    case 0x0d:
                        printf("----------TAU: step13. HSS#Receiver -> Old MME：Cancel Location Ack----------\n");
                        if(NSP_TDYTH_4G_TAU_Fourteenth_HSSToNewMME(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                            printf("----------TAU: step14. SEND ERROR----------\n");
                            break;
                        }else {
                            printf("----------TAU: step14. HSS#Sender -> New MME：Update Location Answer----------\n");
                        }
                        break;
                    default:
                        printf("收到TAU包的步骤信息无法识别！[Step=%d]\n", pstHeader->ucStep);
                        break;
                }
                break;
            default:
                printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                break;
        }
    }
    else
    {
        if (errno != EAGAIN)
        {
            perror("连接失败");
            exit(1);
        }
    }
    
    return NULL;
}

/*
 * 功能：采用epoll模型，创建线程处理消息
 * 参数：  无
 * 返回值：  0   成功
 *         <0   失败
 */
int NSP_TDYTH_4G_Epoll_HSS_Function()
{
    int iFD,kdpfd,nfds,n,curfds;

    struct epoll_event ev;
    struct epoll_event events[D_NSP_TDYTH_NAAAS_4G_MAXEPOLLSIZE];
    struct rlimit rt;

    pthread_t thread;
    pthread_attr_t attr;

    /* 设置每个进程允许打开的最大文件数 */
    rt.rlim_max = rt.rlim_cur = 100;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
    {
        perror("setrlimit");
        exit(1);
    }

    /* 开启 socket 监听 */
    if ((iFD = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket create failed");
        exit(1);
    }

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iFD,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iFD);

    //ENB Sockaddr
    struct sockaddr_in HSSSockaddr;
    bzero(&HSSSockaddr, sizeof(HSSSockaddr));
    HSSSockaddr.sin_family = AF_INET;
    HSSSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_HSS_PORT);
    HSSSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_HSS_IP);

    if (bind(iFD, (struct sockaddr *) &HSSSockaddr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    kdpfd = epoll_create(D_NSP_TDYTH_NAAAS_4G_MAXEPOLLSIZE);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = iFD;

    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, iFD, &ev) < 0)
    {
        fprintf(stderr, "epoll set insertion error:fd=%d",iFD);
        return -1;
    }

    for( ; ; )
    {
        nfds = epoll_wait(kdpfd, events, 10000, 500);

        /* 处理所有事件 */
        for (n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == iFD)
            {
                /*初始化属性值，均设为默认值*/
                pthread_attr_init(&attr);
                pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

                /*  设置线程为分离属性*/
                pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
                if(pthread_create(&thread,&attr,(void*)NSP_TDYTH_4G_ListenAccept_HSS_Function,(void*)&(events[n].data.fd)))
                {
                    perror("pthread_creat error!\n");
                    exit(-1);
                }
            }
        }
    }
    close(kdpfd);
    close(iFD);
    return 0;
}

int main()
{
    puts("HSS开启监听状态");
    NSP_TDYTH_4G_Epoll_HSS_Function();
    return 0;
}
