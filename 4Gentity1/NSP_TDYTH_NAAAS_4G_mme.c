/*
 * 文件功能：MME功能
 * 作者：董珂 李高明 陈李兰 张一帆
 * 创建日期：2020/02/10
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_anet.h"
#include "NSP_TDYTH_NAAAS_4G_utils.h"
#include "NSP_TDYTH_NAAAS_4G_gen.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"
#include "NSP_TDYTH_NAAAS_4G_debug.h"
#include "NSP_TDYTH_NAAAS_4G_db.h"


/*
 * 备注：在认证过程中，MME端需要保存一些信息，按照文档中数据库对应表，暂时保存这些信息，MME端保存信息结构体自行定义和保存。
 */

/*
 * 功能：MME端存储信息结构体
 * 参数：	@xxx [in]	xxx
 *          @xxx [out]	xxx
 * 返回值：
 */
typedef struct {
    unsigned int uiID_MME;
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucAV[D_NSP_TDYTH_NAAAS_4G_AV_LEN];
    unsigned char aucHSSID;
    unsigned char aucSNID[D_NSP_TDYTH_NAAAS_4G_SNID_LEN];
} __attribute__((packed)) NSP_TDYTH_4G_AuMME;

static NSP_TDYTH_4G_AuMME s_stMME;

static unsigned int gs_imme_enb_id = 3000;                                 //MME分给ENB的id
static unsigned int gs_imme_ue_id = 4000;                                 //MME分给UE的id

KeyPair keypair[7];

//清空MME、HSS用户信息表
int NSP_TDYTH_4G_CleanOut()
{
#ifdef DATABASE
    int iRet;
    MYSQL *mysql;
    mysql = NSP_TDYTH_4G_DB_connect();
    char *pcsql1 = "delete from NSP_TDYTH_MME_USER_INFORMATION WHERE IMSI!='001010123123123' and IMSI !='001010123123456';";
    iRet = mysql_query(mysql, pcsql1);
    if (iRet < 0)
    {
        printf("Failed to query: %s\n", mysql_error(mysql));
        return -1;
    }
    char *pcsql2 = "delete from NSP_TDYTH_HSS_USER_INFORMATION WHERE IMSI!='001010123123123' and IMSI !='001010123123456';";
    iRet = mysql_query(mysql, pcsql2);
    if (iRet < 0)
    {
        printf("Failed to query: %s\n", mysql_error(mysql));
        return -1;
    }
    NSP_TDYTH_4G_DB_close(mysql);
#else 
    printf("未启用数据库代码，请启用！(宏定义DATABASE)\n");
#endif

    return 0;

}
/*
 * 功能：展示注册用户的信息
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int  NSP_TDYTH_4G_show_register_userInfo()
{
#ifdef DATABASE
    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();
    char *sql = "select IMSI,userName,Sex,IdentityCard,Category,DetailAddress from NSP_TDYTH_HSS_USER_INFORMATION;";
    int iRet = mysql_query(mysql, sql);
    if (iRet < 0)
    {
        printf("Failed to query: %s\n", mysql_error(mysql));
        return -1;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;

    res = mysql_store_result(mysql);

    row = mysql_fetch_row(res);

    if(row == NULL)
    {
        printf("no result\n");
    }
    while (row)
    {

        printf("%s\t%s\t%s\t%s\t%s\t", row[0],row[1],row[2],row[3],row[4]);
        printf_dae_mmesend("%s\t%s\t%s\t%s\t%s\t", row[0],row[1],row[2],row[3],row[4]);

        printf("\n");

        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);

    NSP_TDYTH_4G_DB_close(mysql);
#else 
    printf("未启用数据库代码，请启用！(宏定义DATABASE)\n");
#endif

    return 0;
}

/*
 * 功能：展示在线用户的信息
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_show_online_userInfo()
{
#ifdef DATABASE
    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();
    char *sql = "select IMSI,MME_UEid,Online,Certification,Connection from NSP_TDYTH_MME_USER_INFORMATION where Online = '在线';";
    int iRet = mysql_query(mysql, sql);
    if (iRet < 0)
    {
        printf("Failed to query: %s\n", mysql_error(mysql));
        return -1;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;

    res = mysql_store_result(mysql);

    row = mysql_fetch_row(res);

    if(row == NULL)
    {
        printf("no result\n");
    }

    while (row)
    {
        printf("%s\t%s\t%s\t%s\t%s\t", row[0],row[1],row[2],row[3],row[4]);
        printf_dae_mmesend("%s\t%s\t%s\t%s\t%s\t", row[0],row[1],row[2],row[3],row[4]);
        printf("\n");
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);

    NSP_TDYTH_4G_DB_close(mysql);
#else 
    printf("未启用数据库代码，请启用！(宏定义DATABASE)\n");
#endif

    return 0;
}

/*
 * 功能：网络拓扑展示
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_NetworkTopology()
{
#ifdef DATABASE
    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();
    char *sql = "select IMSI,MME_UEid,Online,Certification,Connection from NSP_TDYTH_MME_USER_INFORMATION where Certification = '已认证';";
    int iRet = mysql_query(mysql, sql);
    if (iRet < 0)
    {
        printf("Failed to query: %s\n", mysql_error(mysql));
        return -1;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;

    res = mysql_store_result(mysql);

    row = mysql_fetch_row(res);

    if(row == NULL)
    {
        printf("no result\n");
        printf_dae_mmesend("no result\n");
    }

    while (row)
    {
        printf("%s\t%s\t%s\t%s\t%s\t", row[0],row[1],row[2],row[3],row[4]);
        printf_dae_mmesend("%s\t%s\t%s\t%s\t%s\t", row[0],row[1],row[2],row[3],row[4]);
        printf("\n");
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);

    NSP_TDYTH_4G_DB_close(mysql);
#else 
    printf("未启用数据库代码，请启用！(宏定义DATABASE)\n");
#endif
    return 0;
}

//查询表中是否出现指定的元素
int NSP_TDYTH_4G_select_onlyinfo_by_locate(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *pstDbQueryInfo)
{
#ifdef DATABASE
    int iRet;
    char sql[500];
    char *header = "select";
    char *FROM = "FROM";
    char *WHERE = "WHERE locate";
    char *SQLLeft = "(";
    char *SQLRight = ")";
    char *SQLComma = ",";

    sprintf(sql, "%s %s %s %s %s %s'%s'%s%s%s;", header, pstDbQueryInfo->fieldKey, FROM, pstDbQueryInfo->tableName, WHERE, SQLLeft, pstDbQueryInfo->limitValue,SQLComma,pstDbQueryInfo->fieldKey,SQLRight);

    printf("querySQLQuery: %s\n", sql);
    MYSQL_RES *res;

    unsigned long *lengths;

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
    printf("未启用数据库代码，请启用！(宏定义DATABASE)\n");
#endif
    return 0;
}

/*
 * 功能：附着完成后，插入信息
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_attach_insert(char *Recvbuf)
{
#ifdef DATABASE
    int iRet;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME_LEN];
    memcpy(aucbody, Recvbuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME *stAttBodyFourtheNBToMME = (St_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME *) aucbody;

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    char acIMSI[15];
    memcpy(acIMSI, stAttBodyFourtheNBToMME->aucIMSI, 15);

    //查询以往的记录中imsi是否存在
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *) malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery->fieldKey = "IMSI";
    stquery->limitValue = acIMSI;
    if(NSP_TDYTH_4G_select_onlyinfo_by_locate(mysql, stquery) == 0)
    {
        char sql1[200];
        char *online = "在线";
        sprintf(sql1, "update(NSP_TDYTH_MME_USER_INFORMATION)SET Online='%s',MME_UEid=%d,MME_ENBid=%d,ENB_UEid=%d where IMSI='%s';", online,++gs_imme_ue_id,++gs_imme_enb_id,stAttBodyFourtheNBToMME->uiEnb_Ue_Id,
                acIMSI);

        iRet = mysql_query(mysql, sql1);
        if (iRet < 0) {
            printf("更新失败！\n");
            return -1;
        }
    }else {
        struct NSP_TDYTH_MME_ATTACHFieldValue {
            char IMSI[15];
            char UE_IP[15];
            unsigned int MME_UEid;
            unsigned int MME_ENBid;
            unsigned int ENB_UEid;
            char Online[20];
            char Connection[20];
        };

        char *tableName = "NSP_TDYTH_MME_USER_INFORMATION";
        char *fieldKey = "IMSI, UE_IP,MME_UEid, MME_ENBid, ENB_UEid, Online, Connection,UE_PORT";
        char *fieldStringValue = malloc(200);
        //  MYSQL *mysql = NSP_TDYTH_4G_DB_connect();
        struct NSP_TDYTH_MME_ATTACHFieldValue stfieldValue;

        memcpy(stfieldValue.IMSI, stAttBodyFourtheNBToMME->aucIMSI, 15);
        memcpy(stfieldValue.UE_IP, stAttBodyFourtheNBToMME->aucSrcId, 16);
        stfieldValue.ENB_UEid = stAttBodyFourtheNBToMME->uiEnb_Ue_Id;
        stfieldValue.MME_ENBid = ++gs_imme_enb_id;
        stfieldValue.MME_UEid = ++gs_imme_ue_id;
        memcpy(stfieldValue.Online,"在线", 7);
        memcpy(stfieldValue.Connection, "连接", 7);

        printf_dae_uesend("当前UE_ID为：%d\n",gs_imme_ue_id);

        sprintf(fieldStringValue, "'%.*s','%.*s','%d','%d','%d','%s','%s'", 15, stfieldValue.IMSI, 15,
                stfieldValue.UE_IP,
                stfieldValue.MME_UEid, stfieldValue.MME_ENBid, stfieldValue.ENB_UEid,stfieldValue.Online,
                stfieldValue.Connection);
        St_NSP_TDYTH_NAAAS_4G_DBInsertInfo *insertInfo = (St_NSP_TDYTH_NAAAS_4G_DBInsertInfo *) malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBInsertInfo));
        insertInfo->tableName = tableName;
        insertInfo->fieldKey = fieldKey;
        insertInfo->fieldStringValue = fieldStringValue;
        insertInfo->port = stAttBodyFourtheNBToMME->aucSrcPort;

        iRet = NSP_TDYTH_4G_DB_Newinsert(mysql, insertInfo);
        if (iRet < 0) {
            printf("Failed to insert data\n");
            return -1;
        }
    }
    NSP_TDYTH_4G_DB_close(mysql);
#else 
    printf("未启用数据库代码，请启用！(宏定义DATABASE)\n");
#endif
    return 0;
}

/*
 * 功能：4G接入认证：MME发起用户身份请求
 * 参数：	无
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_accessAuth_User_Identity_Request(int iSockfd, unsigned char *ip, unsigned int port)
{
    //根据id获取UE的ip与port

    //整理用户身份请求数据
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB stAuBodyFirstMMEToENB;
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    int iRet;
    printf("------------------------Start to Authentication------------------------\n");
    printf("开始构造身份请求数据.\n");
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x01;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stAuBodyFirstMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAuBodyFirstMMEToENB.aucIdNumber = 0x0;
    memcpy(stAuBodyFirstMMEToENB.aucDstId, ip, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAuBodyFirstMMEToENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAuBodyFirstMMEToENB.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB_LEN;
    stAuBodyFirstMMEToENB.aucDstPort = port;
    stAuBodyFirstMMEToENB.aucSrcPort = 11111;

    stAuBodyFirstMMEToENB.ucReqAuth = 0x01;  //暂时定义0x01来定义用户身份请求标识

    memcpy(stAuBodyFirstMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    //2、UDP发送用户身份请求
    memset(cSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(cSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(cSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodyFirstMMEToENB, D_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB_LEN);
    printf("身份请求数据构造完毕.\n");
    //此处将MME作为客户端，UE作为服务端来使用UDP建立连接
    /*
    if ((iSockfd = NSP_TDYTH_4G_ANET_ipv4_send_init()) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 socket", __LINE__, errno, strerror(errno));
        return -1;
    }
     */
    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, cSendBuf,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB_LEN,
                                            D_NSP_TDYTH_NAAAS_ENB1_IP, D_NSP_TDYTH_NAAAS_ENB1_PORT, 0)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("身份请求数据发送完毕,step:%x 完成.\n", stHeader.ucStep);
    }
    printf("------------------------step1: MME#Sender -> ENB:0x01------------------------\n");
    return 0;
}

