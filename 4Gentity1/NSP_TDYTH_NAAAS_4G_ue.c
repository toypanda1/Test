/*
 * 文件功能：UE与MME通信的协议（UE端）
 * 作者：关键 孙雄韬 张一帆
 * 创建日期：
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_anet.h"
#include "NSP_TDYTH_NAAAS_4G_gen.h"
#include "NSP_TDYTH_NAAAS_4G_utils.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"
#include "NSP_TDYTH_NAAAS_4G_debug.h"
#include "NSP_TDYTH_NAAAS_4G_db.h"

static int gs_iIsOpen = 0;                                          //是否开机
static int gs_iIsReadCard = 0;                                      //是否读卡
static int gs_iIsRegis = 0;                                         //是否注册
static int gs_iIsAuth = 0;                                          //是否认证
static int gs_iENBnum = -1;                                         //UE当前驻留基站标识；-1为无驻留
static int gs_iBestENBnum = -1;                                     //UE检索出的最优基站
/*
 * 备注：协议中用到UE端自己本身含有或者可以得到的数据如：
 * (1)HSS ID 是在制卡时就写入SIM卡的数据
 * (2)SN id(即SNID)是在UE广播时可以得到
 * (3)在认证过程中，UE端需要保存一些信息，按照文档中数据库对应表，暂时保存这些信息，UE端保存信息结构体自行定义和保存。
 */
//UE端存储的NH与NCC对
KeyPair keypair[7];