/*
 * 功能：4G接入认证：MME发起用户身份请求(前端)
 * 参数：  无
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_FrontaccessAuth_User_Identity_Request(int iSockfd, unsigned char *ip, unsigned int port)
{
    //1、整理用户身份请求数据
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB stAuBodyFirstMMEToENB;
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    int iRet;
    printf("------------------------Start to Authentication------------------------\n");
    printf("开始构造身份请求数据.\n");
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x01;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stAuBodyFirstMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    printf("timestamp = %ld\n",stAuBodyFirstMMEToENB.TimeStamp);
    stAuBodyFirstMMEToENB.aucIdNumber = 0x0;
    memcpy(stAuBodyFirstMMEToENB.aucDstId, ip, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAuBodyFirstMMEToENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAuBodyFirstMMEToENB.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB_LEN;
    stAuBodyFirstMMEToENB.aucDstPort = port;
    stAuBodyFirstMMEToENB.aucSrcPort = 11111;

    stAuBodyFirstMMEToENB.ucReqAuth = 0x01;  //暂时定义0x01来定义用户身份请求标识

    memcpy(stAuBodyFirstMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    //2、UDP发送用户身份请求
    memset(cSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(cSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(cSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodyFirstMMEToENB, D_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB_LEN);
    printf("身份请求数据构造完毕.\n");

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, cSendBuf,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB_LEN,
                                            D_NSP_TDYTH_NAAAS_ENB1_IP, D_NSP_TDYTH_NAAAS_ENB1_PORT, 0)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("身份请求数据发送完毕,step:%x 完成.\n", stHeader.ucStep);
        printf_dae_mmesend("认证请求已发送!\n");
    }
    printf("------------------------step1: MME#Sender -> ENB:0x01------------------------\n");
    return 0;
}
/*
 * 功能：4G接入认证：进行用户身份响应，并发起认证数据请求
 * 参数：	@pcRecvBuf [in]	收到的数据
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_accessAuth_Auth_Data_Request(char *pcRecvBuf, int iSockfd)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS stAuBodyFifthMMEToHSS;
    int iRet;
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    unsigned char aucAubody_ue[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];           //解析从ue传来的认证请求数据
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN+1];

    memcpy(aucAubody_ue, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyFourthENBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyFourthENBToMME *stAuBodyFourthENBToMME = (St_NSP_TDYTH_NAAAS_4G_AuBodyFourthENBToMME*)aucAubody_ue;
    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyFourthENBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    printf("用户身份响应解析完成.\n");

    memcpy(aucIMSI,stAuBodyFourthENBToMME->aucIMSI,D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    printf("step2(UEtoMME):IMSI:%s\n", aucIMSI);
    printf_dae_mmesend("UE IMSI = %s\n",aucIMSI);

    //2、构造发送HSS的数据
    printf("开始构造认证向量请求数据\n");
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x01;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x05;

    stAuBodyFifthMMEToHSS.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAuBodyFifthMMEToHSS.aucIdNumber = 0x0;
    memcpy(stAuBodyFifthMMEToHSS.aucDstId, stAuBodyFourthENBToMME->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAuBodyFifthMMEToHSS.aucSrcId, stAuBodyFourthENBToMME->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAuBodyFifthMMEToHSS.aucDstPort = stAuBodyFourthENBToMME->aucDstPort;
    stAuBodyFifthMMEToHSS.aucSrcPort = stAuBodyFourthENBToMME->aucSrcPort;
    stAuBodyFifthMMEToHSS.iDataLen = 0;

    memcpy(stAuBodyFifthMMEToHSS.aucIMSI, stAuBodyFourthENBToMME->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(stAuBodyFifthMMEToHSS.aucGUTI, stAuBodyFourthENBToMME->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stAuBodyFifthMMEToHSS.aucSNID, "000", D_NSP_TDYTH_NAAAS_4G_SNID_LEN);
    stAuBodyFifthMMEToHSS.ucNetworkType = 0x0;

    memcpy(stAuBodyFifthMMEToHSS.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("认证向量请求数据构造完毕.\n");

    //MME端存储信息
    printf("开始储存终端信息.\n");
    memcpy(s_stMME.aucIMSI, stAuBodyFourthENBToMME->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    //memcpy(s_stMME.aucHSSID, stAuBodyFourthENBToMME->aucHSSID, D_NSP_TDYTH_NAAAS_4G_HSSID_LEN);
    memcpy(s_stMME.aucSNID, stAuBodyFifthMMEToHSS.aucSNID, D_NSP_TDYTH_NAAAS_4G_SNID_LEN);
    printf("MME储存终端(IMSI):%s 相关用户信息.\n",stAuBodyFourthENBToMME->aucIMSI);   //应当为“000000”结果输出为空

    //3、调用UDP接口向HSS发送消息
    //此处MME->HSS，将MME作为客户端，HSS作为服务端
    memset(cSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(cSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(cSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodyFifthMMEToHSS, D_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, cSendBuf,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS_LEN,
                                            D_NSP_TDYTH_NAAAS_HSS_IP, D_NSP_TDYTH_NAAAS_HSS_PORT, 0)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("身份请求数据发送完毕,step:%x 完成.\n", stHeader.ucStep);
    }
    printf("------------------------step5: MME#Sender -> HSS：0x05------------------------\n");
    return 0;
}

/*
 * 功能：4G接入认证：进行认证数据响应，并发起用户认证请求
 * 参数：	@pcRecvBuf [in]	收到的数据
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_accessAuth_User_Auth_Request(char *pcRecvBuf, int iSockfd)
{
    //St_NSP_TDYTH_NAAAS_4G_AuBodyFourthHSSToMME stAuBodyFourthHSSToMME;
    unsigned char aucAV[D_NSP_TDYTH_NAAAS_4G_AV_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];
    unsigned char aucKASME[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];
    unsigned char cAubody[D_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME_LEN];          // 构造接收函数体
    unsigned char ucKSIASME;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB stAuBodyNinethMMEToENB;
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    //int iSockfd;
    int iRet;
    /*
        //1、接收HSS->MME的认证数据响应
        //此处HSS->MME，HSS作为客户端，MME作为服务端
        struct sockaddr_in stSin4;

        bzero(&stSin4, sizeof(stSin4));
        stSin4.sin_family = AF_INET;
        stSin4.sin_port = htons(D_NSP_TDYTH_NAAAS_HSS_PORT);
        stSin4.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_HSS_IP);

        //从UE处接收消息，得到buf，进行处理
        if ((iSockfd = NSP_TDYTH_4G_ANET_ipv4_recv_init(D_NSP_TDYTH_NAAAS_MME1_PORT, &stSin4)) < 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 socket", __LINE__, errno, strerror(errno));
            return -1;
        }
        if ((iRet = NSP_TDYTH_4G_ANET_ipv4_recv(iSockfd, cRecvBuf, sizeof(cRecvBuf), (struct sockaddr *) &stSin4,
                                                sizeof(stSin4), 0)) < 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 recv", __LINE__, errno, strerror(errno));
            return -1;
        }
        memcpy(&stHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
        if (stHeader.ucStep != 0x04) {
            printf("%s,[line=%d],errno=%d:%s\n", "MME:recv err", __LINE__, errno, strerror(errno));
        }
    */

    memcpy(cAubody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME *stAuBodyEighthHSSToMME = (St_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME *)cAubody;
    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyEighthHSSToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(aucAV, stAuBodyEighthHSSToMME->aucAV, D_NSP_TDYTH_NAAAS_4G_AV_LEN);
    //MME端存储信息
    memcpy(s_stMME.aucAV, stAuBodyEighthHSSToMME->aucAV, D_NSP_TDYTH_NAAAS_4G_AV_LEN);
    printf("认证向量AVs存储完毕.\n");
    //(2)从AV中提取RAND、AUTN、KASME ；AV：RAND||XRES||KASME||AUTN
    memcpy(aucRAND, aucAV, D_NSP_TDYTH_NAAAS_4G_RAND_LEN);
    memcpy(aucKASME, aucAV + D_NSP_TDYTH_NAAAS_4G_RAND_LEN + D_NSP_TDYTH_NAAAS_4G_RES_LEN, sizeof(aucKASME));
    memcpy(aucAUTN, aucAV + D_NSP_TDYTH_NAAAS_4G_RAND_LEN + D_NSP_TDYTH_NAAAS_4G_RES_LEN + D_NSP_TDYTH_NAAAS_4G_KASME_LEN, sizeof(aucAUTN));
    printf("认证向量解析完毕,提取RAND,KASME等信息.\n");
    //(3)为KASME分配一个 3bits 的密钥标识KSIAMSE，此密钥标识由MME自行随机生成，生成算法待确定
    //暂定KSIASME="110"(二进制）
    ucKSIASME = 0x6;

    //3、构造MME->UE的用户认证请求数据内容 RAND||AUTN||KSIASME
    printf("构造MME->UE的用户认证请求数据内容.\n");
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x01;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x09;

    stAuBodyNinethMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAuBodyNinethMMEToENB.aucIdNumber = 0x0;
    memcpy(stAuBodyNinethMMEToENB.aucDstId, stAuBodyEighthHSSToMME->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAuBodyNinethMMEToENB.aucSrcId, stAuBodyEighthHSSToMME->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAuBodyNinethMMEToENB.aucDstPort = stAuBodyEighthHSSToMME->aucDstPort;
    stAuBodyNinethMMEToENB.aucSrcPort = stAuBodyEighthHSSToMME->aucSrcPort;
    stAuBodyNinethMMEToENB.iDataLen = 0;
    memcpy(stAuBodyNinethMMEToENB.aucAuthReq, aucRAND, D_NSP_TDYTH_NAAAS_4G_RAND_LEN);
    memcpy(stAuBodyNinethMMEToENB.aucAuthReq + D_NSP_TDYTH_NAAAS_4G_RAND_LEN, aucAUTN, D_NSP_TDYTH_NAAAS_4G_AUTN_LEN);
    memcpy(stAuBodyNinethMMEToENB.aucAuthReq + D_NSP_TDYTH_NAAAS_4G_RAND_LEN + D_NSP_TDYTH_NAAAS_4G_AUTN_LEN, &ucKSIASME,D_NSP_TDYTH_NAAAS_4G_KSIASME_LEN);
    memcpy(stAuBodyNinethMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    //4、调用UDP接口向UE发送消息
    //此处MME->UE，将MME作为客户端，UE作为服务端
    memset(cSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(cSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(cSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodyNinethMMEToENB, D_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB_LEN);
    printf("MME->UE的用户认证请求数据内容构造完毕.\n");
    //此处将MME作为客户端，UE作为服务端来使用UDP建立连接
    /*
    if ((iSockfd = NSP_TDYTH_4G_ANET_ipv4_send_init()) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 socket", __LINE__, errno, strerror(errno));
        return -1;
    }
    */

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, cSendBuf,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB_LEN,
                                            D_NSP_TDYTH_NAAAS_ENB1_IP, D_NSP_TDYTH_NAAAS_ENB1_PORT, 0)) < 0) 
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("身份请求数据发送完毕,step:%x 完成.\n", stHeader.ucStep);
    }
    printf("------------------------step9: MME#Sender -> ENB：0x09------------------------\n");
    return 0;
}

/*
 * 功能：4G接入认证：进行用户认证响应，并向UE反馈认证结果
 * 参数：	@pcRecvBuf [in]	收到的数据
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_accessAuth_AuthResult(char *pcRecvBuf, int iSockfd)
{
    unsigned char aucXRES[D_NSP_TDYTH_NAAAS_4G_RES_LEN];  //XRES从存储的AV中得到
    unsigned char aucAubody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    int iRet;
    int i;
    int RES_flag=1;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB stAuBodyThirteenthMMEToENB;

    memcpy(aucAubody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyTwelfthENBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyTwelfthENBToMME *stAuBodyTwelfthENBToMME = (St_NSP_TDYTH_NAAAS_4G_AuBodyTwelfthENBToMME *)aucAubody;
    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyTwelfthENBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    printf("数据包解析完毕.\n");

    //从MME端存储的AV中得到XRES
    memcpy(aucXRES, s_stMME.aucAV + D_NSP_TDYTH_NAAAS_4G_RAND_LEN, D_NSP_TDYTH_NAAAS_4G_RES_LEN);

    //2、将收到的RES和本地保存的XRES进行比较，如果相等，则认证通过
    for (i = 0; i < D_NSP_TDYTH_NAAAS_4G_RES_LEN; i++) {
        if (stAuBodyTwelfthENBToMME->aucRes[i] != aucXRES[i])
        {
            RES_flag=0;
        }
    }
    if(RES_flag==0)
        printf("%s,[line=%d]\n", "RES compare err, access auth err", __LINE__);
    else
        printf("RES校验成功,开始构造认证确认信息.\n");

    //认证完毕后，将MME的用户信息表的认证状态更新
#ifdef DATABASE
    MYSQL *mysql;
    mysql = NSP_TDYTH_4G_DB_connect();

    char acbuf[200];
    sprintf(acbuf,"update NSP_TDYTH_MME_USER_INFORMATION SET Certification='已认证' where UE_IP='%s' and UE_PORT=%d", stAuBodyTwelfthENBToMME->aucDstId, stAuBodyTwelfthENBToMME->aucDstPort);
    iRet = mysql_query(mysql,acbuf);
    if(iRet < 0)
    {
        printf("更新认证状态失败！\n");
    } else
    {
        printf("更新认证状态成功！\n");
    }
    NSP_TDYTH_4G_DB_close(mysql);
#endif
    //3、如果认证通过，由MME给UE发送个认证结果的反馈信息
    //此处MME->UE，将MME作为客户端，UE作为服务端
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x01;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x0d;

    stAuBodyThirteenthMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAuBodyThirteenthMMEToENB.aucIdNumber = 0x0;
    memcpy(stAuBodyThirteenthMMEToENB.aucDstId, stAuBodyTwelfthENBToMME->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAuBodyThirteenthMMEToENB.aucSrcId, stAuBodyTwelfthENBToMME->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAuBodyThirteenthMMEToENB.aucDstPort = stAuBodyTwelfthENBToMME->aucDstPort;
    stAuBodyThirteenthMMEToENB.aucSrcPort = stAuBodyTwelfthENBToMME->aucSrcPort;
    stAuBodyThirteenthMMEToENB.iDataLen = 0;

    memcpy(stAuBodyThirteenthMMEToENB.aucResult, "success", D_NSP_TDYTH_NAAAS_4G_AuResult_LEN);

    memcpy(stAuBodyThirteenthMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    memcpy(cSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(cSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodyThirteenthMMEToENB, D_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB_LEN);
    printf("认证确认信息构造完毕.\n");

    //此处将MME作为客户端，UE作为服务端来使用UDP建立连接
    /*
    if ((iSockfd = NSP_TDYTH_4G_ANET_ipv4_send_init()) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "UDP ipv4 socket", __LINE__, errno, strerror(errno));
        return -1;
    }
*/
    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, cSendBuf,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB_LEN,
                                            D_NSP_TDYTH_NAAAS_ENB1_IP, D_NSP_TDYTH_NAAAS_ENB1_PORT, 0)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("身份请求数据发送完毕,step:%x 完成.\n", stHeader.ucStep);
    }
    printf("------------------------step13: MME#Sender -> ENB：0x0d------------------------\n");
    printf_dae_mmesend("认证已完成，success!\n");

    return 0;
}
/*
 * 功能：MME请求用户信息
 * 参数：	无
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_UserInfo_Request(int iSockfdUE)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request UserInfoRequest;

    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    int iRet;
    printf("------------------------Start to UserInfo Request------------------------\n");

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x02;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x0a;   //暂定0x0a代表请求用户信息

    UserInfoRequest.TimeStamp = NSP_TDYTH_4G_get_time_now();
    UserInfoRequest.aucIdNumber = 0x0;
    memcpy(UserInfoRequest.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(UserInfoRequest.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    UserInfoRequest.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_LEN;
    UserInfoRequest.ucUserInfo = 0x01;  //代表请求用户信息
    memcpy(UserInfoRequest.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    //2、UDP发送用户身份请求
    memset(cSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(cSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(cSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &UserInfoRequest, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_LEN);
    /****
    int iSockfdUE = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    if ((iRet = sendto(iSockfdUE, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN, MSG_DONTWAIT, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("用户信息请求完毕,step:%x 完成.\n", stHeader.ucStep);
    }
    */
    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfdUE, cSendBuf,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_LEN,
                                            D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT, 0)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("身份请求数据发送完毕,step:%x 完成.\n", stHeader.ucStep);
    }
    printf("------------------------ MME#Sender -> UE----------------------\n");
    return 0;
}

/*
 * 功能：请求用户信息第1步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_UserInfo_Request_First_MMESendToHSS(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS stURBodyFirstMMEToHSS;

    int iRet;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x02;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    stURBodyFirstMMEToHSS.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stURBodyFirstMMEToHSS.aucIdNumber = 0x1;
    memcpy(stURBodyFirstMMEToHSS.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stURBodyFirstMMEToHSS.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stURBodyFirstMMEToHSS.iDataLen = 0;

    stURBodyFirstMMEToHSS.ucUserInfo = 0x01;

    memcpy(stURBodyFirstMMEToHSS.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memset(pcSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stURBodyFirstMMEToHSS, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS_LEN);


    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：解析用户信息并展示出来
 * 参数：	@xxx [pcRecvBuf in]	接收缓冲区
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_accessAuth_UserInfo(char *pcRecvBuf)
{

    char aucAubody_ue[D_NSP_TDYTH_NAAAS_4G_UserInfo_Response_LEN];           //解析从UE传来的用户信息


    memcpy(&aucAubody_ue, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_UserInfo_Response_LEN);
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Response *UserInfoResponse = (St_NSP_TDYTH_NAAAS_4G_UserInfo_Response *)aucAubody_ue;  //解析
    printf("-----------------------开始解析用户信息------------------------\n");

    printf("1.用户姓名:%s.\n", UserInfoResponse->aucUserName);
    printf_dae_mmesend("1.用户姓名:%s.\n", UserInfoResponse->aucUserName);
    printf("2.用户身份证号:%s.\n", UserInfoResponse->aucUserID);
    printf_dae_mmesend("2.用户身份证号:%s.\n", UserInfoResponse->aucUserID);
    printf("3.用户GPS位置信息:%s.\n", UserInfoResponse->aucUserGps);
    printf_dae_mmesend("3.用户GPS位置信息:%s.\n", UserInfoResponse->aucUserGps);
    if(UserInfoResponse->aucUserStatus == 0x00)
    {
        printf("4.用户认证状态: 未认证\n");
        printf_dae_mmesend("4.用户认证状态: 未认证\n");
    }
    else{
        printf("4.用户认证状态: 已认证\n");
        printf_dae_mmesend("4.用户认证状态: 未认证\n");
    }
    return 0;
}

/*
 * 功能：请求用户信息第3步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_UserInfo_Request_Third_MMESendToUE(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE stBodyThirdMMEToUE;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME *stURBodySecondHSSToMME = (St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stURBodySecondHSSToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x02;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x03;

    stBodyThirdMMEToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBodyThirdMMEToUE.aucIdNumber = 0x0;
    memcpy(stBodyThirdMMEToUE.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBodyThirdMMEToUE.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBodyThirdMMEToUE.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_LEN;

    stBodyThirdMMEToUE.ucUserInfo = 0x01;  //代表请求用户信息

    memcpy(stBodyThirdMMEToUE.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕\n");

    //2、UDP发送用户身份请求
    memset(pcSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBodyThirdMMEToUE, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE_LEN);

    return 0;

}

/*
 * 功能：请求用户信息第3步发送函数（给所有注册用户发送）
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_UserInfo_Request_ThirdSend_MMESendToUE(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;

    char acurbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];
    char aucRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char acIps[D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_IPArray_LEN];
    int aiPorts[D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_PortArray_LEN];

    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));

    int ue_sin_len = sizeof(UESockaddr);

    memcpy(acurbody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME *stBodySecondMMEToHSS = (St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME *)acurbody;
    printf("开始解析数据包，获取注册用户的IP与PORT\n");

    memcpy(acIps, stBodySecondMMEToHSS->aucIPs, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_IPArray_LEN);

    for (int i = 0; i < stBodySecondMMEToHSS->iCount; ++i) 
    {
        aiPorts[i] = stBodySecondMMEToHSS->aiPorts[i];
    }

    for (int k1 = 0; k1 < stBodySecondMMEToHSS->iCount; ++k1)
    {
        int iPort;
        char acIp[16];

        memcpy(acIp, acIps+(k1*16),16);
        iPort = aiPorts[k1];

        if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,
                                                D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_LEN,
                                                acIp, iPort, 0)) < 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
        } 
        else
        {
            printf("%d : MME#Sender -> UE (ip: %s port: %d)\n", k1+1, acIp, iPort);
        }
    }
    printf("------------------------step3: MME#Sender -> UE:0x03------------------------\n");

    return 0;
}

/***********************************************************************IP分配*********************************************************************/
/*
 * 功能：IP分配第一步
 * 参数：  @iSockfd [in] Socket套接字
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_IPAllot_First_MMESendToSGW(int iSockfd, unsigned char *ip, unsigned int port)
{
    int iRet;

    //1、整理请求会话数据
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW stIPAllotBodyFirstMMEToSGW;
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    printf("------------------------Start to IP Allot------------------------\n");
    printf("开始构造请求会话数据.\n");
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stIPAllotBodyFirstMMEToSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIPAllotBodyFirstMMEToSGW.aucIdNumber = 0x0;
    memcpy(stIPAllotBodyFirstMMEToSGW.aucDstId, ip, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIPAllotBodyFirstMMEToSGW.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIPAllotBodyFirstMMEToSGW.aucDstPort = port;
    stIPAllotBodyFirstMMEToSGW.aucSrcPort = 11111;
    stIPAllotBodyFirstMMEToSGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW_LEN;

    memcpy(stIPAllotBodyFirstMMEToSGW.aucIMSI, "000111000000000", D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    stIPAllotBodyFirstMMEToSGW.ucPDNType = 0x11;
    memcpy(stIPAllotBodyFirstMMEToSGW.aucPDNAddress, "222.222.222.222", D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN);
    stIPAllotBodyFirstMMEToSGW.ucPCO = 0x01;

    memcpy(stIPAllotBodyFirstMMEToSGW.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    //2、UDP发送用户身份请求
    memset(cSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(cSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(cSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIPAllotBodyFirstMMEToSGW, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW_LEN);
    printf("请求会话数据构造完毕.\n");

    //此处将MME作为客户端，UE作为服务端来使用UDP建立连接
    /*******************************************************************************************************
    if ((iSockfd = NSP_TDYTH_4G_ANET_ipv4_send_init()) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 socket", __LINE__, errno, strerror(errno));
        return -1;
    }
    *******************************************************************************************************/

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, cSendBuf,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW_LEN,
                                            D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_SGW1_PORT, 0)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }
    else
    {
        printf("请求会话数据发送完毕,step:%x 完成.\n", stHeader.ucStep);
    }
    printf("------------------------step1: MME#Sender -> SGW:0x01------------------------\n");

    return 0;
}