//UE端存储信息
typedef struct {
    unsigned int uiID_UE;
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char ucHSSID;
    unsigned char aucSNID[D_NSP_TDYTH_NAAAS_4G_SNID_LEN];
    unsigned char aucSQNID[5];
    unsigned char aucAMF[D_NSP_TDYTH_NAAAS_4G_AMF_LEN];
    unsigned char aucK[D_NSP_TDYTH_NAAAS_4G_KEY_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucKasme[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuUE;

//功能：UE端存储注册信息结构体
typedef struct
{
    unsigned char aucUserId[D_NSP_TDYTH_NAAAS_4G_USERTD_LEN];
    unsigned char aucSex;
    unsigned char aucIdNum[D_NSP_TDYTH_NAAAS_4G_IdNum_LEN];
    unsigned char aucKind;
    unsigned char aucCountry[D_NSP_TDYTH_NAAAS_4G_Country_LEN];
    unsigned char aucProvince[D_NSP_TDYTH_NAAAS_4G_Province_LEN];
    unsigned char aucCity[D_NSP_TDYTH_NAAAS_4G_City_LEN];
    unsigned char aucDetailAddr[D_NSP_TDYTH_NAAAS_4G_Addr_LEN];
    unsigned char aucServicePasswd[D_NSP_TDYTH_NAAAS_4G_SerPWD_LEN];
    unsigned char aucAccountTime[D_NSP_TDYTH_NAAAS_4G_AcTIME_LEN];


} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_RegisterInfo;


//UE端存储SIM卡信息
typedef struct
{
    unsigned char aucCardID[11];
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN+1];
    unsigned char aucHSSID[15];
    unsigned char aucPIN[4];
    unsigned char aucPUK[8];
    unsigned char aucKI[33];
    unsigned char aucDATE[15];
    unsigned char aucPHONE[15];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_SimInfo;

//UE端GPS信息
typedef struct {
    char acCity[20];
    char acCountry[20];
    char acIp[20];
    double dLongitude;
    double dLlatitude;
}__attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_GPS;

//UE端存储的自身的位置，与当前驻留基站与最优基站
typedef struct {
    double E;
    double N;
    St_NSP_TDYTH_NAAAS_4G_EnbInfo curGNBInfo;
    St_NSP_TDYTH_NAAAS_4G_EnbInfo bestGNBInfo;
} UE_INFO;
UE_INFO ue_Info;

St_NSP_TDYTH_NAAAS_4G_RegisterInfo RegisterInfo;
static St_NSP_TDYTH_NAAAS_4G_SimInfo gs_SimInfo;
static St_NSP_TDYTH_NAAAS_4G_AuUE s_stUE;
static St_NSP_TDYTH_NAAAS_4G_GPS gs_stGPS;

static St_NSP_TDYTH_NAAAS_4G_EnbInfo gs_astEnbList[10];
static int gs_iEnblist_last = -1;       //创建基站信息表的末尾标识，便于添加和删除

//UE支持载频列表
static unsigned int gs_auiEARFCN_List[] = {38400, 38544};
//UE端存储的TAIList信息
static unsigned char gs_aucTAIList[10][D_NSP_TDYTH_NAAAS_4G_TAI_LEN] = {"4600060001"};
static int gs_iTAIList_Size = 1;


/*
 * 功能：UE进行测量
 * 参数：   
 * 返回值：    
 *              result   测量结果
 */
int NSP_TDYTH_4G_UE_Measure()
{
    int result = 100;  //应通过硬件测量等获得，并且不止一个测量结果，此处简易替代

    return result;
}

/*
 * 功能：UE进行去附着
 * 参数：   
 * 返回值：    
 *              0   去附着成功
 */
int NSP_TDYTH_4G_UE_Deattach_Function()
{
    return 0;
}

int EnterWebURL(char *pcWebUrl)
{
    printf("请输入需要访问的网页URL\n");
    scanf("%s",pcWebUrl);
    return 0;
}

int IpFunction(char *pcWebUrl)
{
    int ip_function;
    printf("请选择上网功能\n1.访问网页\n99.退出\n");
    scanf("%d", &ip_function);
    getchar();
    switch(ip_function)
    {
        case 1:
            EnterWebURL(pcWebUrl);
            break;
        case 99:
            return -1;
        default:
            printf("输入错误\n");
            break;
    }
    return 0;
}
/*
 * 功能：开机
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int NSP_TDYTH_4G_UE_poweron()
{
    if(gs_iIsOpen)
    {
        printf("终端UE已开机！\n");
        printf_dae_uesend("终端UE已开机！\n");
        return -1;
    }

    gs_iIsOpen = 1;
    printf("终端UE开机成功!\n");
    printf_dae_uesend("终端UE开机成功!\n");

    return 0;
}

/*
 * 功能：关机
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int NSP_TDYTH_4G_UE_poweroff()
{
    int iRet;
    if(!gs_iIsOpen)
    {
        printf("终端UE目前处于关机！\n");
        printf_dae_uesend("终端UE目前处于关机！\n");
        return -1;
    }

    gs_iIsOpen = 0;

    printf("终端UE关机成功!\n");
    printf_dae_uesend("终端UE关机成功!\n");

#ifdef DATABASE
    MYSQL *mysql = NSP_TDYTH_4G_DB_connect();
    char acIMSI[16];
    memcpy(acIMSI,gs_SimInfo.aucIMSI,15);
    char sql1[200];
    char *offline = "离线";
    sprintf(sql1, "update(NSP_TDYTH_MME_USER_INFORMATION)SET Online='%s' where IMSI='%s';", offline, acIMSI);
    iRet = mysql_query(mysql, sql1);
    if (iRet < 0) {
        printf("关机失败！\n");
        return -1;
    }

    NSP_TDYTH_4G_DB_close(mysql);
#endif
    
    return 0;
}

/*
 * 功能：打电话功能
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int NSP_TDYTH_4G_UE_phone()
{

    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    if(gs_iIsRegis == 0)
    {
        printf("终端UE还未注册,请注册！\n");
        printf_dae_uesend("终端UE还未注册,请注册！\n");
        return -1;
    }
    //后续应该添加是否已经进行了IP分配与认证，再决定是否打电话

    printf("开始打电话!\n");
    printf_dae_uesend("开始打电话!\n");

    return 0;
}

/*
 * 功能：重启
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int NSP_TDYTH_4G_UE_restart()
{

    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    printf("终端重启成功!\n");
    printf_dae_uesend("终端重启成功!\n");

    return 0;
}

/*
 * 功能：接收用户身份请求，发起用户身份响应
 * 参数：	@xxx [pcRecvBuf in]	接收缓冲区
 *         @xxx [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          < 0 失败
 */
 int NSP_TDYTH_4G_accessAuth_User_Identity_Response(char *pcRecvBuf, char *pcSendBuf)
 {
     int iRet;
     unsigned char aucAubody_mme[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];           //解析从mme传来的认证请求数据
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     St_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB stAuBodyThirdUEToENB;


     //1、首先接收MME->UE的用户身份请求
     memcpy(&aucAubody_mme, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodySecondENBToUE_LEN);
     St_NSP_TDYTH_NAAAS_4G_AuBodySecondENBToUE *stAuBodySecondENBToUE = (St_NSP_TDYTH_NAAAS_4G_AuBodySecondENBToUE *)aucAubody_mme;  //解析
     if(NSP_TDYTH_4G_verification_timestamp(stAuBodySecondENBToUE->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }

     printf("收到MME请求：%x, 解析完成,开始构造用户身份响应.\n", stAuBodySecondENBToUE->ucReqAuth);
    
     //2、进行用户身份响应UE->MME，构建发送数据：IMSI、GUTI、HSS ID
     stHeader.ucVersion = 0x01;
     stHeader.usModuleCode = 0x0000;
     stHeader.ucReqType = 0x01;
     stHeader.ucSubType = 0x00;
     stHeader.ucUserCategory = 0x00;
     stHeader.ucStep = 0x03;

     stAuBodyThirdUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
     stAuBodyThirdUEToENB.aucIdNumber = 0x0;
     memcpy(stAuBodyThirdUEToENB.aucDstId, stAuBodySecondENBToUE->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     memcpy(stAuBodyThirdUEToENB.aucSrcId, stAuBodySecondENBToUE->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stAuBodyThirdUEToENB.aucDstPort = stAuBodySecondENBToUE->aucDstPort;
     stAuBodyThirdUEToENB.aucSrcPort = stAuBodySecondENBToUE->aucSrcPort;
     stAuBodyThirdUEToENB.iDataLen = 0;

     memcpy(stAuBodyThirdUEToENB.aucIMSI, gs_SimInfo.aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
     memcpy(stAuBodyThirdUEToENB.aucGUTI, "0000000000", D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
     stAuBodyThirdUEToENB.aucHSSID = 0x00;

     memcpy(stAuBodyThirdUEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     printf("身份请求数据构造完毕.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     printf("the send step is %x\n",stHeader.ucStep);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodyThirdUEToENB, D_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB_LEN); //数据体填入发送缓冲区

     /*
     if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,
                                             D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodySecondUEToMME_LEN,
                                             D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MME1_PORT, 0)) < 0) {
         printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
         return -1;
     }else {
         printf("用户身份响应发送完毕,step:%x完成.\n", stHeader.ucStep);
     }*/
     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }

/*
 * 功能：请求用户信息第4步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_UserInfo_Request_Fourth_UESendToMME(char *pcRecvBuf, char *pcSendBuf)
{

    unsigned char aucAubody_mme[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];           //解析从mme传来的认证请求数据
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Response UserInfoResponse;

    //1、首先接收MME->UE的用户身份请求
    memcpy(&aucAubody_mme, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE *stURBodyThirdMMEToUE = (St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE *)aucAubody_mme;  //解析
    if(NSP_TDYTH_4G_verification_timestamp(stURBodyThirdMMEToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    printf("收到MME请求,开始构造用户信息.\n");

    //2、填写用户信息
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x02;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x04;

    UserInfoResponse.TimeStamp = NSP_TDYTH_4G_get_time_now();
    UserInfoResponse.aucIdNumber = 0x0;
    memcpy(UserInfoResponse.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(UserInfoResponse.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    UserInfoResponse.iDataLen = 0;
    memcpy(UserInfoResponse.aucIMSI, "000111000000000", D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(UserInfoResponse.aucUserName, RegisterInfo.aucUserId, 20);
    memcpy(UserInfoResponse.aucUserID, RegisterInfo.aucIdNum, 25);
    memcpy(UserInfoResponse.aucUserGps, RegisterInfo.aucDetailAddr, 30);
    UserInfoResponse.aucUserStatus = (char)gs_iIsAuth;
    memcpy(UserInfoResponse.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("身份信息构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    printf("the send step is %x\n",stHeader.ucStep);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &UserInfoResponse, D_NSP_TDYTH_NAAAS_4G_UserInfo_Response_LEN); //数据体填入发送缓冲区


    return 0;
}

/*
 * 功能：接收用户认证请求，发起用户认证响应
 * 参数：	@xxx [pcRecvBuf in]	接收缓冲区
 *         @xxx [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_accessAuth_User_Auth_Response(char *pcRecvBuf, char *pcSendBuf) 
{
    int iSockfd;
    int iRet;
    int i;
    int MAC_flag=1;

    unsigned char aucK[D_NSP_TDYTH_NAAAS_4G_KEY_LEN];
    unsigned char aucAK[D_NSP_TDYTH_NAAAS_4G_AK_LEN];
    unsigned char aucCK[D_NSP_TDYTH_NAAAS_4G_CK_LEN];
    unsigned char aucIK[D_NSP_TDYTH_NAAAS_4G_IK_LEN];
    unsigned char aucSQN[D_NSP_TDYTH_NAAAS_4G_SQN_LEN];
    unsigned char aucAMF[D_NSP_TDYTH_NAAAS_4G_AMF_LEN];
    unsigned char aucMAC[D_NSP_TDYTH_NAAAS_4G_MAC_LEN];
    unsigned char aucXMAC[D_NSP_TDYTH_NAAAS_4G_MAC_LEN];
    unsigned char aucRES[D_NSP_TDYTH_NAAAS_4G_RES_LEN];
    unsigned char aucSNID[D_NSP_TDYTH_NAAAS_4G_SNID_LEN];   // UE通过广播得到
    unsigned char aucKasme[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];

    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];
    unsigned char ucKSIASME[D_NSP_TDYTH_NAAAS_4G_KSIASME_LEN];

    unsigned char aucSQNAK[D_NSP_TDYTH_NAAAS_4G_AK_LEN];
    unsigned char aucAubody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    memset(aucK,0,D_NSP_TDYTH_NAAAS_4G_KEY_LEN);
    memset(aucAMF,0,D_NSP_TDYTH_NAAAS_4G_AMF_LEN);

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB stAuBodyEleventhUEToENB;


    memcpy(aucAubody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyTenthENBToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyTenthENBToUE *stAuBodyTenthENBToUE = (St_NSP_TDYTH_NAAAS_4G_AuBodyTenthENBToUE *)aucAubody; //解析数据包
    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyTenthENBToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    printf("认证数据包解析完成，开始验证\n");

    //2、处理数据
    // (1)按照协议内容提取AUTN中的MAC，计算XMAC，验证MAC = XMAC
    memcpy(aucRAND, stAuBodyTenthENBToUE->aucAuthReq, D_NSP_TDYTH_NAAAS_4G_RAND_LEN);
    memcpy(aucAUTN, stAuBodyTenthENBToUE->aucAuthReq + D_NSP_TDYTH_NAAAS_4G_RAND_LEN, D_NSP_TDYTH_NAAAS_4G_AUTN_LEN);
    memcpy(ucKSIASME, stAuBodyTenthENBToUE->aucAuthReq + D_NSP_TDYTH_NAAAS_4G_RAND_LEN + D_NSP_TDYTH_NAAAS_4G_AUTN_LEN,
           D_NSP_TDYTH_NAAAS_4G_KSIASME_LEN);
    memcpy(aucSQNAK, aucAUTN, D_NSP_TDYTH_NAAAS_4G_SQN_LEN);

    memcpy(aucMAC, aucAUTN + D_NSP_TDYTH_NAAAS_4G_SQN_LEN + D_NSP_TDYTH_NAAAS_4G_AMF_LEN, D_NSP_TDYTH_NAAAS_4G_MAC_LEN);

    if ((iRet = NSP_TDYTH_4G_gen_AK(aucK, aucRAND, aucAK)) != 0) //计算AK
    {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_AK err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }

    if ((iRet = NSP_TDYTH_4G_XOR(aucSQNAK, aucAK, D_NSP_TDYTH_NAAAS_4G_AK_LEN, aucSQN)) != 0) //计算SQN^AK
    {
        printf("%s,[line=%d],errno=%d:%s\n", "xor err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }

    if ((iRet = NSP_TDYTH_4G_gen_MAC(aucRAND, aucK, aucSQN, aucAMF, aucXMAC)) != 0) //计算MAC
    {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_XMAC err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }

    //XMAC 和 MAC 比较
    for (i = 0; i < D_NSP_TDYTH_NAAAS_4G_MAC_LEN; i++) {
        if (aucXMAC[i] != aucMAC[i])
        {
            MAC_flag=0;
        }
    }
    if(MAC_flag==0)
        printf("%s,[line=%d],errno=%d:%s\n", "compare_MAC_XMAC err", __LINE__, errno, strerror(errno));
    else
        printf("XMAC验证通过，执行下一步操作\n");
    
    // (2)验证SQN是否在合理范围内
    if ((iRet = NSP_TDYTH_4G_verify_SQN(aucSQN)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "verify_SQN err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_PROTO_SQN_ERR;
    }
    printf("SQN验证通过\n");

    // (3)如果MAC和SQN均验证通过，计算认证响应RES、KASME
    if ((iRet = NSP_TDYTH_4G_gen_RES(aucK, aucRAND, aucRES)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_RES err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }

    if ((NSP_TDYTH_4G_gen_CK(aucK, aucRAND, aucCK)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_CK err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }

    if ((iRet = NSP_TDYTH_4G_gen_IK(aucK, aucRAND, aucIK)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_IK err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }

    //用SNID之前，需要先从UE本地获取其值
    if ((iRet = NSP_TDYTH_4G_gen_Kamse(aucSNID, aucCK, aucIK, aucSQN, aucAK, aucKasme)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kamse err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }
    printf("认证响应RES,KASME计算完毕,构造MME->UE的用户认证响应\n");

    //3、构建发送数据
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x01;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x0b;

    stAuBodyEleventhUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAuBodyEleventhUEToENB.aucIdNumber = 0x0;
    memcpy(stAuBodyEleventhUEToENB.aucDstId, stAuBodyTenthENBToUE->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAuBodyEleventhUEToENB.aucSrcId, stAuBodyTenthENBToUE->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAuBodyEleventhUEToENB.aucDstPort = stAuBodyTenthENBToUE->aucDstPort;
    stAuBodyEleventhUEToENB.aucSrcPort = stAuBodyTenthENBToUE->aucSrcPort;
    stAuBodyEleventhUEToENB.iDataLen = 0;
    memcpy(stAuBodyEleventhUEToENB.aucRes, aucRES, D_NSP_TDYTH_NAAAS_4G_RES_LEN);
    memcpy(stAuBodyEleventhUEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodyEleventhUEToENB, D_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB_LEN);
    printf("MME->UE的用户认证响应构造完毕\n");

    //4、进行用户认证响应UE->MME，发送RES
    //此处将UE作为客户端，MME作为服务端
    if ((iSockfd = NSP_TDYTH_4G_ANET_ipv4_send_init()) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_SEND_ERROR;
    }
/*
    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSender,
                                            D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_AuBodySixthUEToMME_LEN,
                                            D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MME1_PORT, 0)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return -1;
    }
    */

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：接收认证结果反馈信息
 * 参数：	@xxx [pcRecvBuf in]	接收到的缓冲区
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_accessAuth_AuthFeedback(char *pcRecvBuf) 
{
    int iSockfd;
    int iRet;
    unsigned char aucAubody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    memcpy(aucAubody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyFourteenthENBToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyFourteenthENBToUE *stAuBodyFourteenthENBToUE = (St_NSP_TDYTH_NAAAS_4G_AuBodyFourteenthENBToUE *)aucAubody;
    
    if (memcmp(stAuBodyFourteenthENBToUE->aucResult, "success", sizeof("success")) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:access_auth err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_AUTH;
    } else {
        printf("------------------------Authentication %s------------------------\n", stAuBodyFourteenthENBToUE->aucResult);
    }

    gs_iIsAuth = 1;

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：接收用户信息请求，发送用户身份信息
 * 参数：	@xxx [pcRecvBuf in]	接收缓冲区
 *         @xxx [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_accessAuth_UserInfo_Response(char *pcRecvBuf, char *pcSendBuf)
{

    unsigned char aucAubody_mme[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];           //解析从mme传来的认证请求数据
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Response UserInfoResponse;

    //1、首先接收MME->UE的用户身份请求
    memcpy(&aucAubody_mme, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_LEN);
    St_NSP_TDYTH_NAAAS_4G_UserInfo_Request *UserInfoRequest = (St_NSP_TDYTH_NAAAS_4G_UserInfo_Request *)aucAubody_mme;  //解析
    printf("收到MME请求,开始构造用户信息.\n");

    //2、填写用户信息
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x02;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x0b;

    UserInfoResponse.TimeStamp = NSP_TDYTH_4G_get_time_now();
    UserInfoResponse.aucIdNumber = 0x0;
    memcpy(UserInfoResponse.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(UserInfoResponse.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    UserInfoResponse.iDataLen = 0;
    memcpy(UserInfoResponse.aucIMSI, "000111000000000", D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(UserInfoResponse.aucUserName, RegisterInfo.aucUserId, 20);
    memcpy(UserInfoResponse.aucUserID, RegisterInfo.aucIdNum, 25);
    memcpy(UserInfoResponse.aucUserGps, RegisterInfo.aucDetailAddr, 30);
    UserInfoResponse.aucUserStatus = 0x00;
    memcpy(UserInfoResponse.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("身份信息构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    printf("the send step is %x\n",stHeader.ucStep);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &UserInfoResponse, D_NSP_TDYTH_NAAAS_4G_UserInfo_Response_LEN); //数据体填入发送缓冲区


    return 0;
}
/*
 * 功能：注册流程请求，发起用注册流程请求(后台)
 * 参数： @xxx [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_Register_User_Send(char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS stRebodyFirstUEToHSS;
    unsigned char acInPut[1024];              //充当输入缓冲区

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x00;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    while(1)
    {
        printf("1、请输入姓名\n");
//        scanf("%*s");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%[^\n]",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>40)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(RegisterInfo.aucUserId, acInPut, sizeof(acInPut));
            break;
        }
    }
    while(1)
    {
        printf("2、请输入性别(一个字符)\n");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%s",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>1)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(&RegisterInfo.aucSex, acInPut, sizeof(acInPut));
            break;
        }
    }


    while(1)
    {
        printf("3、请输入身份证号\n");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%[^\n]",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>19)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(RegisterInfo.aucIdNum, acInPut, sizeof(acInPut));
            break;
        }
    }
    while(1)
    {
        printf("4、请输入用户类别(一个字符)\n");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%s",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>1)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(&RegisterInfo.aucKind, acInPut, sizeof(acInPut));
            break;
        }
    }


    while(1)
    {
        printf("5、请输入国籍\n");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%[^\n]",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>20)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(RegisterInfo.aucCountry, acInPut, sizeof(acInPut));
            break;
        }
    }
    while(1)
    {
        printf("6、请输入省份\n");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%[^\n]",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>20)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(RegisterInfo.aucProvince, acInPut, sizeof(acInPut));
            break;
        }
    }
    while(1)
    {
        printf("7、请输入城市\n");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%[^\n]",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>20)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(RegisterInfo.aucCity, acInPut, sizeof(acInPut));
            break;
        }
    }
    while(1)
    {
        printf("8、请输入街道地址\n");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%[^\n]",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>30)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(RegisterInfo.aucDetailAddr, acInPut, sizeof(acInPut));
            break;
        }
    }
    while(1)
    {
        printf("9、请设置服务密码\n");
        memset(acInPut,0,sizeof(acInPut));
        scanf("%[^\n]",acInPut);
        setbuf(stdin, NULL);
        if(strlen(acInPut)>6)
        {
            printf("输入格式错误\n");
        }
        else
        {
            memcpy(RegisterInfo.aucServicePasswd, acInPut, sizeof(acInPut));
            break;
        }
    }

    stRebodyFirstUEToHSS.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stRebodyFirstUEToHSS.aucIdNumber = 0x0;
    memcpy(stRebodyFirstUEToHSS.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stRebodyFirstUEToHSS.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stRebodyFirstUEToHSS.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stRebodyFirstUEToHSS.aucDstPort = 11111;
    stRebodyFirstUEToHSS.iDataLen = 0;

    memcpy(gs_SimInfo.aucPIN, "1234", 4);
    memcpy(gs_SimInfo.aucPUK, "12345678",8);
    memcpy(gs_SimInfo.aucHSSID, "666",D_NSP_TDYTH_NAAAS_4G_HSSID_LEN);

    memcpy(stRebodyFirstUEToHSS.aucCardID, gs_SimInfo.aucPHONE,D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stRebodyFirstUEToHSS.aucIMSI, gs_SimInfo.aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(stRebodyFirstUEToHSS.aucHSSID, gs_SimInfo.aucHSSID, D_NSP_TDYTH_NAAAS_4G_HSSID_LEN);
    memcpy(stRebodyFirstUEToHSS.aucPIN, gs_SimInfo.aucPIN, 4);
    memcpy(stRebodyFirstUEToHSS.aucPUK, gs_SimInfo.aucPUK, 8);

    memcpy(stRebodyFirstUEToHSS.aucUserId, RegisterInfo.aucUserId, D_NSP_TDYTH_NAAAS_4G_USERTD_LEN);
    stRebodyFirstUEToHSS.aucSex = RegisterInfo.aucSex;
    memcpy(stRebodyFirstUEToHSS.aucIdNum, RegisterInfo.aucIdNum, D_NSP_TDYTH_NAAAS_4G_IdNum_LEN);
    stRebodyFirstUEToHSS.aucKind = RegisterInfo.aucKind;
    memcpy(stRebodyFirstUEToHSS.aucCountry, RegisterInfo.aucCountry, D_NSP_TDYTH_NAAAS_4G_Country_LEN);
    memcpy(stRebodyFirstUEToHSS.aucProvince, RegisterInfo.aucProvince, D_NSP_TDYTH_NAAAS_4G_Province_LEN);
    memcpy(stRebodyFirstUEToHSS.aucCity, RegisterInfo.aucCity, D_NSP_TDYTH_NAAAS_4G_City_LEN);
    memcpy(stRebodyFirstUEToHSS.aucDetailAddr, RegisterInfo.aucDetailAddr,D_NSP_TDYTH_NAAAS_4G_Addr_LEN);
    memcpy(stRebodyFirstUEToHSS.aucServicePasswd, RegisterInfo.aucServicePasswd, D_NSP_TDYTH_NAAAS_4G_SerPWD_LEN);

    memcpy(stRebodyFirstUEToHSS.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("注册请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stRebodyFirstUEToHSS, D_NSP_TDYTH_NAAAS_4G_ReBodyFirstUEToHSS_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}


/*
 * 功能：接受前端注册消息
 * 参数：      
 * 返回值：    
 *           < 0   失败
 *           >=0   成功
 */
int NSP_TDYTH_4G_Register_FrontInfo(unsigned char *pcRecvBuf)
{   
    unsigned char aucBuff[D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN];

    memset(aucBuff, 0, sizeof(aucBuff));

    memcpy(aucBuff, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_DaemonControl_LEN, D_NSP_TDYTH_NAAAS_4G_RegisterInfo_LEN);
    St_NSP_TDYTH_NAAAS_4G_RegisterInfo *Info = (St_NSP_TDYTH_NAAAS_4G_RegisterInfo *) aucBuff;

    memcpy(RegisterInfo.aucUserId, Info->aucUserId, D_NSP_TDYTH_NAAAS_4G_USERTD_LEN);
    memcpy(RegisterInfo.aucIdNum, Info->aucIdNum, D_NSP_TDYTH_NAAAS_4G_IdNum_LEN);
    RegisterInfo.aucSex = Info->aucSex;
    memcpy(RegisterInfo.aucDetailAddr, Info->aucDetailAddr, D_NSP_TDYTH_NAAAS_4G_Addr_LEN);
    RegisterInfo.aucKind = Info->aucKind;

    printf("UserId:%s\n", RegisterInfo.aucUserId);
    printf("IdNum:%s\n", RegisterInfo.aucIdNum);
    printf("Sex:%c\n", RegisterInfo.aucSex);
    printf("Addr:%s\n", RegisterInfo.aucDetailAddr);
    printf("Kind:%c\n", RegisterInfo.aucKind);

    return 0;
}

/*
 * 功能：将前端传来的注册信息存入数据包(前端)
 * 参数： @xxx [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_Register_Front_transform(char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS stRebodyFirstUEToHSS;


    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x00;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stRebodyFirstUEToHSS.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stRebodyFirstUEToHSS.aucIdNumber = 0x0;
    memcpy(stRebodyFirstUEToHSS.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stRebodyFirstUEToHSS.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stRebodyFirstUEToHSS.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stRebodyFirstUEToHSS.aucDstPort = 11111;
    stRebodyFirstUEToHSS.iDataLen = 0;
    memcpy(stRebodyFirstUEToHSS.aucIMSI, gs_SimInfo.aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(stRebodyFirstUEToHSS.aucUserId, RegisterInfo.aucUserId, D_NSP_TDYTH_NAAAS_4G_USERTD_LEN);
    memcpy(stRebodyFirstUEToHSS.aucIdNum, RegisterInfo.aucIdNum, D_NSP_TDYTH_NAAAS_4G_IdNum_LEN);
    stRebodyFirstUEToHSS.aucSex = RegisterInfo.aucSex;
    memcpy(stRebodyFirstUEToHSS.aucDetailAddr, RegisterInfo.aucDetailAddr, D_NSP_TDYTH_NAAAS_4G_Addr_LEN);
    stRebodyFirstUEToHSS.aucKind = RegisterInfo.aucKind;
    memcpy(stRebodyFirstUEToHSS.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("注册请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stRebodyFirstUEToHSS, D_NSP_TDYTH_NAAAS_4G_ReBodyFirstUEToHSS_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/*
 * 功能：UE进行GPS定位
 * 参数：  
 * 返回值：  0   成功
 */
// int NSP_TDYTH_4G_Gps_Function()
// {
//     char mmeOrder[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];   //从键盘接收指令
//     printf("输入exit退出此功能，任意键为开始或重发数据！\n");
//     while(1)
//     {
//         memset(mmeOrder, 0 ,sizeof(mmeOrder));
//         fgets(mmeOrder, sizeof(mmeOrder), stdin);
//         if (strcmp(mmeOrder, "exit\n") == 0)
//         {
//             printf("退出！\n");            //exit为退出指令
//             break;
//         }
//         system("http --verify=no https://api.ip.sb/geoip");
//         printf_dae_uesend("设备定位完毕，目前处于西安长安区!\n");
//     }

//     return 0;
//     system("http --verify=no https://api.ip.sb/geoip");
//     printf_dae_uesend("设备定位完毕，目前处于西安长安区!\n");
//     return 0;
// }


/*
 * 功能：UE进行GPS定位
 * 参数：  
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_Gps_Function() 
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    system("http --verify=no https://api.ip.sb/geoip -o /tmp/gps.json");
    FILE *fp;
    long len;
    char content[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    cJSON *json;
    if((fp = fopen("/tmp/gps.json","rb")) < 0)
    {
        printf("文件打开失败(请确定路径)!\n");
        return -1;
    }
    
    fseek(fp,0,SEEK_END);
    len = ftell(fp);
    fseek(fp,0,SEEK_SET);
    fread(content,1,len,fp);
    fclose(fp);

    json=cJSON_Parse(content);
    if (!json) 
    {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        return -1;
    }

    strcpy(gs_stGPS.acCity, cJSON_GetObjectItem(json,"city")->valuestring);
    strcpy(gs_stGPS.acCountry, cJSON_GetObjectItem(json,"country")->valuestring);
    strcpy(gs_stGPS.acIp, cJSON_GetObjectItem(json,"ip")->valuestring);
    gs_stGPS.dLlatitude = cJSON_GetObjectItem(json,"latitude")->valuedouble;
    gs_stGPS.dLongitude = cJSON_GetObjectItem(json,"longitude")->valuedouble;

    printf("city:%s\n",gs_stGPS.acCity);
    printf("country:%s\n",gs_stGPS.acCountry);
    printf("ip:%s\n",gs_stGPS.acIp);
    printf("latitude:%f\n",gs_stGPS.dLlatitude);
    printf("longitude:%f\n",gs_stGPS.dLongitude);

    printf_dae_uesend("设备IP地址为：%s\n",gs_stGPS.acIp);
    printf_dae_uesend("国家：%s\n",gs_stGPS.acCountry);
    printf_dae_uesend("城市：%s\n",gs_stGPS.acCity);
    printf_dae_uesend("经度：%f\n",gs_stGPS.dLongitude);
    printf_dae_uesend("纬度：%f\n",gs_stGPS.dLlatitude);

    return 0;
}

/*
 * 功能：附着请求第一步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_Attach_First_UESendToENB(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB stAttBodyFirstUEToeNB;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x03;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stAttBodyFirstUEToeNB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAttBodyFirstUEToeNB.aucIdNumber = 0x0;
    memcpy(stAttBodyFirstUEToeNB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAttBodyFirstUEToeNB.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAttBodyFirstUEToeNB.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stAttBodyFirstUEToeNB.aucDstPort = 11111;
    stAttBodyFirstUEToeNB.iDataLen = 0;

    memcpy(stAttBodyFirstUEToeNB.aucIMSI, gs_SimInfo.aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    memcpy(stAttBodyFirstUEToeNB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("随机接入请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAttBodyFirstUEToeNB, D_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：附着请求第三步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_Attach_Third_UESendToENB(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB stAttBodyThirdUEToeNB;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x03;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x03;

    stAttBodyThirdUEToeNB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAttBodyThirdUEToeNB.aucIdNumber = 0x0;
    memcpy(stAttBodyThirdUEToeNB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAttBodyThirdUEToeNB.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAttBodyThirdUEToeNB.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stAttBodyThirdUEToeNB.aucDstPort = 11111;
    stAttBodyThirdUEToeNB.iDataLen = 0;

    memcpy(stAttBodyThirdUEToeNB.aucIMSI, gs_SimInfo.aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(stAttBodyThirdUEToeNB.aucPDN, "0", D_NSP_TDYTH_NAAAS_4G_PDNTYPE_LEN);
    memcpy(stAttBodyThirdUEToeNB.aucPCO, "0", D_NSP_TDYTH_NAAAS_4G_PCO_LEN);

    memcpy(stAttBodyThirdUEToeNB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("初次附着请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAttBodyThirdUEToeNB, D_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}


/*
 * 功能：IP分配第七步
 * 参数：   
 *          @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IPAllot_Seventh_UESendToENB(char *pcRecvBuf, char *pcSendBuf)
{
    //一直接收数据
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB  stSebodyUEToENB;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE *stIPAllotBodySixthENBToUE = (St_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIPAllotBodySixthENBToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x07;

    stSebodyUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSebodyUEToENB.aucIdNumber = 0x0;
    memcpy(stSebodyUEToENB.aucDstId, stIPAllotBodySixthENBToUE->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stSebodyUEToENB.aucSrcId, stIPAllotBodySixthENBToUE->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSebodyUEToENB.aucDstPort = stIPAllotBodySixthENBToUE->aucDstPort;
    stSebodyUEToENB.aucSrcPort = stIPAllotBodySixthENBToUE->aucSrcPort;
    stSebodyUEToENB.iDataLen = 0;

    stSebodyUEToENB.flag='0';
    memcpy(stSebodyUEToENB.aucTerminalId,"0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("附着消息数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    printf("the send step is %x\n",stHeader.ucStep);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSebodyUEToENB, D_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB_LEN); //数据体填入发送缓冲区

    return 0;
}
/***********************************************************************切换测量***********************************************************************/

/*
 * 功能：发送切换报告
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_HandOver_Measure_First_UESendToSENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB stSendBody;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x0a;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    stSendBody.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSendBody.aucIdNumber = 0x1;
    memcpy(stSendBody.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stSendBody.iDataLen = 0;

    stSendBody.uiCommunityId = 6;

    int X_coordinates,Y_coordinates;
    printf("请输入终端的坐标X:\n");
    scanf("%d",&X_coordinates);
    printf("请输入终端的坐标Y:\n");
    scanf("%d",&Y_coordinates);
    stSendBody.X_coordinates=X_coordinates;
    stSendBody.Y_coordinates=Y_coordinates;
//todo
    memcpy(stSendBody.aucGUTI, "000000000", D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    stSendBody.uiRSCP = 2400;
    stSendBody.uiRSRQ = 2000;  //RSCP,RSRQ由测量、计算得到（此处目前写死）
    stSendBody.usPCI = 300;
    stSendBody.aucBestEnbNum = 2;

    stSendBody.usPCI = ue_Info.bestGNBInfo.uiPCI;
    stSendBody.ucEARFCN = ue_Info.bestGNBInfo.uiEARFCN;
    stSendBody.aucBestEnbNum = ue_Info.bestGNBInfo.uiENBID;
    memcpy(stSendBody.aucDstId,ue_Info.curGNBInfo.aucENBIp, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.aucDstPort = ue_Info.curGNBInfo.uiENBPort;




    memcpy(stSendBody.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("测量报告数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSendBody, St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/*
 * 功能：发送测量报告(前端)
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_HandOver_FrontMeasure_First_UESendToSENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    unsigned char aucBuff[D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN];
    memset(aucBuff, 0, sizeof(aucBuff));

    memcpy(aucBuff, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_DaemonControl_LEN, D_NSP_TDYTH_NAAAS_4G_MeasureInfo_LEN);
    St_NSP_TDYTH_NAAAS_4G_MeasureInfo *Info = (St_NSP_TDYTH_NAAAS_4G_MeasureInfo *) aucBuff;

    for(int i = 0;i<(sizeof(St_NSP_TDYTH_NAAAS_4G_DaemonControl) + sizeof(St_NSP_TDYTH_NAAAS_4G_MeasureInfo));i++)
    {
        printf("%x",pcRecvBuf[i]);
    }
    printf("\n");
    printf("front x = %d\n",Info->X_coordinates);
    printf("front y = %d\n",Info->Y_coordinates);

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB stSendBody;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x0a;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    stSendBody.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSendBody.aucIdNumber = 0x1;
    memcpy(stSendBody.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stSendBody.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stSendBody.aucDstPort = 11111;
    stSendBody.iDataLen = 0;

    stSendBody.uiCommunityId = 6;

    stSendBody.X_coordinates=Info->X_coordinates;
    stSendBody.Y_coordinates=Info->Y_coordinates;

    stSendBody.uiRSCP = 2400;
    stSendBody.uiRSRQ = 2000;  //RSCP,RSRQ由测量、计算得到（此处目前写死）
    stSendBody.usPCI = ue_Info.bestGNBInfo.uiPCI;
    stSendBody.ucEARFCN = ue_Info.bestGNBInfo.uiEARFCN;



    memcpy(stSendBody.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("测量报告数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSendBody, St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/*
 * 功能：X2切换第一步
 * 参数：   
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：    
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_HandOver_First_UESendToSENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    if((iRet = NSP_TDYTH_4G_UE_Measure()) < D_NSP_TDYTH_NAAAS_4G_Measure_Threshold)
    {
        printf("测量结果未达到发送切换报告要求！");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodyFirstUEToSENB stHOBodyFirstUEToSENB;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    stHOBodyFirstUEToSENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stHOBodyFirstUEToSENB.aucIdNumber = 0x1;
    memcpy(stHOBodyFirstUEToSENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stHOBodyFirstUEToSENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stHOBodyFirstUEToSENB.iDataLen = 0;

    stHOBodyFirstUEToSENB.uiCommunityId = 6;
    stHOBodyFirstUEToSENB.uiRSCP = 2400;
    stHOBodyFirstUEToSENB.uiRSRQ = 2000;  //RSCP,RSRQ由测量、计算得到（此处目前写死）

    memcpy(stHOBodyFirstUEToSENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("测量报告数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stHOBodyFirstUEToSENB, D_NSP_TDYTH_NAAAS_4G_HandOverBodyFirstUEToSENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：X2切换第6步
 * 参数：   
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：    
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_HandOver_Sixth_UESendToTENB(char *pcRecvBuf, char *pcSendBuf)
{
    //todo Kasme, KeNB 应该是保存有的，调数据库查询
    unsigned char aucKasme[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];
    memcpy(aucKasme, "00000000000000", D_NSP_TDYTH_NAAAS_4G_KASME_LEN);
    unsigned char aucKenb[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    memcpy(aucKenb,"000000000000000000", D_NSP_TDYTH_NAAAS_4G_KENB_LEN);

    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_HandOverBodyThirdTENBToSENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_N2_BodyFourthSENBToUE *pstRecvBody = (St_NSP_TDYTH_NAAAS_4G_N2_BodyFourthSENBToUE*)auchobody;

    int iRet;
    int i = (int)pstRecvBody->aucNCC;
    char tmpKeNB[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    char newKeNB[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    if(i == 0){
        if ((iRet = NSP_TDYTH_4G_gen_Kenb(aucKenb, &ue_Info.bestGNBInfo.uiPCI, &ue_Info.bestGNBInfo.uiEARFCN, tmpKeNB)) != 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb1 err", __LINE__, errno, strerror(errno));
            return -1;
        }
        if ((iRet = NSP_TDYTH_4G_gen_Kenb3(tmpKeNB, &ue_Info.bestGNBInfo.uiPCI, &ue_Info.bestGNBInfo.uiEARFCN, newKeNB)) != 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb3 err", __LINE__, errno, strerror(errno));
            return -1;
        }

        keypair[++i].NCC = i;

        if ((iRet = NSP_TDYTH_4G_gen_NH1(aucKasme,aucKenb, keypair[i].NH) != 0)){
            printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb3 err", __LINE__, errno, strerror(errno));
            return -1;
        }
        // NH第二轮计算
        keypair[++i].NCC = i;
        if ((iRet = NSP_TDYTH_4G_gen_X2_NH2(aucKasme, keypair[i - 1].NH, keypair[i].NH) != 0)) {
            printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb3 err", __LINE__, errno, strerror(errno));
            return -1;
        }
    }
    if (i > 0){
        if ((iRet = NSP_TDYTH_4G_gen_Kenb(keypair[i].NH, &ue_Info.bestGNBInfo.uiPCI, &ue_Info.bestGNBInfo.uiEARFCN, tmpKeNB)) != 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb1 err", __LINE__, errno, strerror(errno));
            return -1;
        }
        if ((iRet = NSP_TDYTH_4G_gen_Kenb3(tmpKeNB, &ue_Info.bestGNBInfo.uiPCI, &ue_Info.bestGNBInfo.uiEARFCN, newKeNB)) != 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb3 err", __LINE__, errno, strerror(errno));
            return -1;
        }

        keypair[++i].NCC = i;

        if ((iRet = NSP_TDYTH_4G_gen_X2_NH2(aucKasme, keypair[i - 1].NH, keypair[i].NH) != 0)) {
            printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb3 err", __LINE__, errno, strerror(errno));
            return -1;
        }

    }
    //去附着流程
    if((iRet = NSP_TDYTH_4G_UE_Deattach_Function()) != 0)
    {
        printf("去附着失败！");
        return -1;
    }else{
        printf("去附着成功！");
    }
    printf("随机接入成功！"); //此处应该添加随机接入流程函数

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodySixthUEToTENB stHOBodySixthUEToTENB;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x06;

    stHOBodySixthUEToTENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stHOBodySixthUEToTENB.aucIdNumber = 0x1;
    memcpy(stHOBodySixthUEToTENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stHOBodySixthUEToTENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stHOBodySixthUEToTENB.iDataLen = 0;

    stHOBodySixthUEToTENB.ucResponese = 0X01;

    memcpy(stHOBodySixthUEToTENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stHOBodySixthUEToTENB, D_NSP_TDYTH_NAAAS_4G_HandOverBodySixthUEToTENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/***********************************************************************S1切换***********************************************************************/
/*
 * 功能：S1切换第一步
 * 参数：   
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：    
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_First_UESendToSENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFirstUEToSENB stS1HOBodyFirstUEToSENB;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    stS1HOBodyFirstUEToSENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyFirstUEToSENB.aucIdNumber = 0x1;
    memcpy(stS1HOBodyFirstUEToSENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyFirstUEToSENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyFirstUEToSENB.iDataLen = 0;

    stS1HOBodyFirstUEToSENB.uiCommunityId = 6;
    stS1HOBodyFirstUEToSENB.uiRSCP = 2400;
    stS1HOBodyFirstUEToSENB.uiRSRQ = 2000;  //RSCP,RSRQ由测量、计算得到（此处目前写死）
    stS1HOBodyFirstUEToSENB.usPCI = 300;

    memcpy(stS1HOBodyFirstUEToSENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("测量报告数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyFirstUEToSENB, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFirstUEToSENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：S1切换第9步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Ninth_UESendToTENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    unsigned char aucKenb1[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucNH1[D_NSP_TDYTH_NAAAS_4G_NH_LEN];
    unsigned char aucNH2[D_NSP_TDYTH_NAAAS_4G_NH_LEN];
    unsigned char aucKasme[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];
    unsigned char aucPCI[D_NSP_TDYTH_NAAAS_4G_PCI_LEN];
    unsigned char aucEARFCN[D_NSP_TDYTH_NAAAS_4G_EARFCN_LEN];
    unsigned char aucKenb2[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucKenb3[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucKupenc[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucKrrcint[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucKrrcenc[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];

    if((iRet = NSP_TDYTH_4G_UE_Deattach_Function()) != 0)
    {
        printf("去附着失败！");
        return -1;
    }else{
        printf("去附着成功！");
    }
    printf("随机接入成功！"); //此处应该添加随机接入流程函数
    
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE *stS1HOBodySixthSENBToUE = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE*)auchobody;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB stS1HOBodyNinthUEToTENB;
    unsigned char aucKenb[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];

    memcpy(aucKenb,"000000000000000000", D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    memcpy(aucKasme, "00000000000000", D_NSP_TDYTH_NAAAS_4G_KASME_LEN);
    printf("开始生成密钥！\n");

    if ((iRet = NSP_TDYTH_4G_gen_Kenb(aucKenb, &ue_Info.curGNBInfo.uiPCI, &ue_Info.curGNBInfo.uiEARFCN, aucKenb1)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb1 err", __LINE__, errno, strerror(errno));
        return -1;
    }
    NSP_TDYTH_4G_print("计算的KeNB*为：\n",aucKenb1,D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    if ((iRet = NSP_TDYTH_4G_gen_Kenb2(aucKasme, aucKenb1, aucKenb2)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb2 err", __LINE__, errno, strerror(errno));
        return -1;
    }
    NSP_TDYTH_4G_print("计算的KeNB*+为：\n",aucKenb2,D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    if ((iRet = NSP_TDYTH_4G_gen_S1_NH2(aucKasme, aucKenb2, aucNH1)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_NH2 err", __LINE__, errno, strerror(errno));
        return -1;
    }
    if ((iRet = NSP_TDYTH_4G_gen_Kenb3(aucKenb2, &ue_Info.bestGNBInfo.uiPCI, &ue_Info.bestGNBInfo.uiEARFCN, aucKenb3)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb3 err", __LINE__, errno, strerror(errno));
        return -1;
    }
    NSP_TDYTH_4G_print("计算的KeNB**为：\n",aucKenb3,D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    if ((iRet = NSP_TDYTH_4G_gen_Kupenc(aucKenb3, aucKupenc)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kupenc err", __LINE__, errno, strerror(errno));
        return -1;
    }
    if ((iRet = NSP_TDYTH_4G_gen_Krrcint(aucKenb3, aucKrrcint)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Krrcint err", __LINE__, errno, strerror(errno));
        return -1;
    }
    if ((iRet = NSP_TDYTH_4G_gen_Krrcenc(aucKenb3, aucKrrcenc)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Krrcenc err", __LINE__, errno, strerror(errno));
        return -1;
    }

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x09;

    stS1HOBodyNinthUEToTENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyNinthUEToTENB.aucIdNumber = 0x1;
    memcpy(stS1HOBodyNinthUEToTENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyNinthUEToTENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyNinthUEToTENB.iDataLen = 0;

    stS1HOBodyNinthUEToTENB.ucResponese = 0x01;

    memcpy(stS1HOBodyNinthUEToTENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyNinthUEToTENB, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/***********************************************************************站内切换***********************************************************************/
/*
 * 功能：     Intra切换第1步
 * 参数：   
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：    
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_Intra_HandOver_First_UESendToENB(char *pcRecvBuf, char *pcSendBuf)
{
    // unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    // memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE_LEN);
    // St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE *stS1HOBodySixthSENBToUE = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE)auchobody;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB stIntraHOBodyFirstUEToENB;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x07;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    stIntraHOBodyFirstUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIntraHOBodyFirstUEToENB.aucIdNumber = 0x1;
    memcpy(stIntraHOBodyFirstUEToENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIntraHOBodyFirstUEToENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIntraHOBodyFirstUEToENB.iDataLen = 0;

    stIntraHOBodyFirstUEToENB.uiCommunityId = 6;
    stIntraHOBodyFirstUEToENB.uiRSCP = 2400;
    stIntraHOBodyFirstUEToENB.uiRSRQ = 2000;  //RSCP,RSRQ由测量、计算得到（此处目前写死）

    memcpy(stIntraHOBodyFirstUEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("测量报告数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIntraHOBodyFirstUEToENB, D_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/*
 * 功能：     Intra切换第3步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_Intra_HandOver_Third_UESendToENB(char *pcRecvBuf, char *pcSendBuf)
{
     unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE_LEN);
     St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE *stIntraHandOverBodySecondENBToUE = (St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE *)auchobody;

    if(NSP_TDYTH_4G_verification_timestamp(stIntraHandOverBodySecondENBToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyThirdUEToENB stIntraHOBodyThirdUEToENB;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x07;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x03;

    stIntraHOBodyThirdUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIntraHOBodyThirdUEToENB.aucIdNumber = 0x1;
    memcpy(stIntraHOBodyThirdUEToENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIntraHOBodyThirdUEToENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIntraHOBodyThirdUEToENB.iDataLen = 0;

    stIntraHOBodyThirdUEToENB.ucResponese = 0X01;

    memcpy(stIntraHOBodyThirdUEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("切换完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIntraHOBodyThirdUEToENB, D_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyThirdUEToENB_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/***********************************************************************MME连接中断***********************************************************************/
/*
 * 功能：连接中断第三步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_LinkageInterrupt_Third_UE(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_LinkageInterruptSecondENBToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB *stLiBodyFirstMMEToUE = (St_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stLiBodyFirstMMEToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    printf("收到MME中断请求，断开与MME连接！\n");

    return 0;
}
/***********************************************************************MME恢复连接***********************************************************************/
/*
 * 功能：连接恢复第三步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_LinkageRecover_Third_UE(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_LinkageRecoverSecondENBToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB *stReBodyFirstMMEToUE = (St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stReBodyFirstMMEToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    printf("收到MME中断请求，恢复与MME连接！\n");

    return 0;
}

/***********************************************************************上网请求***********************************************************************/

/*
 * 功能：上网请求第一步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet_First_UESendToENB(char *pcRecvBuf, char *pcSendBuf , char *pcWeburl, char *pcDnsIp)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB stIpBodyFirstUEToENB;
    unsigned char aucrequest[D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN];

    memset(aucrequest,0,D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN);

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x10;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stIpBodyFirstUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIpBodyFirstUEToENB.aucIdNumber = 0x0;
    memcpy(stIpBodyFirstUEToENB.aucDstId, "1111111111111111", D_NSP_TDYTH_NAAAS_4G_ID_LEN); // 1111111111111111互联网
    memcpy(stIpBodyFirstUEToENB.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIpBodyFirstUEToENB.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stIpBodyFirstUEToENB.aucDstPort = 80;
    stIpBodyFirstUEToENB.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB);

    stIpBodyFirstUEToENB.uiDRBId = 0x00000001;
    memcpy(stIpBodyFirstUEToENB.aucDstIp, pcDnsIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyFirstUEToENB.aucSrcIp, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);  //此处ip应为PGW分配IP
    memcpy(stIpBodyFirstUEToENB.aucGUTI, "0000000000", D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);      //此处GUTI应该从存储中拿取

    sprintf(aucrequest, "GET %s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n", "/index.html", pcWeburl, 0);
    printf("-------------\n%s\n",aucrequest);

    memcpy(stIpBodyFirstUEToENB.aucRequest, aucrequest, D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN);

    memcpy(stIpBodyFirstUEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("上网请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIpBodyFirstUEToENB, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB)); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：上网请求第9步获取内容
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet9(char *pcRecvBuf)
{
    //system("firefox /tmp/web.html");
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE));

    St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE *stIpBodyEighthENBToUE = (St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIpBodyEighthENBToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    printf("响应结果：\n%s\n", stIpBodyEighthENBToUE->aucResult);

    return 0;
}


static char gs_cue_BearerID;
/*
 * 功能：专有承载建立第六步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Sixth_UEToENB(char *pcRecvBuf, char *pcSendBuf)
{
    //也开始重新构造头部
    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE));

    St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE *stBearerSetUpFifthENBToUE = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpFifthENBToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB stBearerSetUpSixthUEToENB;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x06;

    stBearerSetUpSixthUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpSixthUEToENB.aucIdNumber = 0x0;
    memcpy(stBearerSetUpSixthUEToENB.aucDstId, D_NSP_TDYTH_NAAAS_PCRF_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);//目的ID是什么？
    memcpy(stBearerSetUpSixthUEToENB.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpSixthUEToENB.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stBearerSetUpSixthUEToENB.aucDstPort = D_NSP_TDYTH_NAAAS_PCRF_PORT;
    stBearerSetUpSixthUEToENB.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME);

    gs_cue_BearerID=stBearerSetUpFifthENBToUE->ucBearerID;//在UE里面设置一个全局变量，第八步发送的时候用

    memcpy(stBearerSetUpSixthUEToENB.aucTerminalId, stBearerSetUpFifthENBToUE->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpSixthUEToENB, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB));
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：专有承载建立第八步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Eighth_UEToENB(char* pcRecvBuf, char* pcSendBuf)
{
    //重新构造头部
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB stBearerSetUpEighthUEToENB;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x0b;//暂时认为0x0b是专有承载的建立
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x08;

    stBearerSetUpEighthUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpEighthUEToENB.aucIdNumber = 0x0;
    memcpy(stBearerSetUpEighthUEToENB.aucDstId, D_NSP_TDYTH_NAAAS_PCRF_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);//也不太清楚是不是PCRF
    memcpy(stBearerSetUpEighthUEToENB.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpEighthUEToENB.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stBearerSetUpEighthUEToENB.aucDstPort = D_NSP_TDYTH_NAAAS_PCRF_PORT;
    stBearerSetUpEighthUEToENB.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB);

    stBearerSetUpEighthUEToENB.ucBearerID = gs_cue_BearerID;

    memcpy(stBearerSetUpEighthUEToENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);//还不确定里面写什么
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpEighthUEToENB, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：注册整体流程功能函数(后台)
 * 参数：      
 * 返回值：    
 *           < 0   失败
 *           >=0   成功
 */
int NSP_TDYTH_4G_BackRegister_Function()
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        return -1;
    }

    if(gs_iIsReadCard == 0)
    {
        printf("设备未读取SIM卡信息，请读取！\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    int iRet;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    // UE
    int iSockfdHSS = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in HSSSockaddr;
    bzero(&HSSSockaddr, sizeof(HSSSockaddr));
    HSSSockaddr.sin_family = AF_INET;
    HSSSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_HSS_PORT);
    HSSSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_HSS_IP);

    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfdHSS,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfdHSS);

    // bind，成功返回0，出错返回-1
    if (bind(iSockfdHSS, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    int sin_len = sizeof(HSSSockaddr);

    /****************************************注册流程，属于以前放在while1循环中的功能******************************************/
    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    NSP_TDYTH_4G_Register_User_Send(cSendBuf);
    iRet = sendto(iSockfdHSS, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &HSSSockaddr, sin_len);
    if(iRet > 0)
    {
        printf("UE->HSS 注册请求发送成功!\n");
        printf_dae_uesend("UE->HSS 注册请求发送成功!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfdHSS);

    return 0;
}

/*
 * 功能：注册整体流程功能函数(前端)
 * 参数：      
 * 返回值：    
 *           < 0   失败
 *           >=0   成功
 */
int NSP_TDYTH_4G_FrontRegister_Function(unsigned char *pcRecvBuf)
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    if(gs_iIsReadCard == 0)
    {
        printf("设备未读取SIM卡信息，请读取！\n");
        printf_dae_uesend("设备未读取SIM卡信息，请读取！\n");
        return -1;
    }

    NSP_TDYTH_4G_Register_FrontInfo(pcRecvBuf);

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    int iRet;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];


    // UE
    int iSockfdHSS = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in HSSSockaddr;
    bzero(&HSSSockaddr, sizeof(HSSSockaddr));
    HSSSockaddr.sin_family = AF_INET;
    HSSSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_HSS_PORT);
    HSSSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_HSS_IP);

    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfdHSS,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfdHSS);

    // bind，成功返回0，出错返回-1
    if (bind(iSockfdHSS, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    int sin_len = sizeof(HSSSockaddr);

    /****************************************注册流程，属于以前放在while1循环中的功能******************************************/
    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    NSP_TDYTH_4G_Register_Front_transform(cSendBuf);
    iRet = sendto(iSockfdHSS, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &HSSSockaddr, sin_len);
    if(iRet > 0)
    {
        printf("UE->HSS 注册请求发送成功!\n");
        printf_dae_uesend("UE->HSS 注册请求发送成功!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }
    close(iSockfdHSS);
    return 0;
}



/*
 * 功能： UE附着完整功能
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_Attach_Function()
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    if(gs_iIsRegis == 0)
    {
        printf("终端UE还未注册,请注册！\n");
        printf_dae_uesend("终端UE还未注册,请注册！\n");
        return -1;
    }

    if(gs_iENBnum == -1)
    {
        printf("暂无驻留基站，请等待基站驻留后重试！\n");
        printf_dae_uesend("暂无驻留基站，请等待基站驻留后重试！\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    int iRet;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // UE
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    // ENB
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(gs_astEnbList[gs_iENBnum].uiENBPort);
    ENBSockaddr.sin_addr.s_addr = inet_addr(gs_astEnbList[gs_iENBnum].aucENBIp);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfd);

    if (bind(iSockfd, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
        //exit(1);
    }

    int sin_len = sizeof(ENBSockaddr);

    /****************************************附着流程，属于以前放在while1循环中的功能******************************************/
    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    NSP_TDYTH_4G_Attach_First_UESendToENB(cRecvBuf,cSendBuf);
    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENBSockaddr, sin_len);
    if(iRet > 0)
    {
        printf("UE->ENB 随机接入请求发送成功!\n");
        printf_dae_uesend("UE->ENB 随机接入请求发送成功!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
    return 0;
}

/*
 * 功能： UE发送测量报告
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_UE_MeasureReport_Function()
{

    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        return -1;
    }

    if(gs_iIsRegis == 0)
    {
        printf("终端UE还未注册,请注册！\n");
        return -1;
    }

    int iRet;
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // ENB1(SENB)
    struct sockaddr_in ENB1Sockaddr;
    bzero(&ENB1Sockaddr, sizeof(ENB1Sockaddr));
    ENB1Sockaddr.sin_family = AF_INET;
    /*ENB1Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENB1Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);*/

    ENB1Sockaddr.sin_port = htons(ue_Info.curGNBInfo.uiENBPort);
    ENB1Sockaddr.sin_addr.s_addr = inet_addr(ue_Info.curGNBInfo.aucENBIp);

    // UE
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfd);

    if (bind(iSockfd, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
        //exit(1);
    }

    NSP_TDYTH_4G_HandOver_Measure_First_UESendToSENB(cRecvBuf,cSendBuf);

    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr));
    if(iRet > 0)
    {
        printf("------------------------step1.UE#Sender ->SENB:0x01------------------------\n");
        printf("UE->SENB 切换测量报告发送成功 !\n");
        printf_dae_uesend("UE->SENB 切换测量报告发送成功!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
    return 0;
}

/*int update_best_gNB(UE_INFO* ueInfo, gNB_INFO* newGNBInfo) {
    int ret = 0;
    if(ueInfo->curGNBInfo.eNBID == 0) {
        // 当前无基站，直接为地址列表赋值
        memcpy(&ueInfo->curGNBInfo, newGNBInfo, sizeof(gNB_INFO));
        memcpy(&ueInfo->bestGNBInfo, newGNBInfo, sizeof(gNB_INFO));
        ret = 2;
    }
    else {
        // 比较新基站与当前基站
        if(compute_distance(ueInfo->E, ueInfo->N, newGNBInfo->E, newGNBInfo->N) <
           compute_distance(ueInfo->E, ueInfo->N, ueInfo->curGNBInfo.E, ueInfo->curGNBInfo.N) ) {
            if(ueInfo->bestGNBInfo.gNBID == 0 ||
               compute_distance(ueInfo->E, ueInfo->N, newGNBInfo->E, newGNBInfo->N) <
               compute_distance(ueInfo->E, ueInfo->N, ueInfo->bestGNBInfo.E, ueInfo->bestGNBInfo.N) ) {
                // 更新最优基站
                memcpy(&ueInfo->bestGNBInfo, newGNBInfo, sizeof(gNB_INFO));
                ret = 1;
            }
        }
    }
    return ret;
}*/





/*
 * 功能：  根据接收到的ENB广播包对ENB信息进行更新或添加
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_Broadcast_Receive(char *pcRecvBuf)
{
    int i, flag = 1;
    unsigned char broadcastbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(broadcastbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_BroadcastBody_LEN);

    St_NSP_TDYTH_NAAAS_4G_BroadcastBody *broadcastInfo = (St_NSP_TDYTH_NAAAS_4G_BroadcastBody *)broadcastbody;
    //更新UE存储的ENB信息
    for(i=0; i <= gs_iEnblist_last; i++)
    {
        if(gs_astEnbList[i].uiENBID == broadcastInfo->uiENBID)
        {
            gs_astEnbList[i].uiPCI = broadcastInfo->uiPCI;
            gs_astEnbList[i].uiEARFCN = broadcastInfo->uiEARFCN;
            gs_astEnbList[i].iRSRP = broadcastInfo->iRSRP;
            gs_astEnbList[i].iCellRP = broadcastInfo->iCellRP;
            gs_astEnbList[i].iQrxlevmin = broadcastInfo->iQrxlevmin;
            memcpy(gs_astEnbList[i].aucTAI, broadcastInfo->aucTAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);

            gs_astEnbList[i].iIsUpdate = 1;

            gs_astEnbList[i].uiENBPort = broadcastInfo->uiENBPort;
            memcpy(gs_astEnbList[i].aucENBIp, broadcastInfo->aucENBIp, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
            flag = 0;
        }
    }
    //若发现没有此eNB_ID，则添加ENB信息
    if(flag)
    {
        gs_iEnblist_last++;
        gs_astEnbList[gs_iEnblist_last].uiENBID = broadcastInfo->uiENBID;
        gs_astEnbList[gs_iEnblist_last].uiPCI = broadcastInfo->uiPCI;
        gs_astEnbList[gs_iEnblist_last].uiEARFCN = broadcastInfo->uiEARFCN;
        gs_astEnbList[gs_iEnblist_last].iRSRP = broadcastInfo->iRSRP;
        gs_astEnbList[gs_iEnblist_last].iCellRP = broadcastInfo->iCellRP;
        gs_astEnbList[gs_iEnblist_last].iQrxlevmin = broadcastInfo->iQrxlevmin;
        memcpy(gs_astEnbList[i].aucTAI, broadcastInfo->aucTAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);

        gs_astEnbList[gs_iEnblist_last].iIsBAN = 0;
        gs_astEnbList[gs_iEnblist_last].iIsUpdate = 1;

        gs_astEnbList[gs_iEnblist_last].uiENBPort = broadcastInfo->uiENBPort;
        memcpy(gs_astEnbList[gs_iEnblist_last].aucENBIp, broadcastInfo->aucENBIp, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  显示当前和最优ENB信息
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_EnbHandover_Display()
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    int i;

    printf("/////////////////////////////////\n");
    printf("///////   当前驻留基站:   ///////\n");
    if(gs_iENBnum >= 0)
    {
        printf("/////////////////////////////////\n");
        printf("//////// eNB Num:%7d ////////\n", gs_iENBnum+1);
        printf("//////// eNB ID:%8d ////////\n", ue_Info.curGNBInfo.uiENBID);
        printf("//////// PCI:%11d ////////\n", ue_Info.curGNBInfo.uiPCI);
        printf("//////// TAI:%11s ////////\n", ue_Info.curGNBInfo.aucTAI);
        printf("//////// isBAN:%9s ////////\n", ue_Info.curGNBInfo.iIsBAN ? "Yes" : "No");
        printf("//////// RSRP:%10d ////////\n", ue_Info.curGNBInfo.iRSRP);
        printf("//////// EARFCN:%8d ////////\n", ue_Info.curGNBInfo.uiEARFCN);
        printf("//////// CellRP:%8d ////////\n", ue_Info.curGNBInfo.iCellRP);
        printf("//////// Qrxlevmin:%5d ////////\n", ue_Info.curGNBInfo.iQrxlevmin);
        printf("//////// Port:%10d ////////\n", ue_Info.curGNBInfo.uiENBPort);
        printf("//////// ID:%12s ////////\n", ue_Info.curGNBInfo.aucENBIp);
        printf("/////////////////////////////////\n\n");
    }else{
        printf("/////////////////////////////////\n");
        printf("////////   当前暂无驻留    ////////\n");
        printf("/////////////////////////////////\n\n");
    }

    printf("/////////////////////////////////\n");
    printf("///////   当前最优基站:   ///////\n");
    if(gs_iBestENBnum >= 0)
    {
        printf("/////////////////////////////////\n");
        printf("//////// eNB Num:%7d ////////\n", gs_iBestENBnum+1);
        printf("//////// eNB ID:%8d ////////\n", ue_Info.bestGNBInfo.uiENBID);
        printf("//////// PCI:%11d ////////\n", ue_Info.bestGNBInfo.uiPCI);
        printf("//////// TAI:%11s ////////\n", ue_Info.bestGNBInfo.aucTAI);
        printf("//////// isBAN:%9s ////////\n", ue_Info.bestGNBInfo.iIsBAN ? "Yes" : "No");
        printf("//////// RSRP:%10d ////////\n", ue_Info.bestGNBInfo.iRSRP);
        printf("//////// EARFCN:%8d ////////\n", ue_Info.bestGNBInfo.uiEARFCN);
        printf("//////// CellRP:%8d ////////\n", ue_Info.bestGNBInfo.iCellRP);
        printf("//////// Qrxlevmin:%5d ////////\n", ue_Info.bestGNBInfo.iQrxlevmin);
        printf("//////// Port:%10d ////////\n", ue_Info.bestGNBInfo.uiENBPort);
        printf("//////// ID:%12s ////////\n", ue_Info.bestGNBInfo.aucENBIp);
        printf("/////////////////////////////////\n\n");
    }else{
        printf("/////////////////////////////////\n");
        printf("///////   当前暂无最优基站    //////\n");
        printf("/////////////////////////////////\n\n");
    }

    if(gs_iENBnum >= 0 && gs_iBestENBnum >= 0)
    {
        if(ue_Info.curGNBInfo.iRSRP < ue_Info.bestGNBInfo.iRSRP){
            printf("\n/////////////////////////////////\n");
            printf("/// The UE need to handover. ////\n");
            printf("/////////////////////////////////\n");
        }
    }

    return 0;
}

/*
 * 功能：  显示所有ENB信息
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_EnbInfo_Display()
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    int i;

    printf("/////////////////////////////////\n");
    printf("///////   所有基站信息:   ///////\n");
    printf("/////////////////////////////////\n");
    for(i=0; i <= gs_iEnblist_last; i++)
    {
        printf("/////////////////////////////////\n");
        printf("//////// Enb Num:%7d ////////\n", i+1);
        printf("//////// eNB ID:%8d ////////\n", gs_astEnbList[i].uiENBID);
        printf("//////// PCI:%11d ////////\n", gs_astEnbList[i].uiPCI);
        printf("//////// TAI:%11s ////////\n", gs_astEnbList[i].aucTAI);
        printf("//////// isBAN:%9s ////////\n", gs_astEnbList[i].iIsBAN ? "Yes" : "No");
        printf("//////// RSRP:%10d ////////\n", gs_astEnbList[i].iRSRP);
        printf("//////// EARFCN:%8d ////////\n", gs_astEnbList[i].uiEARFCN);
        printf("//////// CellRP:%8d ////////\n", gs_astEnbList[i].iCellRP);
        printf("//////// Qrxlevmin:%5d ////////\n", gs_astEnbList[i].iQrxlevmin);
        printf("//////// Port:%10d ////////\n", gs_astEnbList[i].uiENBPort);
        printf("//////// ID:%12s ////////\n", gs_astEnbList[i].aucENBIp);
        printf("/////////////////////////////////\n");
    }

    return 0;
}

/*
 * 功能：  输出UE相关参数，方便进行调试
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_UEDebug_Function()
{
    printf("gs_iENBnum:%d\n", gs_iENBnum);
    printf("gs_iBestENBnum:%d\n", gs_iBestENBnum);
    return 0;
}

/*******************************************************************攻击相关******************************************************************/
/*
 * 功能：  根据接收到的拒绝消息禁用ENB
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_AucRefuse_Receive(char *pcRecvBuf)
{
    int i;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AucRefuseBody_LEN);

    St_NSP_TDYTH_NAAAS_4G_AucRefuseBody *stAucRefuseBody = (St_NSP_TDYTH_NAAAS_4G_AucRefuseBody *)aucbody;
    //对拒绝消息中的PCI进行禁用
    for(i=0; i <= gs_iEnblist_last; i++)
    {
        if(gs_astEnbList[i].uiPCI == stAucRefuseBody->iPCI)
        {
            gs_astEnbList[i].iIsBAN = 1;
            printf("////// 完成对相应PCI的禁用 //////\n");
            printf("////// 禁用PCI的值为：%d //////\n", gs_astEnbList[i].uiPCI);
        }
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  对TAU Reject消息的处理
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_TauReject_Receive(char *pcRecvBuf)
{
    int i;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE_LEN);

    St_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE *stTauRejectBody = (St_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE *) aucbody;
    switch (stTauRejectBody->iEMM_Cause) {
        case 0b00000111:
            printf("////////// 4G网络被禁用！//////////\n");
            break;
        case 0b00001000:
            printf("////////// 所有网络被禁用！//////////\n");
            break;
        default:
            printf("EMM Cause Error![EMM Cause=%b]\n", stTauRejectBody->iEMM_Cause);
            break;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/*******************************************************************攻击相关End******************************************************************/

/*
 * 功能： UE发送测量报告(前端)
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_UE_FrontMeasureReport_Function(char *pcRecvBuf)
{ 
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    if(gs_iIsRegis == 0)
    {
        printf("终端UE还未注册,请注册！\n");
        printf_dae_uesend("终端UE还未注册,请注册！\n");
        return -1;
    }

    int iRet;

    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN];

    memset(cSendBuf, 0, sizeof(cSendBuf));

    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // ENB1(SENB)
    struct sockaddr_in ENB1Sockaddr;
    bzero(&ENB1Sockaddr, sizeof(ENB1Sockaddr));
    ENB1Sockaddr.sin_family = AF_INET;
    ENB1Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENB1Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);

    // UE
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfd);

    if (bind(iSockfd, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
        //exit(1);
    }

    NSP_TDYTH_4G_HandOver_FrontMeasure_First_UESendToSENB(pcRecvBuf,cSendBuf);

    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr));
    if(iRet > 0)
    {
        printf("------------------------step1.UE#Sender ->SENB:0x01------------------------\n");
        printf("UE->SENB 切换测量报告发送成功 !\n");
        printf_dae_uesend("UE->SENB 切换测量报告发送成功!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
    return 0;
}

/*****************************************   IMS   ****************************************************************/
/*
 * 功能： IMS
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_IMSRegister_First_UEToENB(char *pcRecvBuf, char *pcSendBuf){
    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x001;
    stHeader.ucReqType = 0x0c;
    stHeader.ucSubType = 0x21;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stSecHeader.aucSrcId,D_NSP_TDYTH_NAAAS_UE_IP ,D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stSecHeader.aucDstId,D_NSP_TDYTH_NAAAS_SCSCF_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    stSecHeader.aucDstPort = D_NSP_TDYTH_NAAAS_SCSCF_PORT;
    stSecHeader.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    memcpy(stSecHeader.aucTerminalId, "0000000000000000",D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    //构建第三层头
    St_NSP_TDYTH_NAAAS_4G_SIPHeader stSipHeader;
    memcpy(stSipHeader.aucFrom, "sip:NSP_TDYTH_NAAAS_4G2@xidian.edu",D_NSP_TDYTH_NAAAS_4GSIP_From_LEN);
    memcpy(stSipHeader.aucTo,"sip:NSP_TDYTH_NAAAS_4G1@xidian.edu",D_NSP_TDYTH_NAAAS_4GSIP_To_LEN);
    memcpy(stSipHeader.aucCall_ID,"123123123123@127.0.0.1",D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN);
    memcpy(stSipHeader.aucCSeq,"12345678",D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN);
    memcpy(stSipHeader.aucVia,"SIP/2.0/UDP 127.0.0.1:16664",D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN);
    stSipHeader.uiMax_Forwards = 70;
    NSP_TDYTH_4G_printSIP(&stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

    //构建包体
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF stImsBodyFirstUEToPCSCF;
    memcpy(stImsBodyFirstUEToPCSCF.aucIMPI, "sip:NSP_TDYTH_NAAAS_4GPriAddr@xidian.edu",D_NSP_TDYTH_NAAAS_4G_IMPI_LEN);
    memcpy(stImsBodyFirstUEToPCSCF.aucIMPU, "sip:NSP_TDYTH_NAAAS_4GPubAddr@xidian.edu",D_NSP_TDYTH_NAAAS_4G_IMPU_LEN);
    memcpy(stImsBodyFirstUEToPCSCF.aucHomeNetworkDomainName, "sip:NSP_TDYTH_NAAAS_4G_HomeAdrr@xidian.edu",D_NSP_TDYTH_NAAAS_4G_HomeNetworkDomainName_LEN);
    memcpy(stImsBodyFirstUEToPCSCF.aucUEIp,D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stImsBodyFirstUEToPCSCF.aucEnd,D_NSP_TDYTH_NAAAS_4G_END, D_NSP_TDYTH_NAAAS_4G_END_LEN);
    stImsBodyFirstUEToPCSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF_LEN;

    printf("IMS-UE第一步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN,
            &stImsBodyFirstUEToPCSCF ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能： IMS注册第十一步
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_IMSRegister_Eleventh_UEToENB(char *pcRecvBuf, char *pcSendBuf){
    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

    memcpy(sipbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeadereader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *)sipbody;

    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +
                    D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE *stIMSRegisterTenthPCSCFToUE = (St_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE *)imsbody;
    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //检查sip头的合法性
    if(gSipHeadereader->uiMax_Forwards == 0 ){
        printf("IMS第十步出错,sip非法");
        return -1;
    }
    //todo
    //检查data合法性
    if(stIMSRegisterTenthPCSCFToUE->iDataLen <= 0){
        printf("IMS第十步出错,数据包长度非法");
        return -1;
    }
    NSP_TDYTH_4G_printSIP(gSipHeadereader,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x001;
    stHeader.ucReqType = 0x0c;
    stHeader.ucSubType = 0x21;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x0b;

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stSecHeader.aucSrcId,D_NSP_TDYTH_NAAAS_UE_IP ,D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stSecHeader.aucDstId,D_NSP_TDYTH_NAAAS_SCSCF_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    stSecHeader.aucDstPort = D_NSP_TDYTH_NAAAS_SCSCF_PORT;
    stSecHeader.aucSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    memcpy(stSecHeader.aucTerminalId, "11111111111111",D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    //构建第三层头
    St_NSP_TDYTH_NAAAS_4G_SIPHeader stSipHeader;
    memcpy(stSipHeader.aucFrom, "sip:NSP_TDYTH_NAAAS_4G2@xidian.edu",D_NSP_TDYTH_NAAAS_4GSIP_From_LEN);
    memcpy(stSipHeader.aucTo,"sip:NSP_TDYTH_NAAAS_4G1@xidian.edu",D_NSP_TDYTH_NAAAS_4GSIP_To_LEN);
    memcpy(stSipHeader.aucCall_ID,"321321321321@127.0.0.1",D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN);//与第一次不一样
    memcpy(stSipHeader.aucCSeq,"11111111",D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN);//与第一次不一样
    memcpy(stSipHeader.aucVia,"SIP/2.0/UDP 127.0.0.1:16664",D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN);
    stSipHeader.uiMax_Forwards = 70 ;

    printf("十一步的SIP头\n");
    NSP_TDYTH_4G_printSIP(&stSipHeader,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

    //构建包体
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF stIMSRegisterEleventhUEToPCSCF;
    memcpy(stIMSRegisterEleventhUEToPCSCF.aucIMPI, stIMSRegisterTenthPCSCFToUE->aucIMPI,D_NSP_TDYTH_NAAAS_4G_IMPI_LEN);
    memcpy(stIMSRegisterEleventhUEToPCSCF.aucIMPU, "sip:NSP_TDYTH_NAAAS_4GPubAddr@xidian.edu",D_NSP_TDYTH_NAAAS_4G_IMPU_LEN);
    memcpy(stIMSRegisterEleventhUEToPCSCF.aucHomeNetworkDomainName, "sip:NSP_TDYTH_NAAAS_4G_HomeAdrr@xidian.edu",D_NSP_TDYTH_NAAAS_4G_HomeNetworkDomainName_LEN);
    memcpy(stIMSRegisterEleventhUEToPCSCF.aucUEIp,D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIMSRegisterEleventhUEToPCSCF.aucEnd,D_NSP_TDYTH_NAAAS_4G_END, D_NSP_TDYTH_NAAAS_4G_END_LEN);
    stIMSRegisterEleventhUEToPCSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF_LEN;
    //Todo
    //有从第10步传过来的AUTN和RAND
    //RES里面要构造东西,    UE通过RAND和ISIM中保存的共享密钥key计算RES。(可以参考接入认证 UE计算RES的方法)
    memcpy(stIMSRegisterEleventhUEToPCSCF.aucRes," 00000000000000000000",D_NSP_TDYTH_NAAAS_4G_RES_LEN);
    //UE 还计算CK 和 IK


    printf("IMS-UE第十一步完成.\n");

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN , &stSipHeader ,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN,
           &stIMSRegisterEleventhUEToPCSCF ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF_LEN);

    memcpy(imsbody, pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +
                    D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

int NSP_TDYTH_4G_IMSRegister_Twenty_first_UEToENB(char *pcRecvBuf, char *pcSendBuf){

    printf("IMS finished!\n");

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/*
 * 功能： TAU流程第1步
 * 参数：
 * 返回值：  0   成功
 *        < 0   失败
 */
int NSP_TDYTH_4G_TAU_First_UEToENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB stTauBodyFirstUEToENB;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x13;
    stHeader.ucSubType = 0x04;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    stTauBodyFirstUEToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyFirstUEToENB.aucDstId, gs_astEnbList[gs_iENBnum].aucENBIp, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyFirstUEToENB.uiDstPort = gs_astEnbList[gs_iENBnum].uiENBPort;
    memcpy(stTauBodyFirstUEToENB.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyFirstUEToENB.uiSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stTauBodyFirstUEToENB.uiDataLen = 0;

    memcpy(stTauBodyFirstUEToENB.aucOldGUTI, "0000000000", D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stTauBodyFirstUEToENB.aucOldTAI, "0000000000", D_NSP_TDYTH_NAAAS_4G_TAI_LEN);   //暂时用“0000000000”代替，因为前面的流程中没有给到UE具体的TAI和GUTI

    memcpy(stTauBodyFirstUEToENB.aucTerminalId, "000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyFirstUEToENB, D_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能： IP上网请求
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_IpNet_Function()
{
    if(gs_iENBnum == -1)
    {
        printf("暂无驻留基站，请等待基站驻留后重试！\n");
        printf_dae_uesend("暂无驻留基站，请等待基站驻留后重试！\n");
        return -1;
    }

    unsigned char aucWebUrl[D_NSP_TDYTH_NAAAS_4G_URL_LEN];
    unsigned char aucDnsIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    if(IpFunction(aucWebUrl) < 0) return 0;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    int iRet;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN];

    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // UE
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    // ENB
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(gs_astEnbList[gs_iENBnum].uiENBPort);
    ENBSockaddr.sin_addr.s_addr = inet_addr(gs_astEnbList[gs_iENBnum].aucENBIp);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfd);

    if (bind(iSockfd, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
        //exit(1);
    }

    int sin_len = sizeof(ENBSockaddr);

    /****************************************IP上网第一步******************************************/
    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    //DNS解析
    if(NSP_TDYTH_NAAAS_4G_ANET_dns(aucWebUrl, aucDnsIp) < 0)
    {
        printf("DNS error for Url\n");
        return -1;
    }

    NSP_TDYTH_4G_IpNet_First_UESendToENB(cRecvBuf,cSendBuf,aucWebUrl,aucDnsIp);
    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENBSockaddr, sin_len);
    if(iRet > 0)
    {
        printf("UE->ENB IP上网第一步发送成功!\n");
        printf("------------------------step1: UE#Sender -> ENB1：0x01------------------------\n");
    }
    else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));//这个地方理论上是要改的
        return -1;
    }

    close(iSockfd);
    return 0;
}

/*
 * 功能：读取SIM卡信息，获得IMSI与KI
 * 参数： 
 * 返回值：  
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_readmessage() 
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }
    int iRet;
   
    unsigned char aucIMSIline[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char aucKIline[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char aucDATAline[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char aucPHONEline[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    memset(aucIMSIline, 0, sizeof(aucIMSIline));
    memset(aucKIline, 0, sizeof(aucKIline));
    memset(aucDATAline, 0, sizeof(aucDATAline));
    memset(aucPHONEline, 0, sizeof(aucPHONEline));
    
    if((iRet = NSP_TDYTH_4G_getfiletxt(D_NSP_TDYTH_NAAAS_4G_SIM1_Location, D_NSP_TDYTH_NAAAS_4G_SIM_ISMILINE,aucIMSIline )) == -1)
    {
        printf("文件打开失败或文件不存在！\n");
        return -1;
    }
    else{
        printf("SIM card begin read !\n");
        printf_dae_uesend("IMSI读取成功！\n");
    }

    sscanf(aucIMSIline, "%*[^=]=%[^\n]", gs_SimInfo.aucIMSI);
    gs_SimInfo.aucIMSI[16]='\n';
    printf("IMSI :%s\n",gs_SimInfo.aucIMSI);
    printf_dae_uesend("IMSI：%s\n",gs_SimInfo.aucIMSI);

    if((iRet = NSP_TDYTH_4G_getfiletxt(D_NSP_TDYTH_NAAAS_4G_SIM1_Location, D_NSP_TDYTH_NAAAS_4G_SIM_KILINE, aucKIline)) == -1)
    {
        printf("文件打开失败或文件不存在！\n");
        return -1;
    }
    else{
        printf("SIM card begin read !\n");
        printf_dae_uesend("KI读取成功！\n");
    }

    sscanf(aucKIline, "%*[^=]=%[^\n]", gs_SimInfo.aucKI);
    gs_SimInfo.aucKI[33]='\n';
    printf("KI :%s\n",gs_SimInfo.aucKI);
    printf_dae_uesend("KI：%s\n",gs_SimInfo.aucKI);

    if((iRet = NSP_TDYTH_4G_getfiletxt(D_NSP_TDYTH_NAAAS_4G_SIM1_Location, D_NSP_TDYTH_NAAAS_4G_SIM_DATELINE, aucDATAline)) == -1)
    {
        printf("文件打开失败或文件不存在！\n");
        return -1;
    }
    else{
        printf("SIM card begin read !\n");
        printf_dae_uesend("DATE CARD 读取成功！\n");
    }

    sscanf(aucDATAline, "%*[^=]=%[^\n]", gs_SimInfo.aucDATE);
    printf("DATE :%s\n",gs_SimInfo.aucDATE);
    printf_dae_uesend("DATE：%s\n",gs_SimInfo.aucDATE);

    if((iRet = NSP_TDYTH_4G_getfiletxt(D_NSP_TDYTH_NAAAS_4G_SIM1_Location, D_NSP_TDYTH_NAAAS_4G_SIM_PHONELINE, aucPHONEline)) == -1)
    {
        printf("文件打开失败或文件不存在！\n");
        return -1;
    }
    else{
        printf("SIM card begin read !\n");
        printf_dae_uesend("PHONE NUMBER 读取成功！\n");
    }

    sscanf(aucPHONEline, "%*[^=]=%[^\n]", gs_SimInfo.aucPHONE);
    printf("PHONE NUMBER :%s\n",gs_SimInfo.aucPHONE);
    printf_dae_uesend("PHONE NUMBER：%s\n",gs_SimInfo.aucPHONE);

    gs_iIsReadCard = 1;

    return 0;
}

int NSP_TDYTH_4G_IMS_Function()
{
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // UE
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    // ENB
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfd);

    if (bind(iSockfd, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
        //exit(1);
    }

    int sin_len = sizeof(ENBSockaddr);

    /****************************************IMS注册第一步******************************************/
    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    NSP_TDYTH_4G_IMSRegister_First_UEToENB(cRecvBuf, cSendBuf);

    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENBSockaddr, sin_len);
    if(iRet > 0)
    {
        printf("------------------------step1. UE#Sender -> ENB：0x01------------------------\n");
        printf("UE->ENB IMS注册第一步发送成功!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);

    return 0;
}

/***********************************************************************TAU***********************************************************************/
/*
 * 功能：UE发起TAU流程
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_TAU_Function()
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    if(gs_iENBnum == -1)
    {
        printf("暂无驻留基站，请等待基站驻留后重试！\n");
        printf_dae_uesend("暂无驻留基站，请等待基站驻留后重试！\n");
        return -1;
    }

    int iRet;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // UE
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    // ENB
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(gs_astEnbList[gs_iENBnum].uiENBPort);
    ENBSockaddr.sin_addr.s_addr = inet_addr(gs_astEnbList[gs_iENBnum].aucENBIp);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfd);

    int sin_len = sizeof(ENBSockaddr);

    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    //构造TAU第1步发送数据包
    NSP_TDYTH_4G_TAU_First_UEToENB(cRecvBuf,cSendBuf);

    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &ENBSockaddr, sin_len);
    if(iRet > 0)
    {
        printf("----------TAU: step1. UE#Sender -> ENB：TAU Request----------\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
    return 0;
}

/*
 * 功能：  TAU流程第18步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_TAU_Eighteenth_UEToNewMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int i;
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyEighteenthUEToNewMME stTauBodyEighteenthUEToNewMME;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE *stTauBodySeventeenthNewMMEToUE = (St_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodySeventeenthNewMMEToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x12;

    stTauBodyEighteenthUEToNewMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyEighteenthUEToNewMME.aucDstId, stTauBodySeventeenthNewMMEToUE->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyEighteenthUEToNewMME.uiDstPort = stTauBodySeventeenthNewMMEToUE->uiSrcPort;
    memcpy(stTauBodyEighteenthUEToNewMME.aucSrcId, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyEighteenthUEToNewMME.uiSrcPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stTauBodyEighteenthUEToNewMME.uiDataLen = 0;

    memcpy(stTauBodyEighteenthUEToNewMME.aucTerminalId, stTauBodySeventeenthNewMMEToUE->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    //存储TAIList
    gs_iTAIList_Size = stTauBodySeventeenthNewMMEToUE->iTAIList_Size;
    for(i=0; i<gs_iTAIList_Size; i++)
    {
        memcpy(gs_aucTAIList[i], stTauBodySeventeenthNewMMEToUE->aucTAIList[i], D_NSP_TDYTH_NAAAS_4G_TAI_LEN);
    }
    printf("--------------------\n");
    printf("更新后的TAI List：\n");
    for(i=0; i<gs_iTAIList_Size; i++)
    {
        printf("%d. %s\n", i+1, gs_aucTAIList[i]);
    }
    printf("--------------------\n");


    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyEighteenthUEToNewMME, D_NSP_TDYTH_NAAAS_4G_TauBodyEighteenthUEToNewMME_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyEighteenthUEToNewMME.aucDstId, stTauBodyEighteenthUEToNewMME.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/***********************************************************************TAU结束***********************************************************************/

/*
 * 功能： UE开启监听流程
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_ListenAccept_Function()
{
    int iSockfd;
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN];
    char acHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];          //对mme发来的数据包头进行解析

    iSockfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

//    // enb1
//    struct sockaddr_in ENBSockaddr;
//    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
//    ENBSockaddr.sin_family = AF_INET;
//    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
//    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);
//
    // enb2
    struct sockaddr_in ENB2Sockaddr;
    bzero(&ENB2Sockaddr, sizeof(ENB2Sockaddr));
    ENB2Sockaddr.sin_family = AF_INET;
    ENB2Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB2_PORT);
    ENB2Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB2_IP);

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iSockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iSockfd);

    if (bind(iSockfd, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        //err_print();
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
        //exit(1);
    }

    int iSinLen = sizeof(UESockaddr);

    struct sockaddr_in ENBSockaddr;

    while (1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));
        iRet = recvfrom(iSockfd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                        (struct sockaddr *) &UESockaddr, &iSinLen);

        // 每次接收消息后将根据当前驻留的基站进行后续消息的发送
        bzero(&ENBSockaddr, sizeof(ENBSockaddr));
        ENBSockaddr.sin_family = AF_INET;
        ENBSockaddr.sin_port = htons(gs_astEnbList[gs_iENBnum].uiENBPort);
        ENBSockaddr.sin_addr.s_addr = inet_addr(gs_astEnbList[gs_iENBnum].aucENBIp);

        if (iRet > 0)
        {
            memcpy(acHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *) acHeader;
            switch(pstHeader->ucReqType)
            {
                case 0x00:
                    printf("-------------目前处于注册流程！-------------\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x02:
                            printf("-------------step2. UE#Receiver ->HSS:0x02-------------\n");

                            printf("HSS->UE 注册成功!\n");

                            gs_iIsRegis = 1;

                            printf_dae_uesend("UE注册信息已绑定成功，设备注册成功!\n");
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;

                case 0x01:
                    printf("-------------目前处于认证流程！-------------\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x02:
                            printf("-------------step2: UE#Receiver <- ENB:0x02-------------\n");
                            if(NSP_TDYTH_4G_accessAuth_User_Identity_Response(cRecvBuf, cSendBuf) < 0) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("用户身份响应发送完毕,step: 3 完成.\n");
                            }
                            printf("-------------step3: UE#Sender -> ENB：0x03-------------\n");
                            break;

                        case 0x0a:
                            printf("收到MME请求step：%x, 解析完成,开始进行认证.\n", pstHeader->ucStep);
                            printf("-------------step10: UE#Receiver <- ENB:0x0a-------------\n");

                            if(NSP_TDYTH_4G_accessAuth_User_Auth_Response(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("用户身份响应发送完毕,step: 11 完成.\n");
                            }
                            printf("-------------step11: UE#Sender -> ENB：0x0b-------------\n");
                            break;

                        case 0x0e:
                            printf("收到MME请求step：%x, 解析完成,开始进行认证.\n", pstHeader->ucStep);
                            NSP_TDYTH_4G_accessAuth_AuthFeedback(cRecvBuf);      //处理认证反馈消息 0x09
                            printf_dae_uesend("认证已完成，success!\n");
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;

                case 0x02:
                    printf("-------------目前处于请求用户信息流程！-------------\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x03:
                            printf("-------------step3. UE#Receiver ->MME:0x03-------------\n");
                            if(NSP_TDYTH_4G_UserInfo_Request_Fourth_UESendToMME(cRecvBuf, cSendBuf) < 0) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else 
                            {
                                printf("-------------step4: UE#Sender -> MME：0x04-------------\n");
                                printf_dae_uesend("收到MME用户信息请求，用户身份信息发送成功!\n");
                            }
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;

                case 0x03:
                    printf("-------------目前处于UE附着流程！-------------\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x02:
                            printf("-------------step2: UE#Receiver <- ENB:0x02-------------\n");
                            if(NSP_TDYTH_4G_Attach_Third_UESendToENB(cRecvBuf , cSendBuf) < 0) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &ENBSockaddr, sizeof(ENBSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else 
                            {
                                printf("UE->ENB 附着请求发送成功!\n");
                            }
                            printf("-------------step3: UE#Sender -> ENB：0x03-------------\n");
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;

                case 0x04:
                    printf("-------------目前处于IP分配流程！--------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x06:          // IP分配流程
                            printf("-------------step6. UE#Receiver ->ENB:0x06-------------\n");

                            if(NSP_TDYTH_4G_IPAllot_Seventh_UESendToENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENBSockaddr, sizeof(ENBSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("IP分配发送完毕,step: 7 完成.\n");
                            }

                            printf("-------------step7: UE#Sender -> ENB：0x07-------------\n");
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;

                case 0x05:
                    printf("-------------目前处于X2流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x04:          // IP分配流程
                            printf("-------------step4: UE#Receiver <- SENB:0x04--------------\n");

                            if(NSP_TDYTH_4G_HandOver_Sixth_UESendToTENB(cRecvBuf , cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB2Sockaddr, sizeof(ENB2Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("UE->TENB RRC连接配置完成消息发送!\n");
                            }

                            printf("-------------step6.UE#Receiver ->TENB:0x06-------------\n");
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }

                    break;

                case 0x06:
                    printf("--------------目前处于S1流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x06:          // IP分配流程
                            printf("-------------step6: UE#Receiver <- SENB:0x06-------------\n");

                            if(NSP_TDYTH_4G_S1_HandOver_Ninth_UESendToTENB(cRecvBuf , cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB2Sockaddr, sizeof(ENB2Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("UE->TENB RRC连接配置完成消息发送!\n");
                            }

                            printf("-------------step9.UE#Sender ->TENB:0x09--------------\n");
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }

                    break;

                case 0x07:
                    printf("-------------目前处于站内切换流程！--------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x02:          // IP分配流程
                            printf("-------------step2: UE#Receiver <- ENB:0x02-------------\n");

                            if(NSP_TDYTH_4G_Intra_HandOver_Third_UESendToENB(cRecvBuf , cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENBSockaddr, sizeof(ENBSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("UE->TENB RRC连接配置完成消息发送!\n");
                            }

                            printf("-------------step3.UE#Sender ->ENB:0x03-------------\n");
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }

                    break;

                case 0x08:
                    printf("--------------目前处于连接中断流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x02:          // IP分配流程
                            printf("-------------step2. UE#Receiver ->ENB:0x02-------------\n");

                            if(NSP_TDYTH_4G_LinkageInterrupt_Third_UE(cRecvBuf, cSendBuf) < 0) break;

                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;

                case 0x09:
                    printf("-------------目前处于恢复连接流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x02:          // IP分配流程
                            printf("-------------step2. UE#Receiver ->ENB:0x02--------------\n");

                            if(NSP_TDYTH_4G_LinkageRecover_Third_UE(cRecvBuf, cSendBuf) < 0) break;

                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x10:
                    switch (pstHeader->ucStep)
                    {
                        case 0x08:
                            NSP_TDYTH_4G_IpNet9(cRecvBuf);
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "Step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x0b:
                    printf("-------------目前处于专有承载建立流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x05:
                            printf("-------------step5.UE#Receiver ->ENB:0x05-------------\n");
                            if (NSP_TDYTH_4G_BearerSetUp_Sixth_UEToENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENBSockaddr, sizeof(ENBSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step6.UE#Sender ->ENB:0x06-------------\n");
                            }
                            sleep(1);
                            if (NSP_TDYTH_4G_BearerSetUp_Eighth_UEToENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENBSockaddr, sizeof(ENBSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step8.UE#Sender ->ENB:0x08-------------\n");
                            }
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;

                case 0x0c:
                    printf("-------------目前处于IMS流程！--------------\n");
                    switch (pstHeader->ucStep) {
                        case 0x0a:
                            printf("-------------step10.UE#Receiver ->ENB:0x0a-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Eleventh_UEToENB(cRecvBuf , cSendBuf) < 0) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &ENBSockaddr, sizeof(ENBSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else
                            {
                                //printf("UE->ENB IMS请求发送成功!\n");
                                printf("--------------step11: UE#Sender -> ENB：0x0b-------------\n");
                            }
                            break;
                        case 0x14:
                             printf("-------------step21.UE#Receiver ->ENB:0x0a--------------\n");
                             NSP_TDYTH_4G_IMSRegister_Twenty_first_UEToENB(cRecvBuf , cSendBuf);
                             printf("------------- step21: finished!  -------------\n");
                             break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;

                    }
                case 0x11:
                    //printf("------------------------收到ENB广播包------------------------\n");
                    if(NSP_TDYTH_4G_Broadcast_Receive(cRecvBuf) < 0)
                    {
                        printf("ENB数据更新失败!\n");
                    }else{
                        //printf("ENB数据更新成功!\n");
                    }
                    break;
                case 0x12:
                    printf("\n----------收到错误信息包----------\n\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x00:
                            if(NSP_TDYTH_4G_AucRefuse_Receive(cRecvBuf) < 0)
                            {
                                printf("拒绝消息处理失败!\n");
                            }else{
                                //printf("拒绝消息处理完成!\n");
                            }
                            break;
                        case 0x01:
                            if(NSP_TDYTH_4G_TauReject_Receive(cRecvBuf) < 0)
                            {
                                printf("拒绝消息处理失败!\n");
                            }else{
                                //printf("拒绝消息处理完成!\n");
                            }
                            break;
                        default:
                            printf("收到未知类型错误信息包！[Step=%d]\n", pstHeader->ucStep);
                            break;
                    }
                    break;
                case 0x13:
                    printf("\n----------目前处于TAU流程----------\n\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x11:
                            printf("----------TAU: step17. UE#Receiver -> New MME：TAU Accept----------\n");
                            if(NSP_TDYTH_4G_TAU_Eighteenth_UEToNewMME(cRecvBuf, cSendBuf, iSockfd) < 0 ) {
                                printf("----------TAU: step18. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step18. UE#Sender -> New MME：TAU Complete----------\n");
                            }
                            break;
                        default:
                            printf("收到TAU包的步骤信息无法识别！[Step=%d]\n", pstHeader->ucStep);
                            break;
                    }
                    break;
                default:
                    printf("%s ReqType=%d,[line=%d],errno=%d:%s\n", "ReqType errno.", pstHeader->ucReqType, __LINE__, errno, strerror(errno));
                    break;
            }
        }
    }
    close(iSockfd);
    return 0;
}

/*
 * 功能：UE小区（ENB）重选函数
 * 参数：
 * 返回值：
 *            = 0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_CellReSelection_Function()
{
    if(gs_iIsOpen == 0)
    {
        printf("终端UE还未开机,请开机！\n");
        printf_dae_uesend("终端UE还未开机,请开机！\n");
        return -1;
    }

    gs_iENBnum = gs_iBestENBnum;        //选择最优基站进行驻留
    memcpy(&ue_Info.curGNBInfo,&gs_astEnbList[gs_iENBnum], sizeof(St_NSP_TDYTH_NAAAS_4G_EnbInfo));
    printf("完成小区选择/重选\n");

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}


/*
 * 功能：UE对小区（ENB）的持续监测函数
 * 参数：
 * 返回值：
 *            = 0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_CellMonitor_Function()
{
    int i, j;
    int ENBnum;
    int maxRSRP = -200;
    int iEARFCN_List_Len = sizeof(gs_auiEARFCN_List) / sizeof(gs_auiEARFCN_List[0]);

    while(1)
    {
        if(!gs_iIsOpen || !gs_iIsReadCard) continue;   //这里是为了让UE在未开机或未读卡时跳过监测过程，实测不能用while()来判别

        //先检索一遍EnbList，删除未更新的基站信息
        for(i=0; i <= gs_iEnblist_last; i++)
        {
            if(gs_astEnbList[i].iIsUpdate == 1){
                gs_astEnbList[i].iIsUpdate = 0;
            }else if(gs_astEnbList[i].iIsUpdate == 0 || gs_astEnbList[i].iRSRP < gs_astEnbList[i].iQrxlevmin)
            {
                //删除操作
                for(j=i; j <= gs_iEnblist_last - 1; j++)
                {
                    gs_astEnbList[j].uiENBID = gs_astEnbList[j + 1].uiENBID;
                    gs_astEnbList[j].uiPCI = gs_astEnbList[j + 1].uiPCI;
                    gs_astEnbList[j].uiEARFCN = gs_astEnbList[j + 1].uiEARFCN;
                    gs_astEnbList[j].iRSRP = gs_astEnbList[j + 1].iRSRP;
                    gs_astEnbList[j].iCellRP = gs_astEnbList[j + 1].iCellRP;
                    gs_astEnbList[j].iQrxlevmin = gs_astEnbList[j + 1].iQrxlevmin;
                    memcpy(gs_astEnbList[j].aucTAI, gs_astEnbList[j + 1].aucTAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);

                    gs_astEnbList[j].iIsBAN = gs_astEnbList[j + 1].iIsBAN;
                    gs_astEnbList[j].iIsUpdate = gs_astEnbList[j + 1].iIsUpdate;

                    memcpy(gs_astEnbList[j].aucENBIp, gs_astEnbList[j + 1].aucENBIp, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
                    gs_astEnbList[j].uiENBPort = gs_astEnbList[j + 1].uiENBPort;
                }
                gs_iEnblist_last--;
            }else{
                printf("gs_astEnbList[%d].is_Update信息出错\n", i);
            }
        }

        //小区质量检测
        maxRSRP = -200;             //重置最大RSRP
        for(i=0; i < iEARFCN_List_Len; i++)           //对支持的载频进行逐个扫描
        {
            ENBnum = -1;
            for(j=0; j <= gs_iEnblist_last; j++)      //检索列表中此EARFCN的ENB
            {
                if(gs_astEnbList[j].uiEARFCN == gs_auiEARFCN_List[i])
                {
                    if(gs_astEnbList[j].iRSRP > maxRSRP)
                    {
                        ENBnum = j;             //求得RSRP最大的ENB的序号
                        maxRSRP = gs_astEnbList[j].iRSRP;
                    }
                }
            }
            // 判断此ENB是否适合驻留
            if(ENBnum >= 0)
            {
                if(gs_astEnbList[ENBnum].iRSRP > -125 && gs_astEnbList[ENBnum].iIsBAN == 0) //此处实际应该为满足S准则等多重标准，这里简化处理
                {

                    gs_iBestENBnum = ENBnum;    //若当前载频出现满足条件的最强ENB，则将其标记为最优基站
                    //保存最佳基站的信息
                    memcpy(&ue_Info.bestGNBInfo, &gs_astEnbList[gs_iBestENBnum], sizeof(St_NSP_TDYTH_NAAAS_4G_EnbInfo));
                    break;
                }
                else if(i == iEARFCN_List_Len-1)
                {
                    gs_iBestENBnum = -1;        //没有符合条件的ENB，最优基站标记为无（-1）
                    //清除存最佳基站的信息
                    memset(&ue_Info.bestGNBInfo, 0, sizeof(St_NSP_TDYTH_NAAAS_4G_EnbInfo));
                    break;
                }else{
                    continue;               //若最强ENB不满足条件，对下一频段进行扫描
                }
            }else{
                if(i == iEARFCN_List_Len-1)
                {
                    gs_iBestENBnum = -1;            //没有符合条件的ENB，最优基站标记为无（-1）
                    //清除最佳基站的信息
                    memset(&ue_Info.bestGNBInfo, 0, sizeof(St_NSP_TDYTH_NAAAS_4G_EnbInfo));
                    break;
                }
            }
        }

        //此处为小区更新的时间间隔，需要比ENB发送广播包的间隔大
        sleep(D_NSP_TDYTH_NAAAS_4G_UECellReselection_Interval);
    }
    return 0;
}


/*
 * 功能：通过接受前端指令选择功能
 * 参数： 
 * 返回值：  
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_UEFront_Function()
{
    int UE_Status;
    int UE_Continue;
    int UE_function;

    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];

    int iSockfdUE = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UEF_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);
    int sin_len = sizeof(UESockaddr);

    // bind，成功返回0，出错返回-1
    if (bind(iSockfdUE, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    while(1)
    {

        iRet = recvfrom(iSockfdUE, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, 0,(struct sockaddr *) &UESockaddr, &sin_len);
        St_NSP_TDYTH_NAAAS_4G_DaemonControl *stDaemonControl = (St_NSP_TDYTH_NAAAS_4G_DaemonControl *)cRecvBuf;

        switch(stDaemonControl->uiFunction)
        {
            case D_NSP_TDYTH_NAAAS_4G_UE_REGISTER:
                NSP_TDYTH_4G_FrontRegister_Function(cRecvBuf);
                break;
            case D_NSP_TDYTH_NAAAS_4G_UE_LOCATION:
                NSP_TDYTH_4G_Gps_Function();
                break;
            case D_NSP_TDYTH_NAAAS_4G_UE_ATTACH:
                NSP_TDYTH_4G_Attach_Function();
                break;
            case D_NSP_TDYTH_NAAAS_4G_UE_POWERON:
                NSP_TDYTH_4G_UE_poweron();
                break;
            case D_NSP_TDYTH_NAAAS_4G_UE_RESTART:
                NSP_TDYTH_4G_UE_restart();
                break;
            case D_NSP_TDYTH_NAAAS_4G_UE_PHONE:
                NSP_TDYTH_4G_UE_phone();
                break;
            case D_NSP_TDYTH_NAAAS_4G_UE_POWEROFF:
                NSP_TDYTH_4G_UE_poweroff();
                break;
            case D_NSP_TDYTH_NAAAS_4G_UE_SIMINFO:
                NSP_TDYTH_4G_readmessage();
                break;
            case D_NSP_TDYTH_NAAAS_4G_UE_SWITCH:
                NSP_TDYTH_4G_UE_FrontMeasureReport_Function(cRecvBuf);
                break;
            default:
                printf("功能格式错误！\n");
                break;
        }
    }
    return 0;

}

/*
 * 功能：通过后台选取功能
 * 参数： 
 * 返回值：  
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_UEBack_Function()
{
    int UE_function;
    while(1)
    {
        puts("请选择要执行的操作：");
        puts("1. 开机");
        puts("2. 读取SIM卡信息");
        puts("3. 向HSS发起注册UE");
        puts("4. 执行小区选择/重选");
        puts("5. 发起附着请求");
        puts("6. 发起TAU流程");
        puts("7. 获取gps信息(需安装httpie)");
        puts("8. UE发送测量报告(切换)");
        puts("9. 打电话");
        puts("10. UE上网请求");
        puts("11. IMS注册");

        puts("21. 显示所有基站信息");
        puts("22. 显示当前和最优基站信息");
        puts("23. 显示调试信息");
        puts("31. 重启");
        puts("32. 关机");
        puts("99. 退出");
        puts("......后续添加.......");

        scanf("%d", &UE_function);
        getchar();
        switch(UE_function)
        {
            case 1:
                NSP_TDYTH_4G_UE_poweron();
                break;
            case 2:
                NSP_TDYTH_4G_readmessage();
                break;
            case 3:
                NSP_TDYTH_4G_BackRegister_Function();
                break;
            case 4:
                NSP_TDYTH_4G_CellReSelection_Function();
                break;
            case 5:
                NSP_TDYTH_4G_Attach_Function();
                break;
            case 6:
                NSP_TDYTH_4G_TAU_Function();
                break;
            case 7:
                NSP_TDYTH_4G_Gps_Function();
                break;
            case 8:
                NSP_TDYTH_4G_UE_MeasureReport_Function();
                break;
            case 9:
                NSP_TDYTH_4G_UE_phone();
                break;
            case 10:
                NSP_TDYTH_4G_IpNet_Function();
                break;
            case 11:
                NSP_TDYTH_4G_IMS_Function();
                break;

            case 21:
                NSP_TDYTH_4G_EnbInfo_Display();
                break;
            case 22:
                NSP_TDYTH_4G_EnbHandover_Display();
                break;
            case 23:
                NSP_TDYTH_4G_UEDebug_Function();
                break;

            case 31:
                NSP_TDYTH_4G_UE_restart();
                break;
            case 32:
                NSP_TDYTH_4G_UE_poweroff();
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
    int iUE_select;

    pthread_t pth_epoll,pth_epoll2;

    printf("------------开启子线程进行监听------------\n");

    if(pthread_create(&pth_epoll, NULL, (void *)NSP_TDYTH_4G_ListenAccept_Function, NULL))
    {
       perror("pthread_creat error!\n");
       return -1;
    }
    else
    {
        printf("---------------监听子线程开启成功---------------\n");
    }

    printf("------------开启子线程进行ENB质量监测------------\n");
    if(pthread_create(&pth_epoll2, NULL, (void *)NSP_TDYTH_4G_CellMonitor_Function, NULL))
    {
        perror("pthread_creat error!\n");
        return -1;
    }
    else
    {
        printf("---------------ENB质量监测子线程开启成功---------------\n");
    }
    NSP_TDYTH_4G_UEBack_Function();
//    while(1)
//    {
//        puts("请选择UE需要展现的方式：");
//        puts("1.接受前端消息，结果在前端展示(涉及前端)");
//        puts("2.后台操作(纯后台)");
//        puts("99.退出");
//        scanf("%d", &iUE_select);
//        getchar();
//        switch(iUE_select)
//        {
//            case 1:
//                NSP_TDYTH_4G_UEFront_Function();
//                break;
//            case 2:
//                NSP_TDYTH_4G_UEBack_Function();
//                break;
//            case 99:
//                return 0;
//            default:
//                printf("输入错误\n");
//                break;
//        }
//    }
}