/*
 * 功能：IP分配第一步(前端)
 * 参数：  @iSockfd [in] Socket套接字
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_FrontIPAllot_First_MMESendToSGW(int iSockfd, unsigned char *ip, unsigned int port)
{
    int iRet;

    //1、整理请求会话数据
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW stIPAllotBodyFirstMMEToSGW;
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    printf("------------------------Start to IP Allot------------------------\n");
    printf("开始构造请求会话数据.\n");
    printf_dae_mmesend("开始构造请求会话数据.\n");
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stIPAllotBodyFirstMMEToSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIPAllotBodyFirstMMEToSGW.aucIdNumber = 0x0;
    memcpy(stIPAllotBodyFirstMMEToSGW.aucDstId, ip, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIPAllotBodyFirstMMEToSGW.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIPAllotBodyFirstMMEToSGW.aucDstPort = port;
    stIPAllotBodyFirstMMEToSGW.aucSrcPort = 11111;
    stIPAllotBodyFirstMMEToSGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW_LEN;

    memcpy(stIPAllotBodyFirstMMEToSGW.aucIMSI, "000111000000000", D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    stIPAllotBodyFirstMMEToSGW.ucPDNType = 0x11;
    memcpy(stIPAllotBodyFirstMMEToSGW.aucPDNAddress, "222.222.222.222", D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN);
    stIPAllotBodyFirstMMEToSGW.ucPCO = 0x01;

    memcpy(stIPAllotBodyFirstMMEToSGW.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    //2、UDP发送用户身份请求
    memset(cSendBuf, '\0', D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN);
    memcpy(cSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(cSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIPAllotBodyFirstMMEToSGW, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW_LEN);
    printf("请求会话数据构造完毕.\n");
    printf_dae_mmesend("请求会话数据构造完毕.\n");

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, cSendBuf,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW_LEN,
                                            D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_SGW1_PORT, 0)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }
    else
    {
        printf("请求会话数据发送完毕,step:%x 完成.\n", stHeader.ucStep);
        printf_dae_mmesend("IP分配请求已发送!\n");
    }
    printf("------------------------step1: MME#Sender -> SGW:0x01------------------------\n");

    return 0;
}
/*
 * 功能：IP分配第五步MME发给ENB
 * 参数：  
 *        @pcRecvBuf [in]    接受缓冲区
 *        @pcSendBuf [out]   发送缓冲区
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_IPAllot_Fifth_MMESendToENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    unsigned char aucIPAllotBodyFourthSGWToMME[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFifthMMEToENB stIPAllotBodyFifthMMEToENB;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x05;

    memcpy(aucIPAllotBodyFourthSGWToMME, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME *stIPAllotBodyFourthSGWToMME = (St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME *)aucIPAllotBodyFourthSGWToMME;

    if(NSP_TDYTH_4G_verification_timestamp(stIPAllotBodyFourthSGWToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    stIPAllotBodyFifthMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIPAllotBodyFifthMMEToENB.aucIdNumber = 0x0;
    memcpy(stIPAllotBodyFifthMMEToENB.aucDstId, stIPAllotBodyFourthSGWToMME->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIPAllotBodyFifthMMEToENB.aucSrcId, stIPAllotBodyFourthSGWToMME->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIPAllotBodyFifthMMEToENB.aucDstPort = stIPAllotBodyFourthSGWToMME->aucDstPort;
    stIPAllotBodyFifthMMEToENB.aucSrcPort = stIPAllotBodyFourthSGWToMME->aucSrcPort;
    stIPAllotBodyFifthMMEToENB.iDataLen = 0;

    //填充结构体信息
    //todo
    memcpy(stIPAllotBodyFifthMMEToENB.aucGUTI, "0000000000", D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stIPAllotBodyFifthMMEToENB.pucKenb, "00000000000000000000000000000000", D_NSP_TDYTH_NAAAS_4G_KEY_LEN);
    memcpy(stIPAllotBodyFifthMMEToENB.pucNH, "00000000000000000000000000000000", D_NSP_TDYTH_NAAAS_4G_NH_LEN);
    stIPAllotBodyFifthMMEToENB.pucNCC='0';
    stIPAllotBodyFifthMMEToENB.ucPDNType = stIPAllotBodyFourthSGWToMME->ucPDNType;
    memcpy(stIPAllotBodyFifthMMEToENB.aucPDNAddress, stIPAllotBodyFourthSGWToMME->aucPDNAddress, D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN);
    stIPAllotBodyFifthMMEToENB.ucPCO = stIPAllotBodyFourthSGWToMME->ucPCO;

    memcpy(stIPAllotBodyFifthMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("请求会话创建数据构造完毕.\n");
    printf_dae_mmesend("设备UE IP: %s\n",stIPAllotBodyFifthMMEToENB.aucPDNAddress);
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIPAllotBodyFifthMMEToENB, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFifthMMEToENB_LEN); //数据体填入发送缓冲区

    return  0;
}

/***********************************************************************X2切换***********************************************************************/
/*
 * 功能：X2切换第8步
 * 参数：   
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：    
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_X2_HandOver_Eighth_MMESendToTENB(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_HandOverBodySeventhTENBToMME_LEN);

    St_NSP_TDYTH_NAAAS_4G_N2_BodySeventhTENBToMME *stHOBodySeventhTENBToMME = (St_NSP_TDYTH_NAAAS_4G_N2_BodySeventhTENBToMME*)auchobody;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodyEighthMMEToTENB stHOBodyEighthMMEToTENB;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x08;

    stHOBodyEighthMMEToTENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stHOBodyEighthMMEToTENB.aucIdNumber = 0x1;
    memcpy(stHOBodyEighthMMEToTENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stHOBodyEighthMMEToTENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stHOBodyEighthMMEToTENB.iDataLen = 0;
//todo 生成NHNCC*
    stHOBodyEighthMMEToTENB.uiEnb_Ue_S1Id = stHOBodySeventhTENBToMME->uiEnb_Ue_S1Id;
    stHOBodyEighthMMEToTENB.ucCellId = 1;

    memcpy(stHOBodyEighthMMEToTENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stHOBodyEighthMMEToTENB, D_NSP_TDYTH_NAAAS_4G_HandOverBodyEighthMMEToTENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}


/***********************************************************************S1切换***********************************************************************/
/*
 * 功能：S1切换第3步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Third_MMESendToTENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    unsigned char aucKenb1[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucNH[D_NSP_TDYTH_NAAAS_4G_NH_LEN];
    unsigned char aucKenb2[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    //todo 根据收到的GUTI获取KASME
    unsigned char aucKasme[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];
    memcpy(aucKasme, "00000000000", D_NSP_TDYTH_NAAAS_4G_KASME_LEN);

    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME *stS1HOBodySecondSENBToMME = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME*)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stS1HOBodySecondSENBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    memcpy(aucKenb1, stS1HOBodySecondSENBToMME->aucKenb1, D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
//todo
//根据收到的GUTI查询eNB2的地址信息



    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB stS1HOBodyThirdMMEToTENB;

    int i = 0;
    printf("开始生成密钥！\n");
    if ((iRet = NSP_TDYTH_4G_gen_Kenb2(aucKasme, aucKenb1, aucKenb2)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb2 err", __LINE__, errno, strerror(errno));
        return -1;
    }
    NSP_TDYTH_4G_print("Kenb*+:\n",aucKenb2,D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    if ((iRet = NSP_TDYTH_4G_gen_S1_NH2(aucKasme, aucKenb2, keypair[i].NH)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_NH2 err", __LINE__, errno, strerror(errno));
        return -1;
    }

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x03;

    stS1HOBodyThirdMMEToTENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyThirdMMEToTENB.aucIdNumber = 0x1;
    memcpy(stS1HOBodyThirdMMEToTENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyThirdMMEToTENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyThirdMMEToTENB.iDataLen = 0;

    memcpy(stS1HOBodyThirdMMEToTENB.aucKenb2, aucKenb2, D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    memcpy(stS1HOBodyThirdMMEToTENB.aucNH, keypair[i].NH, D_NSP_TDYTH_NAAAS_4G_NH_LEN);
    stS1HOBodyThirdMMEToTENB.ucNCC = i + 1;
    stS1HOBodyThirdMMEToTENB.uiMme_Ue_S1Id = stS1HOBodySecondSENBToMME->uiMme_Ue_S1Id;
    
    memcpy(stS1HOBodyThirdMMEToTENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyThirdMMEToTENB, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：S1切换第五步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Fifth_MMESendToSENB(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME *stS1HOBodyFourthTENBToMME = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stS1HOBodyFourthTENBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB stS1HOBodyFifthMMEToSENB;

    //todo 应该根据之前的会话ID 找到对应的源ENB ，再进行转发。

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x05;

    stS1HOBodyFifthMMEToSENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyFifthMMEToSENB.aucIdNumber = 0x1;
    memcpy(stS1HOBodyFifthMMEToSENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyFifthMMEToSENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyFifthMMEToSENB.iDataLen = 0;

    stS1HOBodyFifthMMEToSENB.ucNCC = 1;

    memcpy(stS1HOBodyFifthMMEToSENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyFifthMMEToSENB, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：S1切换第8步
 * 参数：   
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：    
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Eighth_MMESendToTENB(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEighthMMEToTENB stS1HOBodyEighthTENBToMME;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x08;

    stS1HOBodyEighthTENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyEighthTENBToMME.aucIdNumber = 0x1;
    memcpy(stS1HOBodyEighthTENBToMME.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyEighthTENBToMME.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyEighthTENBToMME.iDataLen = 0;

    stS1HOBodyEighthTENBToMME.aucPDCP_SN = '1';;

    memcpy(stS1HOBodyEighthTENBToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyEighthTENBToMME, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEighthMMEToTENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：S1切换第11步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Eleventh_MMESendToSENB(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME *stS1HOBodyTenthTENBToMME = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stS1HOBodyTenthTENBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME stS1HOBodyEleventhTENBToMME;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x0b;

    stS1HOBodyEleventhTENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyEleventhTENBToMME.aucIdNumber = 0x1;
    memcpy(stS1HOBodyEleventhTENBToMME.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyEleventhTENBToMME.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyEleventhTENBToMME.iDataLen = 0;

    stS1HOBodyEleventhTENBToMME.uiMME_UE_S1Id = stS1HOBodyTenthTENBToMME->uiMME_UE_S1Id;
    stS1HOBodyEleventhTENBToMME.uieNB_UE_S1Id = stS1HOBodyTenthTENBToMME->uieNB_UE_S1Id;

    memcpy(stS1HOBodyEleventhTENBToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyEleventhTENBToMME, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEleventhMMEToSENB_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/***********************************************************************MME连接中断***********************************************************************/
/*
 * 功能：连接中断第一步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_LinkageInterrupt_First_MMEtoENB(char *pcSendBuf, unsigned char *ip, unsigned int port)
{
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB stLiBodyFirstMMEToENB;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x08;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stLiBodyFirstMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stLiBodyFirstMMEToENB.aucIdNumber = 0x1;
    memcpy(stLiBodyFirstMMEToENB.aucDstId, ip, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stLiBodyFirstMMEToENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stLiBodyFirstMMEToENB.aucDstPort = port;
    stLiBodyFirstMMEToENB.aucSrcPort = 11111;
    stLiBodyFirstMMEToENB.iDataLen = 0;

    memcpy(stLiBodyFirstMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("中断数据构造完毕.\n");

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stLiBodyFirstMMEToENB, D_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB_LEN);

    return 0;
}

/*
 * 功能：连接中断第一步(前端)
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_FrontLinkageInterrupt_First_MMEtoENB(char *pcSendBuf, unsigned char *ip, unsigned int port)
{
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB stReBodyFirstMMEToENB;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x09;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stReBodyFirstMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stReBodyFirstMMEToENB.aucIdNumber = 0x1;
    memcpy(stReBodyFirstMMEToENB.aucDstId, ip, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stReBodyFirstMMEToENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stReBodyFirstMMEToENB.aucDstPort = port;
    stReBodyFirstMMEToENB.aucSrcPort = 11111;
    stReBodyFirstMMEToENB.iDataLen = 0;

    memcpy(stReBodyFirstMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("中断恢复数据构造完毕.\n");

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stReBodyFirstMMEToENB, D_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB_LEN);

    return 0;
}
/***********************************************************************MME恢复连接***********************************************************************/
/*
 * 功能：连接恢复第一步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_LinkageRecover_First_MMEtoENB(char *pcSendBuf, unsigned char *ip, unsigned int port)
{
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB stReBodyFirstMMEToENB;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x09;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stReBodyFirstMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stReBodyFirstMMEToENB.aucIdNumber = 0x1;
    memcpy(stReBodyFirstMMEToENB.aucDstId, ip, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stReBodyFirstMMEToENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stReBodyFirstMMEToENB.aucDstPort = port;
    stReBodyFirstMMEToENB.aucSrcPort = 11111;
    stReBodyFirstMMEToENB.iDataLen = 0;

    memcpy(stReBodyFirstMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("中断恢复数据构造完毕.\n");

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stReBodyFirstMMEToENB, D_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB_LEN);

    return 0;
}

/*
 * 功能：连接恢复第一步(前端)
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_FrontLinkageRecover_First_MMEtoENB(char *pcSendBuf, unsigned char *ip, unsigned int port)
{
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB stReBodyFirstMMEToENB;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x09;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stReBodyFirstMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stReBodyFirstMMEToENB.aucIdNumber = 0x1;
    memcpy(stReBodyFirstMMEToENB.aucDstId, ip, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stReBodyFirstMMEToENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stReBodyFirstMMEToENB.aucDstPort = port;
    stReBodyFirstMMEToENB.aucSrcPort = 11111;
    stReBodyFirstMMEToENB.iDataLen = 0;

    memcpy(stReBodyFirstMMEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("中断恢复数据构造完毕.\n");

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stReBodyFirstMMEToENB, D_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB_LEN);

    return 0;
}

static int  gs_imme_uiS1_TEID;
static char gs_cmme_ucBearerID;
/*
 * 功能：专有承载建立第四步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Fourth_MMEToENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME));

    St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME *stBearerSetUpThirdSGWToMME = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpThirdSGWToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB stBearerSetUpFourthMMEToENB;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x04;

    stBearerSetUpFourthMMEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpFourthMMEToENB.aucIdNumber = 0x0;
    memcpy(stBearerSetUpFourthMMEToENB.aucDstId, stBearerSetUpThirdSGWToMME->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBearerSetUpFourthMMEToENB.aucSrcId, stBearerSetUpThirdSGWToMME->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpFourthMMEToENB.aucSrcPort = stBearerSetUpThirdSGWToMME->aucSrcPort;
    stBearerSetUpFourthMMEToENB.aucDstPort = stBearerSetUpThirdSGWToMME->aucDstPort;
    stBearerSetUpFourthMMEToENB.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB);

    stBearerSetUpFourthMMEToENB.ucPTI = stBearerSetUpThirdSGWToMME->ucPTI;
    memcpy(stBearerSetUpFourthMMEToENB.aucQoS, stBearerSetUpThirdSGWToMME->aucQoS, D_NSP_TDYTH_NAAAS_4G_QoS_LEN);
    memcpy(stBearerSetUpFourthMMEToENB.aucTFT, stBearerSetUpThirdSGWToMME->aucTFT, D_NSP_TDYTH_NAAAS_4G_TFT_LEN);
    stBearerSetUpFourthMMEToENB.uiS1_TEID = stBearerSetUpThirdSGWToMME->uiS1_TEID;
    stBearerSetUpFourthMMEToENB.ucLBI = stBearerSetUpThirdSGWToMME->ucLBI;
    stBearerSetUpFourthMMEToENB.ucBearerID = 0x0;//EPS承载标识???

    gs_imme_uiS1_TEID = stBearerSetUpThirdSGWToMME->uiS1_TEID;
    gs_cmme_ucBearerID = 0x0;

    memcpy(stBearerSetUpFourthMMEToENB.aucTerminalId, stBearerSetUpThirdSGWToMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpFourthMMEToENB, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：专有承载建立第十步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Tenth_MMEToSGW(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME));

    St_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME *stBearerSetUpNinthENBToMME = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpNinthENBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW stBearerSetUpTenthMMEToSGW;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0a;

    stBearerSetUpTenthMMEToSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpTenthMMEToSGW.aucIdNumber = 0x0;
    memcpy(stBearerSetUpTenthMMEToSGW.aucDstId, stBearerSetUpNinthENBToMME->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBearerSetUpTenthMMEToSGW.aucSrcId, stBearerSetUpNinthENBToMME->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpTenthMMEToSGW.aucSrcPort = stBearerSetUpNinthENBToMME->aucSrcPort;
    stBearerSetUpTenthMMEToSGW.aucDstPort = stBearerSetUpNinthENBToMME->aucDstPort;
    stBearerSetUpTenthMMEToSGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME);

    stBearerSetUpTenthMMEToSGW.uiS1_TEID = gs_imme_uiS1_TEID ;   //全局变量
    stBearerSetUpTenthMMEToSGW.ucBearerID = gs_cmme_ucBearerID ;//因为第九步只是一个响应信息

    memcpy(stBearerSetUpTenthMMEToSGW.aucTerminalId, stBearerSetUpNinthENBToMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpTenthMMEToSGW, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  请求用户信息完整流程
 * 参数： 
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_UserInfo_Function()
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    int iRet;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));       

    // MME
    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in HSSSockaddr;
    bzero(&HSSSockaddr, sizeof(HSSSockaddr));
    HSSSockaddr.sin_family = AF_INET;
    HSSSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_HSS_PORT);
    HSSSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_HSS_IP);
    int hss_sin_len = sizeof(HSSSockaddr);

 /***************************************请求用户信息流程，属于以前放在while1循环中的功能******************************************/

    NSP_TDYTH_4G_UserInfo_Request_First_MMESendToHSS(cRecvBuf, cSendBuf);

    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &HSSSockaddr, hss_sin_len);
    if(iRet > 0)
    {
        printf("------------------------step1: MME#Sender -> HSS：0x01------------------------\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
    return 0;
}

/*
 * 功能：发起接入认证
 * 参数：  
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_AccessAuth_Function()
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    int iRet;
    unsigned int uiMme_Ue_Id;
    unsigned char aucIP[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned int uiPort;
    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    // MME
    int iSockfdMME = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    /****************************************认证流程，属于以前放在while1循环中的功能******************************************/
#ifdef DATABASE
    printf("请输入需要认证的UE id！\n");
    scanf("%d",&uiMme_Ue_Id);

    //判断id是否在线
    char acUEId[10];
    sprintf(acUEId, "%d", uiMme_Ue_Id);

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery0 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery0->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery0->fieldKey = "Connection";
    stquery0->limitName = "MME_UEid";
    stquery0->limitValue = acUEId;
    char acbuf0[20];
    char *online = "离线";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery0, acbuf0);
    if (strcmp(acbuf0, online) == 0)
    {
        printf("UE已离线!\n");
        return -1;
    }

    //判断id是否连接
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery1 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery1->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery1->fieldKey = "Connection";
    stquery1->limitName = "MME_UEid";
    stquery1->limitValue = acUEId;
    char acbuf1[20];
    char *connection = "未连接";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery1, acbuf1);
    if (strcmp(acbuf1, connection) == 0)
    {
        printf("UE已断开连接!\n");
        return -1;
    }

    //根据id查ip和port
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery2 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery2->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery2->fieldKey = "UE_IP";
    stquery2->limitName = "MME_UEid";
    stquery2->limitValue = acUEId;
    unsigned char aucip[20];

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery3 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery3->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery3->fieldKey = "UE_PORT";
    stquery3->limitName = "MME_UEid";
    stquery3->limitValue = acUEId;
    unsigned char aucbuf3[10];

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery2, aucip);
    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery3, aucbuf3);
    unsigned int uiport = atoi(aucbuf3);

    NSP_TDYTH_4G_DB_close(mysql);

    NSP_TDYTH_4G_accessAuth_User_Identity_Request(iSockfdMME, aucip, uiport);
#else
    NSP_TDYTH_4G_accessAuth_User_Identity_Request(iSockfdMME, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT);
#endif
    printf_dae_mmesend("认证请求已发送!\n");
    close(iSockfdMME);
    return 0;
}
/*
 * 功能：发起接入认证(前端)
 * 参数：  
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_FrontAccessAuth_Function(char *pcRecvBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    int iRet;
    unsigned int uiMme_Ue_Id;
    unsigned char aucIP[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned int uiPort;

    memcpy(&uiMme_Ue_Id, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_DaemonControl_LEN,sizeof(int));

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    // MME
    int iSockfdMME = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    /****************************************认证流程，属于以前放在while1循环中的功能******************************************/
#ifdef DATABASE
    char acUEId[10];
    sprintf(acUEId, "%d", uiMme_Ue_Id);

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery0 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery0->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery0->fieldKey = "Connection";
    stquery0->limitName = "MME_UEid";
    stquery0->limitValue = acUEId;
    char acbuf0[20];
    char *online = "离线";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery0, acbuf0);
    if (strcmp(acbuf0, online) == 0)
    {
        printf("UE已离线!\n");
        printf_dae_mmesend("UE已离线!\n");
        return -1;
    }

    //判断id是否连接
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery1 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery1->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery1->fieldKey = "Connection";
    stquery1->limitName = "MME_UEid";
    stquery1->limitValue = acUEId;
    char acbuf1[20];
    char *connection = "未连接";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery1, acbuf1);
    if (strcmp(acbuf1, connection) == 0)
    {
        printf("UE已断开连接!\n");
        printf_dae_mmesend("UE已断开连接!\n");
        return -1;
    }

    //根据id查ip和port
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery2 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery2->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery2->fieldKey = "UE_IP";
    stquery2->limitName = "MME_UEid";
    stquery2->limitValue = acUEId;
    unsigned char aucip[20];

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery3 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery3->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery3->fieldKey = "UE_PORT";
    stquery3->limitName = "MME_UEid";
    stquery3->limitValue = acUEId;
    unsigned char aucbuf3[10];


    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery2, aucIP);
    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery3, aucbuf3);
    uiPort = atoi(aucbuf3);


    NSP_TDYTH_4G_DB_close(mysql);

    NSP_TDYTH_4G_FrontaccessAuth_User_Identity_Request(iSockfdMME, aucIP, uiPort);
#else
    NSP_TDYTH_4G_FrontaccessAuth_User_Identity_Request(iSockfdMME, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT);
#endif
    close(iSockfdMME);
    return 0;
}

/***********************************************************************附着***********************************************************************/
/*
 * 功能：接受附着请求第四步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_Attach_Fourth_eNB_Send(char *pcRecvBuf, char *pcSendBuf)
{
    char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME *stAttBodyFourtheNBToMME = (St_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAttBodyFourtheNBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    if(gs_imme_enb_id == 4000)
    {
        gs_imme_enb_id = 3000;
    }

    if(gs_imme_ue_id == 5000)
    {
        gs_imme_ue_id = 4000;
    }

    NSP_TDYTH_4G_attach_insert(pcRecvBuf);

    printf("----------------------附着完成！---------------------\n");
    NSP_TDYTH_4G_AccessAuth_Function();
    return 0;
}

/*
 * 功能：发起IP分配
 * 参数： 
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_IPAllot_Function()
{
    int iRet;
    unsigned int uiMme_Ue_Id;
   // unsigned char aucIP[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned int uiPort;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    // MME
    int iSockfdMME = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);
    int iMMEAddrLen = sizeof(MMESockaddr);

    /****************************************IP分配流程，属于以前放在while1循环中的功能******************************************/
#ifdef DATABASE
    printf("请输入需要分配IP的UE id！\n");


    scanf("%d",&uiMme_Ue_Id);

    //判断id是否在线
    char acUEId[10];
    sprintf(acUEId, "%d", uiMme_Ue_Id);

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery0 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery0->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery0->fieldKey = "Connection";
    stquery0->limitName = "MME_UEid";
    stquery0->limitValue = acUEId;
    char acbuf0[20];
    char *online = "离线";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery0, acbuf0);
    if (strcmp(acbuf0, online) == 0)
    {
        printf("UE已离线!\n");

        return -1;
    }

    //判断id是否连接
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery1 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery1->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery1->fieldKey = "Connection";
    stquery1->limitName = "MME_UEid";
    stquery1->limitValue = acUEId;
    char acbuf1[20];
    char *connection = "未连接";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery1, acbuf1);
    if (strcmp(acbuf1, connection) == 0)
    {
        printf("UE已断开连接!\n");

        return -1;
    }

    //根据id查ip和port
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery2 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery2->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery2->fieldKey = "UE_IP";
    stquery2->limitName = "MME_UEid";
    stquery2->limitValue = acUEId;
    unsigned char aucip[20];

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery3 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery3->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery3->fieldKey = "UE_PORT";
    stquery3->limitName = "MME_UEid";
    stquery3->limitValue = acUEId;
    unsigned char aucbuf3[4];

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery2, aucip);
    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery3, aucbuf3);
    unsigned int uiport = atoi(aucbuf3);

    NSP_TDYTH_4G_DB_close(mysql);

    NSP_TDYTH_4G_IPAllot_First_MMESendToSGW(iSockfdMME, aucip, uiport);
#else
    NSP_TDYTH_4G_IPAllot_First_MMESendToSGW(iSockfdMME, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT);
#endif
    close(iSockfdMME);

    return 0;
}
/*
 * 功能：发起IP分配(前端)
 * 参数： 
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_FrontIPAllot_Function(char *pcRecvBuf)
{
    int iRet;
    unsigned int uiMme_Ue_Id;
    unsigned char aucIP[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned int uiPort;

    memcpy(&uiMme_Ue_Id, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_DaemonControl_LEN,sizeof(int));
    printf("front id:%d\n",uiMme_Ue_Id);
    /* ------------------------------------*/
 //   char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

//    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    // MME
    int iSockfdMME = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);
    int iMMEAddrLen = sizeof(MMESockaddr);

    /****************************************IP分配流程，属于以前放在while1循环中的功能******************************************/
#ifdef DATABASE
    //判断id是否在线
    char acUEId[10];
    sprintf(acUEId, "%d", uiMme_Ue_Id);

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery0 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery0->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery0->fieldKey = "Connection";
    stquery0->limitName = "MME_UEid";
    stquery0->limitValue = acUEId;
    char acbuf0[20];
    char *online = "离线";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery0, acbuf0);
    if (strcmp(acbuf0, online) == 0)
    {
        printf("UE已离线!\n");
        printf_dae_mmesend("UE已离线!\n");
        return -1;
    }

    //判断id是否连接
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery1 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery1->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery1->fieldKey = "Connection";
    stquery1->limitName = "MME_UEid";
    stquery1->limitValue = acUEId;
    char acbuf1[20];
    char *connection = "未连接";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery1, acbuf1);
    if (strcmp(acbuf1, connection) == 0)
    {
        printf("UE已断开连接!\n");
        printf_dae_mmesend("UE已断开连接!\n");
        return -1;
    }

    //根据id查ip和port
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery2 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery2->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery2->fieldKey = "UE_IP";
    stquery2->limitName = "MME_UEid";
    stquery2->limitValue = acUEId;
    unsigned char aucip[20];

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery3 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery3->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery3->fieldKey = "UE_PORT";
    stquery3->limitName = "MME_UEid";
    stquery3->limitValue = acUEId;
    unsigned char aucbuf3[4];

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery2, aucip);
    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery3, aucbuf3);
    unsigned int uiport = atoi(aucbuf3);


    NSP_TDYTH_4G_DB_close(mysql);

    NSP_TDYTH_4G_FrontIPAllot_First_MMESendToSGW(iSockfdMME, aucip, uiport);
#else
    NSP_TDYTH_4G_FrontIPAllot_First_MMESendToSGW(iSockfdMME, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT);
#endif
    close(iSockfdMME);

    return 0;
}
/*
 * 功能：MME发起连接中断
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_LinkageInterrupt_Function()
{
    int iRet;
    unsigned int uiMme_Ue_Id;
    unsigned char aucIP[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned int uiPort;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    // MME
    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB2_IP);
    int isin_len = sizeof(ENBSockaddr);

    /*************************************************************************************************************************/
#ifdef DATABASE
    printf("请输入需要中断连接的UE id！\n");

    scanf("%d",&uiMme_Ue_Id);

    //判断id是否在线
    char acUEId[10];
    sprintf(acUEId, "%d", uiMme_Ue_Id);

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery0 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery0->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery0->fieldKey = "Connection";
    stquery0->limitName = "MME_UEid";
    stquery0->limitValue = acUEId;
    char acbuf0[20];
    char *online = "离线";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery0, acbuf0);
    if (strcmp(acbuf0, online) == 0)
    {
        printf("UE已离线!\n");
        return -1;
    }

    //判断id是否连接
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery1 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery1->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery1->fieldKey = "Connection";
    stquery1->limitName = "MME_UEid";
    stquery1->limitValue = acUEId;
    char acbuf1[20];
    char *connection = "未连接";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery1, acbuf1);
    if (strcmp(acbuf1, connection) == 0)
    {
        printf("UE已断开连接!\n");
        return -1;
    }

    //根据id查ip和port
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery2 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery2->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery2->fieldKey = "UE_IP";
    stquery2->limitName = "MME_UEid";
    stquery2->limitValue = acUEId;
    unsigned char aucip[20];

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery3 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery3->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery3->fieldKey = "UE_PORT";
    stquery3->limitName = "MME_UEid";
    stquery3->limitValue = acUEId;
    unsigned char aucbuf3[4];

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery2, aucip);
    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery3, aucbuf3);
    unsigned int uiport = atoi(aucbuf3);

    NSP_TDYTH_4G_LinkageInterrupt_First_MMEtoENB(cSendBuf, aucip, uiport);
#else
    NSP_TDYTH_4G_LinkageInterrupt_First_MMEtoENB(cSendBuf, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT);
#endif
    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENBSockaddr, isin_len);
    if(iRet > 0)
    {
        printf("------------------------step1: MME#Sender -> ENB：0x01------------------------\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);

#ifdef DATABASE
    //更新连接状态
    St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *update;
    update = (St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo));

    update->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    update->fieldKey = "Connection";
    update->fieldValue = "未连接";
    update->limitName = "MME_UEid";
    update->limitValue = acUEId;
    iRet = NSP_TDYTH_4G_DB_update(mysql, update);
    if(iRet < 0)
    {
        printf("更新连接状态失败！\n");
        return -1;
    }

    NSP_TDYTH_4G_DB_close(mysql);
#endif

    return 0;
};
/*
 * 功能：MME发起连接中断(前端)
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_FrontLinkageInterrupt_Function(char *pcRecvBuf)
{
    int iRet;
    unsigned int uiMme_Ue_Id;
    unsigned char aucIP[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned int uiPort;

    memcpy(&uiMme_Ue_Id, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_DaemonControl_LEN,sizeof(int));

    /* ------------------------------------*/
    unsigned char cSendBuf[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    memset(cSendBuf, 0, sizeof(cSendBuf));

    // MME
    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB2_IP);
    int isin_len = sizeof(ENBSockaddr);
    /*************************************************************************************************************************/
#ifdef DATABASE
    //判断id是否在线
    char acUEId[10];
    sprintf(acUEId, "%d", uiMme_Ue_Id);

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery0 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery0->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery0->fieldKey = "Connection";
    stquery0->limitName = "MME_UEid";
    stquery0->limitValue = acUEId;
    char acbuf0[20];
    char *online = "离线";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery0, acbuf0);
    if (strcmp(acbuf0, online) == 0)
    {
        printf("UE已离线!\n");
        return -1;
    }

    //判断id是否连接
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery1 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery1->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery1->fieldKey = "Connection";
    stquery1->limitName = "MME_UEid";
    stquery1->limitValue = acUEId;
    char acbuf1[20];
    char *connection = "未连接";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery1, acbuf1);
    if (strcmp(acbuf1, connection) == 0)
    {
        printf("UE已断开连接!\n");
        return -1;
    }

    //根据id查ip和port
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery2 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery2->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery2->fieldKey = "UE_IP";
    stquery2->limitName = "MME_UEid";
    stquery2->limitValue = acUEId;
    unsigned char aucip[20];

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery3 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery3->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery3->fieldKey = "UE_PORT";
    stquery3->limitName = "MME_UEid";
    stquery3->limitValue = acUEId;
    unsigned char aucbuf3[4];

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery2, aucip);
    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery3, aucbuf3);
    unsigned int uiport = atoi(aucbuf3);


    NSP_TDYTH_4G_FrontLinkageInterrupt_First_MMEtoENB(cSendBuf, aucIP, uiPort);
#else
    NSP_TDYTH_4G_FrontLinkageInterrupt_First_MMEtoENB(cSendBuf, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT);
#endif
    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENBSockaddr, isin_len);
    if(iRet > 0)
    {
        printf("------------------------step1: MME#Sender -> ENB：0x01------------------------\n");
        printf_dae_mmesend("中断连接请求已发送!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
#ifdef DATABASE
    //更新连接状态
    St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *update;
    update = (St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo));

    update->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    update->fieldKey = "Connection";
    update->fieldValue = "未连接";
    update->limitName = "MME_UEid";
    update->limitValue = acUEId;
    iRet = NSP_TDYTH_4G_DB_update(mysql, update);
    if(iRet < 0)
    {
        printf("更新连接状态失败！\n");
        return -1;
    }


    NSP_TDYTH_4G_DB_close(mysql);
#endif
    return 0;
};
/*
 * 功能：MME发起连接恢复
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_LinkageRecover_Function()
{
    int iRet;
    unsigned int uiMme_Ue_Id;
    unsigned char aucIP[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned int uiPort;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    // ENB
    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB2_IP);
    int isin_len = sizeof(ENBSockaddr);
    /*************************************************************************************************************************/
#ifdef DATABASE
    printf("请输入需要恢复连接的UE id！\n");

    scanf("%d",&uiMme_Ue_Id);

    //判断id是否在线
    char acUEId[10];
    sprintf(acUEId, "%d", uiMme_Ue_Id);

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery0 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery0->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery0->fieldKey = "Connection";
    stquery0->limitName = "MME_UEid";
    stquery0->limitValue = acUEId;
    char acbuf0[20];
    char *online = "离线";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery0, acbuf0);
    if (strcmp(acbuf0, online) == 0)
    {
        printf("UE已离线!\n");
        return -1;
    }

    //判断id是否连接
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery1 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery1->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery1->fieldKey = "Connection";
    stquery1->limitName = "MME_UEid";
    stquery1->limitValue = acUEId;
    char acbuf1[20];
    char *connection = "连接";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery1, acbuf1);
    if (strcmp(acbuf1, connection) == 0)
    {
        printf("UE已连接,无需恢复连接!\n");
        return -1;
    }

    //根据id查ip和port
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery2 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery2->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery2->fieldKey = "UE_IP";
    stquery2->limitName = "MME_UEid";
    stquery2->limitValue = acUEId;
    unsigned char aucip[20];

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery3 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery3->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery3->fieldKey = "UE_PORT";
    stquery3->limitName = "MME_UEid";
    stquery3->limitValue = acUEId;
    unsigned char aucbuf3[4];

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery2, aucip);
    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery3, aucbuf3);
    unsigned int uiport = atoi(aucbuf3);

    NSP_TDYTH_4G_LinkageRecover_First_MMEtoENB(cSendBuf, aucip, uiport);
#else
    NSP_TDYTH_4G_LinkageRecover_First_MMEtoENB(cSendBuf, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT);
#endif
    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENBSockaddr, isin_len);
    if(iRet > 0)
    {
        printf("------------------------step1: MME#Sender -> ENB：0x01------------------------\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
#ifdef DATABASE
    //更新连接状态
    St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *update;
    update = (St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo));

    update->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    update->fieldKey = "Connection";
    update->fieldValue = "连接";
    update->limitName = "MME_UEid";
    update->limitValue = acUEId;
    iRet = NSP_TDYTH_4G_DB_update(mysql, update);
    if(iRet < 0)
    {
        printf("更新连接状态失败！\n");
        return -1;
    }
    NSP_TDYTH_4G_DB_close(mysql);
#endif

    return 0;
}

/*
 * 功能：MME发起连接恢复(前端)
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_FrontLinkageRecover_Function(char *pcRecvBuf)
{
    int iRet;
    unsigned int uiMme_Ue_Id;
    unsigned char aucIP[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned int uiPort;
    /* ------------------------------------*/

    unsigned char cSendBuf[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    
    memcpy(&uiMme_Ue_Id, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_DaemonControl_LEN,sizeof(int));
 //   memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    // ENB
    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB2_IP);
    int isin_len = sizeof(ENBSockaddr);
    /*************************************************************************************************************************/
#ifdef DATABASE
    //判断id是否在线
    char acUEId[10];
    sprintf(acUEId, "%d", uiMme_Ue_Id);

    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery0 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery0->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery0->fieldKey = "Connection";
    stquery0->limitName = "MME_UEid";
    stquery0->limitValue = acUEId;
    char acbuf0[20];
    char *online = "离线";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery0, acbuf0);
    if (strcmp(acbuf0, online) == 0)
    {
        printf("UE已离线!\n");
        return -1;
    }

    //判断id是否连接
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery1 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery1->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery1->fieldKey = "Connection";
    stquery1->limitName = "MME_UEid";
    stquery1->limitValue = acUEId;
    char acbuf1[20];
    char *connection = "连接";

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery1, acbuf1);
    if (strcmp(acbuf1, connection) == 0)
    {
        printf("UE已连接,无需恢复连接!\n");
        return -1;
    }

    //根据id查ip和port
    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery2 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery2->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery2->fieldKey = "UE_IP";
    stquery2->limitName = "MME_UEid";
    stquery2->limitValue = acUEId;
    unsigned char aucip[20];

    St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *stquery3 = (St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBQueryInfo));
    stquery3->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    stquery3->fieldKey = "UE_PORT";
    stquery3->limitName = "MME_UEid";
    stquery3->limitValue = acUEId;
    unsigned char aucbuf3[4];

    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery2, aucip);
    NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(mysql, stquery3, aucbuf3);
    unsigned int uiport = atoi(aucbuf3);

    NSP_TDYTH_4G_FrontLinkageRecover_First_MMEtoENB(cSendBuf, aucIP, uiPort);
#else
    NSP_TDYTH_4G_FrontLinkageRecover_First_MMEtoENB(cSendBuf, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UE_PORT);
#endif
    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENBSockaddr, isin_len);
    if(iRet > 0)
    {
        printf("------------------------step1: MME#Sender -> ENB：0x01------------------------\n");
        printf_dae_mmesend("连接恢复请求已发送!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
#ifdef DATABASE
    //更新连接状态
    St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *update;
    update = (St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *)malloc(sizeof(St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo));

    update->tableName = "NSP_TDYTH_MME_USER_INFORMATION";
    update->fieldKey = "Connection";
    update->fieldValue = "连接";
    update->limitName = "MME_UEid";
    update->limitValue = acUEId;
    iRet = NSP_TDYTH_4G_DB_update(mysql, update);
    if(iRet < 0)
    {
        printf("更新连接状态失败！\n");
        return -1;
    }
    NSP_TDYTH_4G_DB_close(mysql);
#endif
    return 0;
}

/***********************************************************************TAU***********************************************************************/
/*
 * 功能：  TAU流程第3步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Third_NewMMEToOldMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME stTauBodyThirdNewMMEToOldMME;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME *stTauBodySecondENBToMME = (St_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodySecondENBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x03;

    stTauBodyThirdNewMMEToOldMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyThirdNewMMEToOldMME.aucDstId, D_NSP_TDYTH_NAAAS_MME2_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyThirdNewMMEToOldMME.uiDstPort = D_NSP_TDYTH_NAAAS_MME2_PORT;
    memcpy(stTauBodyThirdNewMMEToOldMME.aucSrcId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyThirdNewMMEToOldMME.uiSrcPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    stTauBodyThirdNewMMEToOldMME.uiDataLen = 0;

    memcpy(stTauBodyThirdNewMMEToOldMME.aucTerminalId, stTauBodySecondENBToMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodyThirdNewMMEToOldMME.aucOldGUTI, stTauBodySecondENBToMME->aucOldGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stTauBodyThirdNewMMEToOldMME.aucOldTAI, stTauBodySecondENBToMME->aucOldTAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyThirdNewMMEToOldMME, D_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyThirdNewMMEToOldMME.aucDstId, stTauBodyThirdNewMMEToOldMME.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  TAU流程第4步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Fourth_OldMMEToNewMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME stTauBodyForthOldMMEToNewMME;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME *stTauBodyThirdNewMMEToOldMME = (St_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyThirdNewMMEToOldMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x04;

    stTauBodyForthOldMMEToNewMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyForthOldMMEToNewMME.aucDstId, stTauBodyThirdNewMMEToOldMME->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyForthOldMMEToNewMME.uiDstPort = stTauBodyThirdNewMMEToOldMME->uiSrcPort;
    memcpy(stTauBodyForthOldMMEToNewMME.aucSrcId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyForthOldMMEToNewMME.uiSrcPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    stTauBodyForthOldMMEToNewMME.uiDataLen = 0;

    memcpy(stTauBodyForthOldMMEToNewMME.aucTerminalId, stTauBodyThirdNewMMEToOldMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    //这里应该有一个根据OldGUTI查询IMSI的过程，此处省略
    //…………
    memcpy(stTauBodyForthOldMMEToNewMME.aucIMSI, "000000000000000", D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyForthOldMMEToNewMME, D_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyForthOldMMEToNewMME.aucDstId, stTauBodyForthOldMMEToNewMME.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  TAU流程第5步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Fifth_NewMMEToOldMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyFifthNewMMEToOldMME stTauBodyFifthNewMMEToOldMME;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME *stTauBodyFourthOldMMEToNewMME = (St_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyFourthOldMMEToNewMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x05;

    stTauBodyFifthNewMMEToOldMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyFifthNewMMEToOldMME.aucDstId, stTauBodyFourthOldMMEToNewMME->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyFifthNewMMEToOldMME.uiDstPort = stTauBodyFourthOldMMEToNewMME->uiSrcPort;
    memcpy(stTauBodyFifthNewMMEToOldMME.aucSrcId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyFifthNewMMEToOldMME.uiSrcPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    stTauBodyFifthNewMMEToOldMME.uiDataLen = 0;

    memcpy(stTauBodyFifthNewMMEToOldMME.aucTerminalId, stTauBodyFourthOldMMEToNewMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyFifthNewMMEToOldMME, D_NSP_TDYTH_NAAAS_4G_TauBodyFifthNewMMEToOldMME_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyFifthNewMMEToOldMME.aucDstId, stTauBodyFifthNewMMEToOldMME.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  TAU流程第7步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Seventh_NewMMEToNewSGW(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW stTauBodySeventhNewMMEToNewSGW;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME *stTauBodyFourthOldMMEToNewMME = (St_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME *)aucbody;

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x07;

    stTauBodySeventhNewMMEToNewSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodySeventhNewMMEToNewSGW.aucDstId, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodySeventhNewMMEToNewSGW.uiDstPort = D_NSP_TDYTH_NAAAS_SGW1_PORT;
    memcpy(stTauBodySeventhNewMMEToNewSGW.aucSrcId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodySeventhNewMMEToNewSGW.uiSrcPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    stTauBodySeventhNewMMEToNewSGW.uiDataLen = 0;

    memcpy(stTauBodySeventhNewMMEToNewSGW.aucTerminalId, "000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodySeventhNewMMEToNewSGW.aucIMSI, stTauBodyFourthOldMMEToNewMME->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodySeventhNewMMEToNewSGW, D_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodySeventhNewMMEToNewSGW.aucDstId, stTauBodySeventhNewMMEToNewSGW.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  TAU流程第11步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Eleventh_NewMMEToHSS(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS stTauBodyEleventhNewMMEToHSS;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME *stTauBodyTenthNewSGWToNewMME = (St_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyTenthNewSGWToNewMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0b;

    stTauBodyEleventhNewMMEToHSS.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyEleventhNewMMEToHSS.aucDstId, D_NSP_TDYTH_NAAAS_HSS_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyEleventhNewMMEToHSS.uiDstPort = D_NSP_TDYTH_NAAAS_HSS_PORT;
    memcpy(stTauBodyEleventhNewMMEToHSS.aucSrcId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyEleventhNewMMEToHSS.uiSrcPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    stTauBodyEleventhNewMMEToHSS.uiDataLen = 0;

    memcpy(stTauBodyEleventhNewMMEToHSS.aucTerminalId, stTauBodyTenthNewSGWToNewMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodyEleventhNewMMEToHSS.aucIMSI, stTauBodyTenthNewSGWToNewMME->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyEleventhNewMMEToHSS, D_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyEleventhNewMMEToHSS.aucDstId, stTauBodyEleventhNewMMEToHSS.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  TAU流程第13步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Thirteenth_OldMMEToHSS(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS stTauBodyThirteenthOldMMEToHSS;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME *stTauBodyTwelfthHSSToOldMME = (St_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyTwelfthHSSToOldMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0d;

    stTauBodyThirteenthOldMMEToHSS.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyThirteenthOldMMEToHSS.aucDstId, D_NSP_TDYTH_NAAAS_HSS_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyThirteenthOldMMEToHSS.uiDstPort = D_NSP_TDYTH_NAAAS_HSS_PORT;
    memcpy(stTauBodyThirteenthOldMMEToHSS.aucSrcId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyThirteenthOldMMEToHSS.uiSrcPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    stTauBodyThirteenthOldMMEToHSS.uiDataLen = 0;

    memcpy(stTauBodyThirteenthOldMMEToHSS.aucTerminalId, stTauBodyTwelfthHSSToOldMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodyThirteenthOldMMEToHSS.aucIMSI, stTauBodyTwelfthHSSToOldMME->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyThirteenthOldMMEToHSS, D_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyThirteenthOldMMEToHSS.aucDstId, stTauBodyThirteenthOldMMEToHSS.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  TAU流程第15步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Fifteenth_OldMMEToOldSGW(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW stTauBodyFifteenthOldMMEToOldSGW;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME *stTauBodyTwelfthHSSToOldMME = (St_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyTwelfthHSSToOldMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0f;

    stTauBodyFifteenthOldMMEToOldSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyFifteenthOldMMEToOldSGW.aucDstId, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyFifteenthOldMMEToOldSGW.uiDstPort = D_NSP_TDYTH_NAAAS_SGW1_PORT;
    memcpy(stTauBodyFifteenthOldMMEToOldSGW.aucSrcId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyFifteenthOldMMEToOldSGW.uiSrcPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    stTauBodyFifteenthOldMMEToOldSGW.uiDataLen = 0;

    memcpy(stTauBodyFifteenthOldMMEToOldSGW.aucTerminalId, stTauBodyTwelfthHSSToOldMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodyFifteenthOldMMEToOldSGW.aucIMSI, stTauBodyTwelfthHSSToOldMME->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyFifteenthOldMMEToOldSGW, D_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyFifteenthOldMMEToOldSGW.aucDstId, stTauBodyFifteenthOldMMEToOldSGW.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  TAU流程第17步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Seventeenth_NewMMEToUE(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE stTauBodySeventeenthNewMMEToUE;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME *stTauBodyFourteenthHSSToNewMME = (St_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyFourteenthHSSToNewMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x11;

    stTauBodySeventeenthNewMMEToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodySeventeenthNewMMEToUE.aucDstId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodySeventeenthNewMMEToUE.uiDstPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    memcpy(stTauBodySeventeenthNewMMEToUE.aucSrcId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodySeventeenthNewMMEToUE.uiSrcPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    stTauBodySeventeenthNewMMEToUE.uiDataLen = 0;

    memcpy(stTauBodySeventeenthNewMMEToUE.aucTerminalId, stTauBodyFourteenthHSSToNewMME->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    //此处为传给UE的TAIList，可以根据需要增减
    memcpy(stTauBodySeventeenthNewMMEToUE.aucTAIList[0], D_NSP_TDYTH_NAAAS_4G_ENB1_TAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);
    stTauBodySeventeenthNewMMEToUE.iTAIList_Size = 1;

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodySeventeenthNewMMEToUE, D_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodySeventeenthNewMMEToUE.aucDstId, stTauBodySeventeenthNewMMEToUE.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/***********************************************************************TAU结束***********************************************************************/

/*
 * 功能： MME开启监听流程
 * 参数：
 * 返回值：
 *            0   成功
 *          < 0   失败
 */
void * NSP_TDYTH_4G_ListenAccept_MME_Function(int* iSockfd)
{
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];

    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();

    printf(" 当前线程信息为 :pid %u tid %u (0x%x)\n", (unsigned int) pid,
           (unsigned int) tid, (unsigned int) tid);
    // MME1
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    // MME2
    struct sockaddr_in MME2Sockaddr;
    bzero(&MME2Sockaddr, sizeof(MME2Sockaddr));
    MME2Sockaddr.sin_family = AF_INET;
    MME2Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME2_PORT);
    MME2Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME2_IP);

    // HSS
    struct sockaddr_in HSSSockaddr;
    bzero(&HSSSockaddr, sizeof(HSSSockaddr));
    HSSSockaddr.sin_family = AF_INET;
    HSSSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_HSS_PORT);
    HSSSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_HSS_IP);

    // SGW
    struct sockaddr_in SGWSockaddr;
    bzero(&SGWSockaddr, sizeof(SGWSockaddr));
    SGWSockaddr.sin_family = AF_INET;
    SGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_SGW1_PORT);
    SGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_SGW1_IP);

    // ENB1
    struct sockaddr_in ENB1Sockaddr;
    bzero(&ENB1Sockaddr, sizeof(ENB1Sockaddr));
    ENB1Sockaddr.sin_family = AF_INET;
    ENB1Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENB1Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);

    // ENB2
    struct sockaddr_in ENB2Sockaddr;
    bzero(&ENB2Sockaddr, sizeof(ENB2Sockaddr));
    ENB2Sockaddr.sin_family = AF_INET;
    ENB2Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB2_PORT);
    ENB2Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB2_IP);


    int isin_len = sizeof(MMESockaddr);


    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));
    for(;;)
    {
        iRet = recvfrom(*iSockfd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN, 0,
                        (struct sockaddr *) &MMESockaddr, &isin_len);
        if (iRet < 0)
        {
            return NULL;
        }
        if (iRet > 0)
        {
            memcpy(cHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *)cHeader;             // 解析从mme发来的包头
            printf("reqtype:%d", pstHeader->ucReqType);
            switch (pstHeader->ucReqType)   // 判断请求类别
            {
                case 0x01:
                    printf("----------------------目前处于认证流程！---------------------\n");
                    switch (pstHeader->ucStep) // 判断步骤
                    {
                        case 0x04:
                            printf("-------------step4. MME#Receiver <- ENB:0x04-------------\n");
                            NSP_TDYTH_4G_accessAuth_Auth_Data_Request(cRecvBuf,*iSockfd);     //发送认证请求至HSS
                            break;
                        case 0x08:
                            printf("-------------step8. MME#Receiver <- HSS:0x08-------------\n");
                            NSP_TDYTH_4G_accessAuth_User_Auth_Request(cRecvBuf, *iSockfd);     //发送认证请求至UE 0x09
                            break;
                        case 0x0c:
                            printf("-------------step12. MME#Receiver <- ENB:0x0b-------------\n");
                            NSP_TDYTH_4G_accessAuth_AuthResult(cRecvBuf, *iSockfd);
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x02:
                    printf("-------------目前处于请求用户信息流程！-------------\n");
                    switch (pstHeader->ucStep) // 判断步骤
                    {
                        case 0x02:
                            printf("-------------step2.MME#Receiver ->HSS:0x02-------------\n");
                            if(NSP_TDYTH_4G_UserInfo_Request_Third_MMESendToUE(cRecvBuf, cSendBuf) < 0) break;
                            NSP_TDYTH_4G_UserInfo_Request_ThirdSend_MMESendToUE(cRecvBuf, cSendBuf, *iSockfd);
                            break;
                        case 0x04:
                            printf("-------------step4.MME#Receiver ->UE:0x04-------------\n");
                            NSP_TDYTH_4G_accessAuth_UserInfo(cRecvBuf);
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x03:
                    printf("-------------目前处于附着流程！-------------\n");
                    switch (pstHeader->ucStep) // 判断步骤
                    {
                        case 0x04:
                            printf("-------------step4.MME#Receiver ->ENB:0x04-------------\n");
                            if(NSP_TDYTH_4G_Attach_Fourth_eNB_Send(cRecvBuf, cSendBuf) < 0) break;
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x04:          // IP分配流程 0x04
                    printf("-------------目前处于IP分配流程！-------------\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x04:
                            printf("-------------step4. MME#Receiver <- SGW:0x04-------------\n");
                            //接收从SGW发送的用户请求会话响应

                            if(NSP_TDYTH_4G_IPAllot_Fifth_MMESendToENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN, 0, 
                                        (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("附着接受,step: 5 完成.\n");
                            }
                            printf("-------------step5: MME#Sender -> ENB：0x05-------------\n");

                            break;

                        case 0x08:
                            printf("-------------step8. MME#Receiver <- ENB:0x08-------------\n");

                            printf("IP分配完成,附着完成\n");
                            
                            printf_dae_mmesend("IP分配完成，附着完成!\n");
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "MME:recv err,流程错误,结束认证..", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x05:       // X2切换流程
                    printf("-------------目前处于X2切换流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x07:

                            printf("-------------step7.MME#Receiver ->TENB:0x07-------------\n");
                            NSP_TDYTH_4G_X2_HandOver_Eighth_MMESendToTENB(cRecvBuf, cSendBuf);

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB2Sockaddr, sizeof(ENB2Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("路径切换请求确认发送完毕,step: 8 完成.\n");
                            }

                            printf("-------------step8.MME#Sender ->TENB:0x08-------------\n");
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;

                case 0x06:       // S1切换流程
                    printf("-------------目前处于S1切换流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x02:

                            printf("-------------step2.MME#Receiver ->SENB:0x02-------------\n");
                            if(NSP_TDYTH_4G_S1_HandOver_Third_MMESendToTENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("路径切换请求发送完毕,step: 3 完成.\n");
                            }

                            printf("-------------step3.MME#Sender ->TENB:0x03-------------\n");
                            break;
                        case 0x04:

                            printf("-------------step4.MME#Receiver ->TENB:0x04-------------\n");
                            if(NSP_TDYTH_4G_S1_HandOver_Fifth_MMESendToSENB(cRecvBuf, cSendBuf) < 0);

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("切换命令发送完毕,step: 5 完成.\n");
                            }

                            printf("-------------step5.MME#Sender ->SENB:0x05-------------\n");
                            break;
                        case 0x07:

                            printf("-------------step7.MME#Receiver ->SENB:0x07-------------\n");
                            NSP_TDYTH_4G_S1_HandOver_Eighth_MMESendToTENB(cRecvBuf, cSendBuf);

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB2Sockaddr, sizeof(ENB2Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("MME SN状态转移命令发送完毕,step: 8 完成.\n");
                            }

                            printf("-------------step8.MME#Sender ->TENB:0x08-------------\n");
                            break;

                        case 0x0a:

                            printf("-------------step10.MME#Receiver ->TENB:0x0a-------------\n");
                            if(NSP_TDYTH_4G_S1_HandOver_Eleventh_MMESendToSENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("UE消息释放命令发送完毕,step: 11 完成.\n");
                            }

                            printf("-------------step11.MME#Sender ->TENB:0x0b-------------\n");
                            break;
                        case 0x0c:

                            printf("-------------step12.MME#Receiver ->SENB:0x0c-------------\n");

                            printf("UE消息释放完成、S1切换完成！\n");
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x0b:
                    printf("-------------目前处于专有承载建立流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x03:
                            printf("-------------step3.MME#Receiver ->ENB:0x03-------------\n");
                            if (NSP_TDYTH_4G_BearerSetUp_Fourth_MMEToENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("--------------step4.MME#Sender ->ENB:0x04-------------\n");
                            }
                            break;
                        case 0x07:
                            printf("-------------step7.MME#Receiver ->ENB:0x07-------------\n");

                            break;
                        case 0x09:
                            printf("-------------step9.MME#Receiver ->ENB:0x09--------------\n");
                            if (NSP_TDYTH_4G_BearerSetUp_Tenth_MMEToSGW(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step10.MME#Sender ->SGW:0x0a-------------\n");
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
                        case 0x02:
                            printf("----------TAU: step2. New MME#Receiver -> ENB：TAU Request----------\n");
                            if(NSP_TDYTH_4G_TAU_Third_NewMMEToOldMME(cRecvBuf, cSendBuf, *iSockfd) < 0 ){
                                printf("----------TAU: step3. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step3. New MME#Sender -> Old MME：Context Request----------\n");
                            }
                            break;
                        case 0x03:
                            printf("----------TAU: step3. Old MME#Receiver -> New MME：Context Request----------\n");
                            if(NSP_TDYTH_4G_TAU_Fourth_OldMMEToNewMME(cRecvBuf, cSendBuf, *iSockfd) < 0 ){
                                printf("----------TAU: step4. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step4. Old MME#Sender -> New MME：Context Response----------\n");
                            }
                            break;
                        case 0x04:
                            printf("----------TAU: step4. New MME#Receiver -> Old MME：Context Response----------\n");
                            if(NSP_TDYTH_4G_TAU_Fifth_NewMMEToOldMME(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                                printf("----------TAU: step5. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step5. New MME#Sender -> Old MME：Context Acknowledge----------\n");
                            }
                            printf("----------TAU: step6. MME检测是否需要进行SGW重选----------\n");
                            //这里原本应该有一个判断是否重选SGW的过程，此处省略，默认需要进行SGW的重选
                            if(NSP_TDYTH_4G_TAU_Seventh_NewMMEToNewSGW(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                                printf("----------TAU: step7. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step7. New MME#Sender -> New SGW：Create Session Request----------\n");
                            }
                            //上面的语句当SGW重选时执行
                            break;
                        case 0x05:
                            printf("----------TAU: step5. Old MME#Receiver -> New MME：Context Acknowledge----------\n");
                            printf("----------收到Context Acknowledge消息----------\n");
                            break;
                        case 0x0a:
                            printf("----------TAU: step10. New MME#Receiver -> New SGW：Create Session Response----------\n");
                            if(NSP_TDYTH_4G_TAU_Eleventh_NewMMEToHSS(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                                printf("----------TAU: step11. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step11. New MME#Sender -> HSS：Update Location Request----------\n");
                            }
                            break;
                        case 0x0c:
                            printf("----------TAU: step12. Old MME#Receiver -> HSS：Cancel Location Request----------\n");
                            if(NSP_TDYTH_4G_TAU_Thirteenth_OldMMEToHSS(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                                printf("----------TAU: step13. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step13. Old MME#Sender -> HSS：Cancel Location Ack----------\n");
                            }
                            if(NSP_TDYTH_4G_TAU_Fifteenth_OldMMEToOldSGW(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                                printf("----------TAU: step15. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step15. Old MME#Sender -> Old SGW：Delete Session Request----------\n");
                            }
                            break;
                        case 0x0e:
                            printf("----------TAU: step14. New MME#Receiver -> HSS：Update Location Answer----------\n");
                            if(NSP_TDYTH_4G_TAU_Seventeenth_NewMMEToUE(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                                printf("----------TAU: step17. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step17. New MME#Sender -> UE：TAU Accept----------\n");
                            }
                            break;
                        case 0x10:
                            printf("----------TAU: step16. Old MME#Receiver -> Old SGW：Delete Session Response----------\n");
                            printf("----------收到Delete Session Response消息----------\n");
                            break;
                        case 0x12:
                            printf("----------TAU: step18. New MME#Receiver -> UE：TAU Complete----------\n");
                            printf("----------收到TAU Complete消息----------\n");
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
                return NULL;
            }
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
int NSP_TDYTH_4G_Epoll_MME_Function()
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
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    if (bind(iFD, (struct sockaddr *) &MMESockaddr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        return -1;
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
        nfds = epoll_wait(kdpfd, events, 10000, 2000);

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
                if(pthread_create(&thread,&attr,(void*)NSP_TDYTH_4G_ListenAccept_MME_Function,(void*)&(events[n].data.fd)))
                {
                    perror("pthread_creat error!\n");
                    return -1;
                }
            }
        }
    }
    close(kdpfd);
    close(iFD);
    return 0;
}

/*
 * 功能：通过接受前端指令选择功能
 * 参数： 
 * 返回值：  
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_MMEFront_Function()
{
    int iRet;
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];


    int iSockfdMME = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MMEF_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    if (bind(iSockfdMME, (struct sockaddr *) &MMESockaddr, sizeof(MMESockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    
    memset(cRecvBuf, 0, sizeof(cRecvBuf));

    while(1)
    {

        iRet = recvfrom(iSockfdMME, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN, 0,
                (struct sockaddr *) &MMESockaddr, (socklen_t *)sizeof(MMESockaddr));

        St_NSP_TDYTH_NAAAS_4G_DaemonControl *pstDaemonControl = (St_NSP_TDYTH_NAAAS_4G_DaemonControl *)cRecvBuf;

        switch (pstDaemonControl->uiFunction)
        {
            case D_NSP_TDYTH_NAAAS_4G_MME_REGUSERINFO:
                NSP_TDYTH_4G_show_register_userInfo();
                break;
            case D_NSP_TDYTH_NAAAS_4G_MME_ONLINEUSERINFO:
                NSP_TDYTH_4G_show_online_userInfo();
                break;
            case D_NSP_TDYTH_NAAAS_4G_MME_NETTOPOLOGY:
                NSP_TDYTH_4G_NetworkTopology();
                break;
            case D_NSP_TDYTH_NAAAS_4G_MME_AUTHBEGIN:
                NSP_TDYTH_4G_FrontAccessAuth_Function(cRecvBuf);
                break;
            case D_NSP_TDYTH_NAAAS_4G_MME_IPALLOT:
                NSP_TDYTH_4G_FrontIPAllot_Function(cRecvBuf);
                break;
            case D_NSP_TDYTH_NAAAS_4G_MME_LINKBREAK:
                NSP_TDYTH_4G_FrontLinkageInterrupt_Function(cRecvBuf);
                break;
            case D_NSP_TDYTH_NAAAS_4G_MME_LINKRECOVER:
                NSP_TDYTH_4G_FrontLinkageRecover_Function(cRecvBuf);
                break;
            default:
                printf("格式错误\n");
                break;
        }
    }
    return 0;
}



/*
 * 功能：三合一处理函数
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_ThreeToOne_Function()
{
    int iFD;

    /* 开启 socket 监听 */
    if ((iFD = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket create failed");
        exit(1);
    }

    //ENB Sockaddr
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    if (bind(iFD, (struct sockaddr *) &MMESockaddr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        return -1;
    }
    NSP_TDYTH_4G_ListenAccept_MME_Function(&iFD);
    /* 处理所有事件 */
    close(iFD);
    return 0;
}


/*
 * 功能：通过后台选取功能
 * 参数： 
 * 返回值：  
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_MMEBack_Function()
{
    int iMME_function;

    while(1)
    {
        puts("请选择MME需要执行的功能：");
        puts("1.注册用户信息展示");
        puts("2.在线用户信息展示");
        puts("3.网络拓扑展示");
        puts("4.发起认证流程");
        puts("5.发起IP分配流程");
        puts("6.MME中断UE连接");
        puts("7.MME恢复UE连接");
        puts("99.退出");
        puts("......后续添加.......");
        scanf("%d", &iMME_function);
        getchar();
        switch(iMME_function)
        {
            case 1:
                NSP_TDYTH_4G_show_register_userInfo();
                break;
            case 2:
                NSP_TDYTH_4G_show_online_userInfo();
                break;
            case 3:
                NSP_TDYTH_4G_NetworkTopology();
                break;
            case 4:
                NSP_TDYTH_4G_AccessAuth_Function();
                break;
            case 5:
                NSP_TDYTH_4G_IPAllot_Function();
                break;
            case 6:
                NSP_TDYTH_4G_LinkageInterrupt_Function();
                break;
            case 7:
                NSP_TDYTH_4G_LinkageRecover_Function();
                break;
            case 99:
                return 0;
            default:
                printf("输入错误\n");
                break;
        }
    }
    return 0;
}


int main()
{
    int iMME_select;

    pthread_t pth_epoll;

    printf("------------开启子线程进行epoll监听------------\n");

    if(pthread_create(&pth_epoll, NULL, (void *)NSP_TDYTH_4G_Epoll_MME_Function, NULL))
    {
        perror("pthread_creat error!\n");
        return -1;
    }
    else
    {
        printf("---------------子线程开启成功---------------\n");
    }

#ifdef DATABASE
    NSP_TDYTH_4G_CleanOut();
#endif

    while(1)
    {
        puts("请选择MME需要展现的方式：");
        puts("1.接受前端消息，结果在前端展示(涉及前端)");
        puts("2.后台操作(纯后台)");
        puts("99.退出");
        scanf("%d", &iMME_select);
        getchar();
        switch(iMME_select)
        {
            case 1:
                NSP_TDYTH_4G_MMEFront_Function();
                break;
            case 2:
                NSP_TDYTH_4G_MMEBack_Function();
                break;
            case 99:
                return 0;
            default:
                printf("输入错误\n");
                break;
        }
    }
}


