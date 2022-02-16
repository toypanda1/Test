/*
 * 文件功能：eNB1功能
 * 作者：李高明 张一帆
 * 创建日期：2020/06/21
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_anet.h"
#include "NSP_TDYTH_NAAAS_4G_utils.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"
#include "NSP_TDYTH_NAAAS_4G_debug.h"
#include "NSP_TDYTH_NAAAS_4G_db.h"
#include "NSP_TDYTH_NAAAS_4G_gen.h"
#include "NSP_TDYTH_NAAAS_5G_snowflake.h"

//功能：SENB端存储信息结构体

typedef struct {
    unsigned int  uiNew_Enb_Ue_X2Id;
    unsigned int  uiOld_Enb_Ue_X2Id;
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_SwitchInfo;


static St_NSP_TDYTH_NAAAS_4G_BroadcastBody gsSelfENB;
static St_NSP_TDYTH_NAAAS_4G_SwitchInfo gs_stSwitchInfo;
static int gs_ix = 100;                                         //基站X坐标
static int gs_iy = 100;                                         //基站Y坐标
static unsigned int gs_ienb_ue_id = 2000;                       //ENB分给UE的id
unsigned char gsKeNB2[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
/*
 * 功能：4G接入认证：eNB转发MME发起的用户身份请求
 * 参数：	无
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_accessAuth_User_Identity_Request_Forward(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    printf("eNB解析MME数据(认证第一步)，并开始构造数据!\n");
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB *stAuBodyFirstMMEToENB = (St_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyFirstMMEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    //重新构造数据包头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x02;

    //对MME认证数据进行转发
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodySecondENBToUE_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,stAuBodyFirstMMEToENB->aucDstId,
        stAuBodyFirstMMEToENB->aucDstPort, 0)) < 0) 
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("用户身份响应发送完毕,step: 2 完成.\n");
    }
    printf("------------------------step2: ENB#Sender -> UE：0x02------------------------\n");

    return 0;
}

/*
 * 功能：4G接入认证：eNB转发UE发起的用户身份响应
 * 参数：	无
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_accessAuth_User_Identity_Response_Forward(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB *stAuBodyThirdUEToENB = (St_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyThirdUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    printf("eNB解析UE数据(认证第三步)，并开始重新构造数据!\n");

    //重新构造数据包头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x04;

    //对MME认证数据进行转发
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyFourthENBToMME_LEN);

    return 0;
}

/*
 * 功能：4G接入认证：eNB转发UE发起的用户身份响应
 * 参数：	无
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_accessAuth_User_Auth_Request_Forward(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{ 
    int iRet;
	printf("eNB解析MME数据(认证第九步)，并开始重新构造数据!\n");

    //重新构造数据包头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB *stAuBodyNinethMMEToENB = (St_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyNinethMMEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0a;

    //对MME认证数据进行转发
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyTenthENBToUE_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,stAuBodyNinethMMEToENB->aucDstId, 
        stAuBodyNinethMMEToENB->aucDstPort, 0)) < 0) 
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("用户身份响应发送完毕,step: 10 完成.\n");
    }
    printf("------------------------step10: ENB#Sender -> UE：0x0a------------------------\n");

    return 0;
}

/*
 * 功能：4G接入认证：eNB转发UE发起的用户认证响应
 * 参数：	无
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_accessAuth_User_Auth_Response_Forward(char *pcRecvBuf, char *pcSendBuf)
{
	printf("eNB解析UE数据(认证第十一步)，并开始重新构造数据!\n");

    //重新构造数据包头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB *stAuBodyEleventhUEToENB = (St_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyEleventhUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0c;

    //对MME认证数据进行转发
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyTwelfthENBToMME_LEN);

    return 0;
}

/*
 * 功能：4G接入认证：eNB转发UE发起的用户认证响应
 * 参数：	无
 * 返回值：  0   成功
 */
int NSP_TDYTH_4G_accessAuth_AuthResult_Forward(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{
	int iRet;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB *stAuBodyThirteenthMMEToENB = (St_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAuBodyThirteenthMMEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    printf("eNB解析MME数据(认证第十三步)，并开始重新构造数据!\n");
    //重新构造数据包头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0e;

    //对MME认证数据进行转发
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodyFourteenthENBToUE_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,stAuBodyThirteenthMMEToENB->aucDstId, 
        stAuBodyThirteenthMMEToENB->aucDstPort, 0)) < 0) 
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("用户身份响应发送完毕,step: 14 完成.\n");
    }
    printf("------------------------step14: ENB#Sender -> UE：0x0e------------------------\n");

    return 0;
}
/***************************************************************附着*************************************************************************************/
/*
 * 功能：附着请求第二步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_Second_eNB_Send(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AttBodySecondeNBToUE stAttBodySecondeNBToUE;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB_LEN);
    St_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB *stAttBodyFirstUEToeNB = (St_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAttBodyFirstUEToeNB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x02;

    stAttBodySecondeNBToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAttBodySecondeNBToUE.aucIdNumber = 0x0;
    memcpy(stAttBodySecondeNBToUE.aucDstId, stAttBodyFirstUEToeNB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAttBodySecondeNBToUE.aucSrcId, stAttBodyFirstUEToeNB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAttBodySecondeNBToUE.aucSrcPort = stAttBodyFirstUEToeNB->aucSrcPort;
    stAttBodySecondeNBToUE.aucDstPort = stAttBodyFirstUEToeNB->aucDstPort;
    stAttBodySecondeNBToUE.iDataLen = 0;

    memcpy(stAttBodySecondeNBToUE.aucCRNTI, "00", D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN);              //此处CRNTI固定，后续可添加生成函数，将生成值传进来

    memcpy(stAttBodySecondeNBToUE.aucTerminalId, stAttBodyFirstUEToeNB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("随机接入响应数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAttBodySecondeNBToUE, D_NSP_TDYTH_NAAAS_4G_AttBodySecondeNBToUE_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,stAttBodyFirstUEToeNB->aucSrcId, 
        stAttBodyFirstUEToeNB->aucSrcPort, 0)) < 0) 
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("随机接入响应发送完毕,step: 2 完成.\n");
    }
    printf("------------------------step2: ENB#Sender -> UE：0x02------------------------\n");

    return 0;
}

/*
 * 功能：附着请求第四步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_Fourth_eNB_Send(char *pcRecvBuf, char *pcSendBuf)
{
    char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB_LEN);
    St_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB *stAttBodyThirdUEToeNB = (St_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAttBodyThirdUEToeNB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    
    if(gs_ienb_ue_id == 3000)
    {
        gs_ienb_ue_id = 2000;
    }

    //重新构造数据包头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME stAttBodyFourtheNBToMME;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x04;

    stAttBodyFourtheNBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAttBodyFourtheNBToMME.aucIdNumber = 0x0;
    memcpy(stAttBodyFourtheNBToMME.aucDstId, stAttBodyThirdUEToeNB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAttBodyFourtheNBToMME.aucSrcId, stAttBodyThirdUEToeNB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAttBodyFourtheNBToMME.aucSrcPort = stAttBodyThirdUEToeNB->aucSrcPort;
    stAttBodyFourtheNBToMME.aucDstPort = stAttBodyThirdUEToeNB->aucDstPort;
    stAttBodyFourtheNBToMME.iDataLen = 0;

    memcpy(stAttBodyFourtheNBToMME.aucIMSI, stAttBodyThirdUEToeNB->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    memcpy(stAttBodyFourtheNBToMME.aucPDN, stAttBodyThirdUEToeNB->aucPDN, D_NSP_TDYTH_NAAAS_4G_PDNTYPE_LEN);
    memcpy(stAttBodyFourtheNBToMME.aucPCO, stAttBodyThirdUEToeNB->aucPCO, D_NSP_TDYTH_NAAAS_4G_PCO_LEN);
    stAttBodyFourtheNBToMME.uiEnb_Ue_Id = gs_ienb_ue_id++;

    memcpy(stAttBodyFourtheNBToMME.aucTerminalId, stAttBodyThirdUEToeNB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    //对MME认证数据进行转发
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAttBodyFourtheNBToMME, D_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能： IP分配第六步，将MME发来的数据包转化为发给UE端的格式
 * 参数：  @pcRecvBuf [in] 接收缓冲区
 *        @pcSendBuf [out]  发送缓冲区
 * 返回值：   0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_IPAllot_Sixth_ENBSendToUE(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{   
    int iRet;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE stIPAllotBodySixthENBToUE;

    memcpy(aucbody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN,sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE));

    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFifthMMEToENB * sIPAllotBodyFifthMMEToENB = (St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFifthMMEToENB *)aucbody;
    if(NSP_TDYTH_4G_verification_timestamp(sIPAllotBodyFifthMMEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }



    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x06;


    stIPAllotBodySixthENBToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIPAllotBodySixthENBToUE.aucIdNumber = 0x0;
    memcpy(stIPAllotBodySixthENBToUE.aucDstId, sIPAllotBodyFifthMMEToENB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIPAllotBodySixthENBToUE.aucSrcId, sIPAllotBodyFifthMMEToENB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIPAllotBodySixthENBToUE.aucDstPort = sIPAllotBodyFifthMMEToENB->aucDstPort;
    stIPAllotBodySixthENBToUE.aucSrcPort = sIPAllotBodyFifthMMEToENB->aucSrcPort;
    stIPAllotBodySixthENBToUE.iDataLen = 0;

    //填充结构体信息
    memcpy(stIPAllotBodySixthENBToUE.aucGUTI, sIPAllotBodyFifthMMEToENB->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    stIPAllotBodySixthENBToUE.ucPDNType = sIPAllotBodyFifthMMEToENB->ucPDNType;
    memcpy(stIPAllotBodySixthENBToUE.aucPDNAddress, sIPAllotBodyFifthMMEToENB->aucPDNAddress, D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN);
    stIPAllotBodySixthENBToUE.ucPCO = sIPAllotBodyFifthMMEToENB->ucPCO;

    memcpy(stIPAllotBodySixthENBToUE.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("SixStep构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIPAllotBodySixthENBToUE, sizeof(stIPAllotBodySixthENBToUE)); //数据体填入发送缓冲区

    printf("enb1 ip:%s\n",sIPAllotBodyFifthMMEToENB->aucDstId);
    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,sIPAllotBodyFifthMMEToENB->aucDstId, 
        sIPAllotBodyFifthMMEToENB->aucDstPort, 0)) < 0) 
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("附着接受发送完毕,step: 6 完成.\n");
    }
    printf("------------------------step6. eNB#Sender ->UE:0x06------------------------\n");
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：  IP分配第八步，将UE发来的数据包转化为发给MME端的格式
 * 参数：  @pcRecvBuf [in] 接收缓冲区
 *        @pcSendBuf [out]  发送缓冲区
 * 返回值：   0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_IPAllot_Eighth_ENBSendToMME(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB *stIPAllotBodySeventhUEToENB = (St_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB *)aucbody;
    if(NSP_TDYTH_4G_verification_timestamp(stIPAllotBodySeventhUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyEighthENBToMME  stIPAllotBodyEighthENBToMME;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x08;

    stIPAllotBodyEighthENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIPAllotBodyEighthENBToMME.aucIdNumber = 0x0;
    memcpy(stIPAllotBodyEighthENBToMME.aucDstId, stIPAllotBodySeventhUEToENB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIPAllotBodyEighthENBToMME.aucSrcId, stIPAllotBodySeventhUEToENB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIPAllotBodyEighthENBToMME.aucDstPort = stIPAllotBodySeventhUEToENB->aucDstPort;
    stIPAllotBodyEighthENBToMME.aucSrcPort = stIPAllotBodySeventhUEToENB->aucSrcPort;
    stIPAllotBodyEighthENBToMME.iDataLen = 0;

    stIPAllotBodyEighthENBToMME.flag='0';
    memcpy(stIPAllotBodyEighthENBToMME.aucTerminalId,"0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("附着消息数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIPAllotBodyEighthENBToMME, sizeof(stIPAllotBodyEighthENBToMME)); //数据体填入发送缓冲区
    return 0;
}

/*
 * 功能：X2切换第二步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_X2_HandOver_Second_SENBSendToTENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB *pstRecvBody = (St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB *)auchobody;

    if(NSP_TDYTH_4G_verification_timestamp(pstRecvBody->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    if(pstRecvBody->uiRSCP < D_NSP_TDYTH_NAAAS_4G_RSCP_SCOPE_LEN)
    {
        printf("RSCP范围未达到发送切换报告要求！\n");
        return -1;
    }

    if(pstRecvBody->uiRSRQ < D_NSP_TDYTH_NAAAS_4G_RSRQ_SCOPE_LEN)
    {
        printf("RSRQ范围未达到发送切换报告要求！\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodySecondSENBToTENB stSendBody;

    unsigned char aucKenb[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    memcpy(aucKenb,"000000000000000000", D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    int PCI = 501;
    int EARFCN = 38400;

    //todo KENB为保存的
    printf("开始生成密钥！\n");
    if ((iRet = NSP_TDYTH_4G_gen_Kenb(aucKenb, &PCI, &EARFCN, stSendBody.aucKenb)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb1 err", __LINE__, errno, strerror(errno));
        return -1;
    }

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x02;

    // 生成会话ID， 用来保存UE地址信息
    if(snowflake_init(0, 0)){
        long int sessionId = snowflake_id();
        // 保存addr
       // set_session_value(REDIS_C, sessionId, "UE_ADDR", pstRecvBody->aucSrcId, NULL);
        stSendBody.aucIdNumber = sessionId;
    }
    else {
        stSendBody.aucIdNumber = pstRecvBody->aucIdNumber;
    }

    stSendBody.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSendBody.aucIdNumber = 0x1;
    memcpy(stSendBody.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stSendBody.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.iDataLen = 0;

    stSendBody.uiOld_Enb_Ue_X2Id = 1001;
    stSendBody.uiMME_Ue_S1Id= 2001;
    stSendBody.aucNCC=0;
    memcpy(stSendBody.aucGUMMEI_Id, "000000000", D_NSP_TDYTH_NAAAS_4G_GUMMEI_LEN);
    memcpy(stSendBody.aucGUTI, "000000000000", D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stSendBody.aucOldC_RNTI, "0", D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN);

    memcpy(stSendBody.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSendBody, D_NSP_TDYTH_NAAAS_4G_HandOverBodySecondSENBToTENB_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/*
 * 功能：X2切换第3步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_X2_HandOver_Third_TENBSendToSENB(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_HandOverBodySecondSENBToTENB_LEN);

    St_NSP_TDYTH_NAAAS_4G_N2_BodySecondSENBToTENB *pstRecvBody = (St_NSP_TDYTH_NAAAS_4G_N2_BodySecondSENBToTENB*)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(pstRecvBody->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodyThirdTENBToSENB stSendBody;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x03;

    stSendBody.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSendBody.aucIdNumber = 0x1;
    memcpy(stSendBody.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stSendBody.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.iDataLen = 0;

    /*unsigned char aucKasme[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];
    memcpy(aucKasme, "00000000000000", D_NSP_TDYTH_NAAAS_4G_KASME_LEN);*/
    //计算KeNB2，保存在本地
    NSP_TDYTH_4G_gen_Kenb(pstRecvBody->aucKenb, D_NSP_TDYTH_NAAAS_4G_ENB2_PCI, D_NSP_TDYTH_NAAAS_4G_ENB2_EARFCN,
                          gsKeNB2);

    stSendBody.uiNew_Enb_Ue_X2Id = 1002;
    stSendBody.uiOld_Enb_Ue_X2Id = pstRecvBody->uiOld_Enb_Ue_X2Id;
    stSendBody.aucNCC = pstRecvBody->aucNCC;
    memcpy(stSendBody.aucCRNTI, "1", D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN);

    memcpy(stSendBody.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSendBody, D_NSP_TDYTH_NAAAS_4G_HandOverBodyThirdTENBToSENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：X2切换第四步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_X2_HandOver_Fourth_SENBSendToUE(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_HandOverBodyThirdTENBToSENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_N2_BodyThirdTENBToSENB *pstRecvBody = (St_NSP_TDYTH_NAAAS_4G_N2_BodyThirdTENBToSENB *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(pstRecvBody->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    gs_stSwitchInfo.uiNew_Enb_Ue_X2Id = pstRecvBody->uiNew_Enb_Ue_X2Id;
    gs_stSwitchInfo.uiOld_Enb_Ue_X2Id = pstRecvBody->uiOld_Enb_Ue_X2Id;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodyFourthSENBToUE stSendBody;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x04;

    stSendBody.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSendBody.aucIdNumber = 0x1;
    memcpy(stSendBody.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stSendBody.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.iDataLen = 0;

    stSendBody.aucNCC = pstRecvBody->aucNCC;
    memcpy(stSendBody.aucCRNTI, pstRecvBody->aucCRNTI, D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN);

    memcpy(stSendBody.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSendBody, D_NSP_TDYTH_NAAAS_4G_HandOverBodyFourthSENBToUE_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/*
 * 功能：X2切换第五步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_X2_HandOver_Fifth_SENBSendToTENB(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodyFifthSENBToTENB stSendBody;

    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_HandOverBodyThirdTENBToSENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_N2_BodyThirdTENBToSENB *pstRecvBody = (St_NSP_TDYTH_NAAAS_4G_N2_BodyThirdTENBToSENB *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(pstRecvBody->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x05;

    stSendBody.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSendBody.aucIdNumber = 0x1;
    memcpy(stSendBody.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stSendBody.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.iDataLen = 0;

    stSendBody.uiNew_Enb_Ue_X2Id = gs_stSwitchInfo.uiNew_Enb_Ue_X2Id;
    stSendBody.uiOld_Enb_Ue_X2Id = gs_stSwitchInfo.uiOld_Enb_Ue_X2Id;
    stSendBody.ucPdcpSn = '1';

    memcpy(stSendBody.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSendBody, D_NSP_TDYTH_NAAAS_4G_HandOverBodyFifthSENBToTENB_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/*
 * 功能：X2切换第7步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_X2_HandOver_Seventh_TENBSendToMME(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodySeventhTENBToMME stSendBody;

    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_HandOverBodySixthUEToTENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_N2_BodySixthUEToTENB *pstRecvBody = (St_NSP_TDYTH_NAAAS_4G_N2_BodySixthUEToTENB *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(pstRecvBody->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x07;

    stSendBody.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSendBody.aucIdNumber = 0x1;
    memcpy(stSendBody.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stSendBody.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.iDataLen = 0;

    stSendBody.uiEnb_Ue_S1Id = 1101;
    memcpy(stSendBody.aucECGI, "000000", D_NSP_TDYTH_NAAAS_4G_ECGI_LEN);

    memcpy(stSendBody.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSendBody, D_NSP_TDYTH_NAAAS_4G_HandOverBodySeventhTENBToMME_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：X2切换第9步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_X2_HandOver_Ninth_TENBSendToSENB(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_N2_BodyNinthTENBToSENB stHOBodyNinthTENBToSENB;

    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_HandOverBodySeventhTENBToMME_LEN);
    St_NSP_TDYTH_NAAAS_4G_N2_BodySeventhTENBToMME *StHOBodySeventhTENBToMME = (St_NSP_TDYTH_NAAAS_4G_N2_BodySeventhTENBToMME *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(StHOBodySeventhTENBToMME->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x05;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x09;

    stHOBodyNinthTENBToSENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stHOBodyNinthTENBToSENB.aucIdNumber = 0x1;
    memcpy(stHOBodyNinthTENBToSENB.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stHOBodyNinthTENBToSENB.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stHOBodyNinthTENBToSENB.iDataLen = 0;

    stHOBodyNinthTENBToSENB.uiNew_Enb_Ue_X2Id = gs_stSwitchInfo.uiNew_Enb_Ue_X2Id;
    stHOBodyNinthTENBToSENB.uiOld_Enb_Ue_X2Id = gs_stSwitchInfo.uiOld_Enb_Ue_X2Id;

    memcpy(stHOBodyNinthTENBToSENB.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stHOBodyNinthTENBToSENB, D_NSP_TDYTH_NAAAS_4G_HandOverBodyNinthTENBToSENB_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/***********************************************************************S1切换***********************************************************************/
/*
 * 功能：S1切换第二步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Second_SENBSendToMME(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    unsigned char aucKenb1[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucKenb[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    memcpy(aucKenb,"000000000000000000", D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    //todo NH哪里来的？
    unsigned char aucNH[D_NSP_TDYTH_NAAAS_4G_NH_LEN];


    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB *pstRecvBody = (St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB *)auchobody;

    if(NSP_TDYTH_4G_verification_timestamp(pstRecvBody->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    if(pstRecvBody->uiRSCP < D_NSP_TDYTH_NAAAS_4G_RSCP_SCOPE_LEN)
    {
        printf("RSCP范围未达到发送切换请求要求！\n");
        return -1;
    }

    if(pstRecvBody->uiRSRQ < D_NSP_TDYTH_NAAAS_4G_RSRQ_SCOPE_LEN)
    {
        printf("RSRQ范围未达到发送切换请求要求！\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME stSendBody;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x02;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x02;

    //todo KENB为保存的
    int pci = gsSelfENB.uiPCI;
    int earfcn =gsSelfENB.uiEARFCN;

    printf("开始生成密钥！\n");
    if ((iRet = NSP_TDYTH_4G_S1_gen_Kenb(aucKenb, &pci, &earfcn,
                                      aucKenb1)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb1 err", __LINE__, errno, strerror(errno));
        return -1;
    }
    NSP_TDYTH_4G_print("生成的密钥KeNB*：",aucKenb1, D_NSP_TDYTH_NAAAS_4G_KENB_LEN);




    memcpy(stSendBody.aucGUTI, pstRecvBody->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    stSendBody.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stSendBody.aucIdNumber = 0x1;
    memcpy(stSendBody.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stSendBody.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stSendBody.iDataLen = 0;

    stSendBody.uiMme_Ue_S1Id = 2001;
    stSendBody.uiEnb_Ue_S1Id = 5001;
    stSendBody.ucTarget_Cell_Id = 0;
    memcpy(stSendBody.aucKenb1, aucKenb1, D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    stSendBody.aucNCC = 0;

    memcpy(stSendBody.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSendBody, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME_LEN);



    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/*
 * 功能：S1切换第4步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Fourth_TENBSendToMME(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    unsigned char aucKenb2[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucPCI[D_NSP_TDYTH_NAAAS_4G_PCI_LEN];
    unsigned char aucEARFCN[D_NSP_TDYTH_NAAAS_4G_EARFCN_LEN];
    unsigned char aucKenb3[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucKupenc[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucKrrcint[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucKrrcenc[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucNH[D_NSP_TDYTH_NAAAS_4G_NH_LEN];

    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB *stS1HOBodyThirdMMEToTENB = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stS1HOBodyThirdMMEToTENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME stS1HOBodyFourthTENBToMME;

    printf("开始生成密钥！\n");
    if ((iRet = NSP_TDYTH_4G_S1_gen_Kenb(stS1HOBodyThirdMMEToTENB->aucKenb2, gsSelfENB.uiPCI, gsSelfENB.uiEARFCN, aucKenb3)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_Kenb3 err", __LINE__, errno, strerror(errno));
        return -1;
    }
    NSP_TDYTH_4G_print("生成的密钥KeNB**：",aucKenb3, D_NSP_TDYTH_NAAAS_4G_KENB_LEN);
    //todo
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
    stHeader.ucStep = 0x04;

    stS1HOBodyFourthTENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyFourthTENBToMME.aucIdNumber = 0x1;
    memcpy(stS1HOBodyFourthTENBToMME.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyFourthTENBToMME.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyFourthTENBToMME.iDataLen = 0;

    stS1HOBodyFourthTENBToMME.uiNewEnb_Ue_S1Id = 5002;

    memcpy(stS1HOBodyFourthTENBToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyFourthTENBToMME, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：S1切换第6步
 * 参数：   
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：    
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Sixth_SENBSendToUE(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB *stS1HOBodyFifthMMEToSENB = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stS1HOBodyFifthMMEToSENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE stS1HOBodySixthSENBToUE;

    //todo 根据会话ID 找到之前的UE

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x06;

    stS1HOBodySixthSENBToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodySixthSENBToUE.aucIdNumber = 0x1;
    memcpy(stS1HOBodySixthSENBToUE.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodySixthSENBToUE.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodySixthSENBToUE.iDataLen = 0;

    stS1HOBodySixthSENBToUE.ucTarget_Cell_Id = 0X30;

    memcpy(stS1HOBodySixthSENBToUE.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodySixthSENBToUE, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：S1切换第7步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Seventh_SENBSendToMME(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE *stS1HOBodySixthSENBToUE = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stS1HOBodySixthSENBToUE->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySeventhSENBToMME stS1HOBodySeventhSENBToMME;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x07;

    stS1HOBodySeventhSENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodySeventhSENBToMME.aucIdNumber = 0x1;
    memcpy(stS1HOBodySeventhSENBToMME.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodySeventhSENBToMME.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodySeventhSENBToMME.iDataLen = 0;

    stS1HOBodySeventhSENBToMME.aucPDCP_SN = '1';

    memcpy(stS1HOBodySeventhSENBToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodySeventhSENBToMME, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySeventhSENBToMME_LEN); //数据体填入发送缓冲区

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：S1切换第10步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Tenth_TENBSendToMME(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB *stS1HOBodyNinthUEToTENB = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stS1HOBodyNinthUEToTENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME stS1HOBodyTenthTENBToMME;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x0a;

    stS1HOBodyTenthTENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyTenthTENBToMME.aucIdNumber = 0x1;
    memcpy(stS1HOBodyTenthTENBToMME.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyTenthTENBToMME.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyTenthTENBToMME.iDataLen = 0;

    stS1HOBodyTenthTENBToMME.uiMME_UE_S1Id = 1001;
    stS1HOBodyTenthTENBToMME.uieNB_UE_S1Id = 1001;
    memcpy(stS1HOBodyTenthTENBToMME.aucECGI, "000000", D_NSP_TDYTH_NAAAS_4G_ECGI_LEN);

    memcpy(stS1HOBodyTenthTENBToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyTenthTENBToMME, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/*
 * 功能：S1切换第12步
 * 参数：
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_S1_HandOver_Twelfth_SENBSendToMME(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEleventhMMEToSENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEleventhMMEToSENB *stS1HOBodyEleventhMMEToSENB = (St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEleventhMMEToSENB *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stS1HOBodyEleventhMMEToSENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTwelfthSENBToMME stS1HOBodyTwelfthSENBToMME;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x06;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x0c;

    stS1HOBodyTwelfthSENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stS1HOBodyTwelfthSENBToMME.aucIdNumber = 0x1;
    memcpy(stS1HOBodyTwelfthSENBToMME.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stS1HOBodyTwelfthSENBToMME.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stS1HOBodyTwelfthSENBToMME.iDataLen = 0;

    stS1HOBodyTwelfthSENBToMME.uiMME_UE_S1Id = stS1HOBodyEleventhMMEToSENB->uiMME_UE_S1Id;
    stS1HOBodyTwelfthSENBToMME.uieNB_UE_S1Id = stS1HOBodyEleventhMMEToSENB->uieNB_UE_S1Id;

    memcpy(stS1HOBodyTwelfthSENBToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stS1HOBodyTwelfthSENBToMME, D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTwelfthSENBToMME_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}
/***********************************************************************站内切换***********************************************************************/
/*
 * 功能：     Intra切换第2步
 * 参数：   
 *            @pcRecvBuf [in] 收到的数据
 *            @pcSendBuf [out] 发送的数据
 * 返回值：    
 *             0   成功
 *            <0   失败
 */
int NSP_TDYTH_4G_Intra_HandOver_Second_ENBSendToUE(char *pcRecvBuf, char *pcSendBuf)
{
     unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     memcpy(auchobody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB_LEN);
     St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB *stIntraHandOverBodyFirstUEToENB = (St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB *)auchobody;
    if(NSP_TDYTH_4G_verification_timestamp(stIntraHandOverBodyFirstUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE stIntraHOBodySecondENBToUE;

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x07;
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x02;

    stIntraHOBodySecondENBToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIntraHOBodySecondENBToUE.aucIdNumber = 0x1;
    memcpy(stIntraHOBodySecondENBToUE.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIntraHOBodySecondENBToUE.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIntraHOBodySecondENBToUE.iDataLen = 0;

    stIntraHOBodySecondENBToUE.ucTarget_Cell_Id = 0X01;

    memcpy(stIntraHOBodySecondENBToUE.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("发送数据构造完毕.\n");
    printf("开始执行切换小区.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIntraHOBodySecondENBToUE, D_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

/***********************************************************************MME连接中断***********************************************************************/
/*
 * 功能：连接中断第二步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_LinkageInterrupt_Second_ENBtoUE(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB *stLiBodyFirstMMEToENB = (St_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stLiBodyFirstMMEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    stLiBodyFirstMMEToENB->TimeStamp = NSP_TDYTH_4G_get_time_now();
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x02;

    printf("ENB解析数据，并重新构造！\n");

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, stLiBodyFirstMMEToENB, D_NSP_TDYTH_NAAAS_4G_LinkageInterruptSecondENBToUE_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,stLiBodyFirstMMEToENB->aucDstId, 
        stLiBodyFirstMMEToENB->aucDstPort, 0)) < 0) 
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("ENB转发数据完毕,step: 2 完成.\n");
    }
    printf("------------------------step2.ENB#Sender ->UE:0x02------------------------\n");
    return 0;
}

/***********************************************************************MME恢复连接***********************************************************************/
/*
 * 功能：连接恢复第二步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_LinkageRecover_Second_ENBtoUE(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB *stReBodyFirstMMEToENB = (St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stReBodyFirstMMEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    stReBodyFirstMMEToENB->TimeStamp = NSP_TDYTH_4G_get_time_now();

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x02;

    printf("ENB解析数据，并重新构造！\n");

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, stReBodyFirstMMEToENB, D_NSP_TDYTH_NAAAS_4G_LinkageRecoverSecondENBToUE_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,stReBodyFirstMMEToENB->aucDstId, 
        stReBodyFirstMMEToENB->aucDstPort, 0)) < 0) 
    {
        printf("%s,[line=%d],errno=%d:%s\n", "MME:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("ENB转发数据完毕,step: 2 完成.\n");
    }
    printf("------------------------step2.ENB#Sender ->UE:0x02------------------------\n");

    return 0;
}

/*
 * 功能：第二步收到测量报告决定切换类型
 * 参数：
 * 返回值：
 *            > 0   成功
 *            < 0   失败
 */

int NSP_TDYTH_4G_HandOver_Second_ENBSend(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char auchobody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(auchobody, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB *stMeasureBodyFirstUEToSENB = (St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB *)auchobody;

    if(NSP_TDYTH_4G_verification_timestamp(stMeasureBodyFirstUEToSENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    int length=(stMeasureBodyFirstUEToSENB->X_coordinates - gs_ix)*(stMeasureBodyFirstUEToSENB->X_coordinates - gs_ix) 
              + (stMeasureBodyFirstUEToSENB->Y_coordinates - gs_iy)*(stMeasureBodyFirstUEToSENB->Y_coordinates - gs_iy);

    if(length>D_NSP_TDYTH_NAAAS_4G_ComLength_S1SCOPE_LEN)
    {
        printf("----------------------通过计算，进行S1切换---------------------\n");
        NSP_TDYTH_4G_S1_HandOver_Second_SENBSendToMME(pcRecvBuf, pcSendBuf);
        return 1;
    }
    else if(length>D_NSP_TDYTH_NAAAS_4G_ComLength_X2SCOPE_LEN)
    {
        printf("----------------------通过计算，进行X2切换---------------------\n");
        NSP_TDYTH_4G_X2_HandOver_Second_SENBSendToTENB(pcRecvBuf, pcSendBuf);
        return 2;
    }
    else if(length>D_NSP_TDYTH_NAAAS_4G_ComLength_INSCOPE_LEN)
    {
        printf("----------------------通过计算，进行站内切换---------------------\n");
        NSP_TDYTH_4G_Intra_HandOver_Second_ENBSendToUE(pcRecvBuf, pcSendBuf);
        return 3;
    }   
    else
    {
        printf("----------------------通过计算，不进行切换---------------------\n");
        return 4;
    }
    return 0;
}
/***********************************************************************上网请求***********************************************************************/
/*
 * 功能：上网请求第二步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet_Second_ENBSendToSGW(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB));

    St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB *stIpBodyFirstUEToENB = (St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIpBodyFirstUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW stIpBodySecondENBToSGW;
    
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x02;
    
    stIpBodySecondENBToSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIpBodySecondENBToSGW.aucIdNumber = 0x0;
    memcpy(stIpBodySecondENBToSGW.aucDstId, stIpBodyFirstUEToENB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN); 
    memcpy(stIpBodySecondENBToSGW.aucSrcId, stIpBodyFirstUEToENB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIpBodySecondENBToSGW.aucSrcPort = stIpBodyFirstUEToENB->aucSrcPort;
    stIpBodySecondENBToSGW.aucDstPort = stIpBodyFirstUEToENB->aucDstPort;
    stIpBodySecondENBToSGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW);

    stIpBodySecondENBToSGW.uiS1_TEID_UL = 1000;                         //隧道端点标识,没有确定
    memcpy(stIpBodySecondENBToSGW.aucDstIp, stIpBodyFirstUEToENB->aucDstIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySecondENBToSGW.aucSrcIp, stIpBodyFirstUEToENB->aucSrcIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySecondENBToSGW.aucGTPDstIp, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySecondENBToSGW.aucGTPSrcIp, D_NSP_TDYTH_NAAAS_ENB1_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySecondENBToSGW.aucGUTI, stIpBodyFirstUEToENB->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stIpBodySecondENBToSGW.aucRequest, stIpBodyFirstUEToENB->aucRequest, D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN);

    memcpy(stIpBodySecondENBToSGW.aucTerminalId,stIpBodyFirstUEToENB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("上网请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIpBodySecondENBToSGW, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：上网请求第八步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet_Eighth_ENBSendToUE(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySeventhSGWToENB));

    St_NSP_TDYTH_NAAAS_4G_IpBodySeventhSGWToENB *stIpBodySeventhSGWToENB = (St_NSP_TDYTH_NAAAS_4G_IpBodySeventhSGWToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIpBodySeventhSGWToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE stIpBodyEighthENBToUE;

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x08;

    stIpBodyEighthENBToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIpBodyEighthENBToUE.aucIdNumber = 0x04;
    memcpy(stIpBodyEighthENBToUE.aucDstId, stIpBodySeventhSGWToENB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIpBodyEighthENBToUE.aucSrcId, stIpBodySeventhSGWToENB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIpBodyEighthENBToUE.aucSrcPort = stIpBodySeventhSGWToENB->aucSrcPort;
    stIpBodyEighthENBToUE.aucDstPort = stIpBodySeventhSGWToENB->aucDstPort;
    stIpBodyEighthENBToUE.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE);

    stIpBodyEighthENBToUE.uiDRBId = 0x00000001;
    memcpy(stIpBodyEighthENBToUE.aucDstIp, stIpBodySeventhSGWToENB->aucDstIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyEighthENBToUE.aucSrcIp, stIpBodySeventhSGWToENB->aucSrcIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyEighthENBToUE.aucGUTI, stIpBodySeventhSGWToENB->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stIpBodyEighthENBToUE.aucResult, stIpBodySeventhSGWToENB->aucResult, D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN);

    memcpy(stIpBodyEighthENBToUE.aucTerminalId, stIpBodySeventhSGWToENB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("上网请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIpBodyEighthENBToUE, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

static int gs_ienb_uiS1_TEID ;
static char gs_cenb_ucBearerID ;
/*
 * 功能：专有承载建立第五步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Fifth_ENBToUE (char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB));

    St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB *stBearerSetUpFourthMMEToENB = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpFourthMMEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE stBearerSetUpFifthENBToUE;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x05;

    stBearerSetUpFifthENBToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpFifthENBToUE.aucIdNumber = 0x0;
    memcpy(stBearerSetUpFifthENBToUE.aucDstId, stBearerSetUpFourthMMEToENB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBearerSetUpFifthENBToUE.aucSrcId, stBearerSetUpFourthMMEToENB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpFifthENBToUE.aucSrcPort = stBearerSetUpFourthMMEToENB->aucSrcPort;
    stBearerSetUpFifthENBToUE.aucDstPort = stBearerSetUpFourthMMEToENB->aucDstPort;
    stBearerSetUpFifthENBToUE.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE);

    memcpy(stBearerSetUpFifthENBToUE.aucQoS, stBearerSetUpFourthMMEToENB->aucQoS, D_NSP_TDYTH_NAAAS_4G_QoS_LEN);
    memcpy(stBearerSetUpFifthENBToUE.aucTFT, stBearerSetUpFourthMMEToENB->aucTFT, D_NSP_TDYTH_NAAAS_4G_TFT_LEN);
    stBearerSetUpFifthENBToUE.ucLBI = stBearerSetUpFourthMMEToENB->ucLBI;
    stBearerSetUpFifthENBToUE.ucBearerID = stBearerSetUpFourthMMEToENB->ucBearerID;

    gs_ienb_uiS1_TEID = stBearerSetUpFourthMMEToENB->uiS1_TEID;   //全局变量
    gs_cenb_ucBearerID = stBearerSetUpFourthMMEToENB->ucBearerID; //全局变量

    memcpy(stBearerSetUpFifthENBToUE.aucTerminalId, stBearerSetUpFourthMMEToENB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpFifthENBToUE, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：专有承载建立第七步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Seventh_ENBToMME(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB));

    St_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB *stBearerSetUpSixthUEToENB = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpSixthUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME stBearerSetUpSeventhENBToMME;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x07;

    stBearerSetUpSeventhENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpSeventhENBToMME.aucIdNumber = 0x0;
    memcpy(stBearerSetUpSeventhENBToMME.aucDstId, stBearerSetUpSixthUEToENB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBearerSetUpSeventhENBToMME.aucSrcId, stBearerSetUpSixthUEToENB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpSeventhENBToMME.aucSrcPort = stBearerSetUpSixthUEToENB->aucSrcPort;
    stBearerSetUpSeventhENBToMME.aucDstPort = stBearerSetUpSixthUEToENB->aucDstPort;
    stBearerSetUpSeventhENBToMME.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME);

    stBearerSetUpSeventhENBToMME.uiS1_TEID = gs_ienb_uiS1_TEID;
    stBearerSetUpSeventhENBToMME.ucBearerID = gs_cenb_ucBearerID;

    memcpy(stBearerSetUpSeventhENBToMME.aucTerminalId, stBearerSetUpSixthUEToENB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpSeventhENBToMME, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：专有承载建立第九步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Ninth_ENBToMME(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB));

    St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB *stBearerSetUpEighthUEToENB = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpEighthUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME stBearerSetUpNinthENBToMME;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x09;

    stBearerSetUpNinthENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpNinthENBToMME.aucIdNumber = 0x0;
    memcpy(stBearerSetUpNinthENBToMME.aucDstId, stBearerSetUpEighthUEToENB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBearerSetUpNinthENBToMME.aucSrcId, stBearerSetUpEighthUEToENB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpNinthENBToMME.aucSrcPort = stBearerSetUpEighthUEToENB->aucSrcPort;
    stBearerSetUpNinthENBToMME.aucDstPort = stBearerSetUpEighthUEToENB->aucDstPort;
    stBearerSetUpNinthENBToMME.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME);

    stBearerSetUpNinthENBToMME.ucResponse='1'; //第九步就是一个响应

    //memcpy(stBearerSetUpNinthENBToMME.ucResponse,"1",D_NSP_TDYTH_NAAAS_4G_ucResponse_LEN);//第九步就是一个响应

    memcpy(stBearerSetUpNinthENBToMME.aucTerminalId, stBearerSetUpEighthUEToENB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpNinthENBToMME, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME));
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/***********************************************************************IMS***********************************************************************/
int NSP_TDYTH_4G_IMSRegister_First_ENBToSGW(char *pcRecvBuf, char *pcSendBuf){
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

    unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
    memcpy(sipbody, pcRecvBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN+ D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *) sipbody;
    NSP_TDYTH_4G_printSIP(gSipHeader , D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
                 D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF_LEN);

    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

    printf("IMS-ENB 第一步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, imsbody,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

int NSP_TDYTH_4G_IMSRegister_Tenth_ENBToUE(char *pcRecvBuf, char *pcSendBuf){
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

    unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
    memcpy(sipbody, pcRecvBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN+ D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *) sipbody;
    NSP_TDYTH_4G_printSIP(gSipHeader , D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE_LEN);

    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

    printf("IMS-ENB 第十步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &imsbody,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

int NSP_TDYTH_4G_IMSRegister_Eleventh_ENBToSGW(char *pcRecvBuf, char *pcSendBuf){
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

    unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
    memcpy(sipbody, pcRecvBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN+ D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *) sipbody;
    NSP_TDYTH_4G_printSIP(gSipHeader , D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF_LEN);

    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

    St_NSP_TDYTH_NAAAS_4G_SIPHeader stSipHeader;
    memcpy(&stSipHeader, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    NSP_TDYTH_4G_printSIP(&stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

    printf("IMS-ENB 第11步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &imsbody,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

int NSP_TDYTH_4G_IMSRegister_Twentieth_ENBToUE(char *pcRecvBuf, char *pcSendBuf){
    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

    unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
    memcpy(sipbody, pcRecvBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN+ D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *) sipbody;
    NSP_TDYTH_4G_printSIP(gSipHeader , D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterTwentiethPCSCFToUE_LEN);

    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    /*  //构建4G头
      St_NSP_TDYTH_NAAAS_4G_Header stHeader;
      stHeader.ucVersion = 0x01;
      stHeader.usModuleCode = 0xA105;
      stHeader.ucReqType = 0x12;
      stHeader.ucSubType = 0x21;
      stHeader.ucUserCategory = 0x01;
      stHeader.ucStep = 0x0a;

      //构建第二层头
      St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
      stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();
      memcpy(stSecHeader.aucSrcId,D_NSP_TDYTH_NAAAS_SGW1_IP ,D_NSP_TDYTH_NAAAS_4G_IP_LEN);
      memcpy(stSecHeader.aucDstId,D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
      stSecHeader.uiSrcPort = D_NSP_TDYTH_NAAAS_SGW1_PORT;
      stSecHeader.uiDstPort = D_NSP_TDYTH_NAAAS_UE_PORT;
      memcpy(stSecHeader.aucTerminalId, "00000000000",D_NSP_TDYTH_NAAAS_4G_ID_LEN);*/
    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x14;

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

    printf("IMS-PGW第20步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, imsbody,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterTwentiethPCSCFToUE_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/***********************************************************************TAU***********************************************************************/
/*
 * 功能：  TAU流程第2步
 * 参数：  @pcRecvBuf [in]  收到的数据
 *        @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_TAU_Second_ENBToMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME stTauBodySecondENBToMME;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB *stTauBodyFirstUEToENB = (St_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyFirstUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x02;

    stTauBodySecondENBToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodySecondENBToMME.aucDstId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodySecondENBToMME.uiDstPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
    memcpy(stTauBodySecondENBToMME.aucSrcId, D_NSP_TDYTH_NAAAS_ENB1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodySecondENBToMME.uiSrcPort = D_NSP_TDYTH_NAAAS_ENB1_PORT;
    stTauBodySecondENBToMME.uiDataLen = 0;

    memcpy(stTauBodySecondENBToMME.aucTerminalId, stTauBodyFirstUEToENB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodySecondENBToMME.aucOldGUTI, stTauBodyFirstUEToENB->aucOldGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stTauBodySecondENBToMME.aucOldTAI, stTauBodyFirstUEToENB->aucOldTAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);
    memcpy(stTauBodySecondENBToMME.aucNewTAI, D_NSP_TDYTH_NAAAS_4G_ENB1_TAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodySecondENBToMME, D_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodySecondENBToMME.aucDstId, stTauBodySecondENBToMME.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/***********************************************************************TAU结束***********************************************************************/

/*
 * 功能：eNB进入监听状态
 * 参数：	无
 * 返回值：  0   成功
 *         <0   失败
 */
void * NSP_TDYTH_4G_ListenAccept_eNB_Function(int* iSockfd)
{
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN];
    char acHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];          //对mme发来的数据包头进行解析

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

    // ENB2(TENB)
    struct sockaddr_in ENB2Sockaddr;
    bzero(&ENB2Sockaddr, sizeof(ENB2Sockaddr));
    ENB2Sockaddr.sin_family = AF_INET;
    ENB2Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB2_PORT);
    ENB2Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB2_IP);

    // MME
    struct sockaddr_in MME1Sockaddr;
    bzero(&MME1Sockaddr, sizeof(MME1Sockaddr));
    MME1Sockaddr.sin_family = AF_INET;
    MME1Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MME1Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    // MME2
    struct sockaddr_in MME2Sockaddr;
    bzero(&MME2Sockaddr, sizeof(MME2Sockaddr));
    MME2Sockaddr.sin_family = AF_INET;
    MME2Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME2_PORT);
    MME2Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME2_IP);

    // ENB1(SENB)
    struct sockaddr_in ENB1Sockaddr;
    bzero(&ENB1Sockaddr, sizeof(ENB1Sockaddr));
    ENB1Sockaddr.sin_family = AF_INET;
    ENB1Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENB1Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);

    struct sockaddr_in SGWSockaddr;
    bzero(&SGWSockaddr, sizeof(SGWSockaddr));
    SGWSockaddr.sin_family = AF_INET;
    SGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_SGW1_PORT);
    SGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_SGW1_IP);

    // if (bind(iSockfd, (struct sockaddr *) &ENBSockaddr, sizeof(ENBSockaddr)) == -1)
    // {
    //     printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
    //     //err_print();
    //     return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
    //     //exit(1);
    // }

    int iSinLen = sizeof(ENB1Sockaddr);

    while(1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));
        iRet = recvfrom(*iSockfd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                        (struct sockaddr *) &ENB1Sockaddr, &iSinLen);
        if (iRet < 0)
        {
            return NULL;
        }
        if (iRet > 0)
        {
            memcpy(acHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *) acHeader;
            switch(pstHeader->ucReqType)
            {
                case 0x00:
                    printf("-------------目前处于注册流程！-------------\n");
                    break;
                case 0x01:
                    printf("-------------目前处于认证流程！-------------\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x01:
                            printf("-------------step1: ENB#Receiver <- MME:0x01-------------\n");
                            NSP_TDYTH_4G_accessAuth_User_Identity_Request_Forward(*iSockfd,cRecvBuf, cSendBuf);
                            break;
                        case 0x03:
                            printf("-------------step3: ENB#Receiver <- UE:0x03-------------\n");
                            if(NSP_TDYTH_4G_accessAuth_User_Identity_Response_Forward(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("用户身份响应发送完毕,step: 4 完成.\n");
                            }
                            printf("-------------step4: ENB#Sender -> MME：0x04-------------\n");
                            break;
                        case 0x09:
                            printf("收到MME请求step：%x, 解析完成,开始进行认证.\n", pstHeader->ucStep);
                            printf("-------------step9: ENB#Receiver <- MME:0x09-------------\n");

                            NSP_TDYTH_4G_accessAuth_User_Auth_Request_Forward(*iSockfd,cRecvBuf, cSendBuf);

                            break;
                        case 0x0b:
                            printf("收到MME请求step：%x, 解析完成,开始进行认证.\n", pstHeader->ucStep);
                            printf("-------------step11: ENB#Receiver <- UE:0x0b-------------\n");

                            if(NSP_TDYTH_4G_accessAuth_User_Auth_Response_Forward(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("用户身份响应发送完毕,step: 12 完成.\n");
                            }
                            printf("-------------step12: UE#Sender -> MME：0x0b-------------\n");
                            break;
                        case 0x0d:
                            printf("收到MME请求step：%x, 解析完成,开始进行认证.\n", pstHeader->ucStep);

                            printf("-------------step13: ENB#Receiver <- UE:0x0d-------------\n");

                            NSP_TDYTH_4G_accessAuth_AuthResult_Forward(*iSockfd, cRecvBuf, cSendBuf);

                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x02:
                    printf("-------------目前处于请求用户信息流程！-------------\n");
                case 0x03:
                    printf("-------------目前处于附着流程！-------------\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x01:
                            printf("-------------step1: ENB#Receiver <- UE:0x01-------------\n");

                            NSP_TDYTH_4G_Second_eNB_Send(*iSockfd, cRecvBuf, cSendBuf);

                            break;
                        case 0x03:
                            printf("-------------step3: ENB#Receiver <- UE:0x03-------------\n");
                            if(NSP_TDYTH_4G_Fourth_eNB_Send(cRecvBuf, cSendBuf) < 0) break;
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("附着请求发送完毕,step: 2 完成.\n");
                                printf_dae_uesend("UE初步附着成功，接下来是认证与IP分配!\n");
                            }
                            printf("-------------step4: ENB#Sender -> MME：0x04-------------\n");
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x04:       // IP分配流程 0x04
                    printf("-------------目前处于IP分配接受流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x05:          // IP分配流程
                            printf("-------------step5. eNB#Receiver ->MME:0x05-------------\n");

                            NSP_TDYTH_4G_IPAllot_Sixth_ENBSendToUE(*iSockfd, cRecvBuf, cSendBuf);

                            break;

                        case 0x07:
                            printf("-------------step7: eNB#Receiver -> UE：0x07-------------\n");
                            printf("-------------附着完成转发流程开启！-------------\n");

                            if(NSP_TDYTH_4G_IPAllot_Eighth_ENBSendToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("附着接受发送完毕,step: 8 完成.\n");
                            }

                            printf("-------------step8: eNB#Sender -> MME：0x08-------------\n");
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x05:       // X2切换流程
                    printf("-------------目前处于X2切换流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x01:

                            printf("-------------step1.SENB#Receiver ->UE:0x01-------------\n");
                            if(NSP_TDYTH_4G_X2_HandOver_Second_SENBSendToTENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB2Sockaddr, sizeof(ENB2Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("HandOver Request发送完毕,step: 2 完成.\n");
                            }

                            printf("-------------step2.SENB#Sender ->TENB:0x02-------------\n");
                            break;

                        case 0x02:

                            printf("-------------step2.TENB#Receiver ->SENB:0x02-------------\n");
                            if(NSP_TDYTH_4G_X2_HandOver_Third_TENBSendToSENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("切换请求确认发送完毕,step: 3 完成.\n");
                            }

                            printf("-------------step3.TENB#Sender ->SENB:0x03-------------\n");
                            break;

                        case 0x03:          // 9/附着完成转发流程
                            printf("-------------step3:SENB#Receiver -> TENB：0x03-------------\n");

                            if(NSP_TDYTH_4G_X2_HandOver_Fourth_SENBSendToUE(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("RRC连接重新配置请求发送完毕,step: 4 完成.\n");
                            }

                            printf("-------------step4: eNB#Sender -> UE：0x04-------------\n");
                            //sleep(5);
                            NSP_TDYTH_4G_X2_HandOver_Fifth_SENBSendToTENB(cRecvBuf, cSendBuf);

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB2Sockaddr, sizeof(ENB2Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("SN状态转移请求发送完毕,step: 5 完成.\n");
                            }

                            printf("-------------step5:SENB#Sender -> TENB：0x05-------------\n");
                            break;
                        case 0x05:

                            printf("-------------step5.TENB#Receiver ->SENB:0x05-------------\n");

                            printf("收到状态转移请求与转发数据！\n");
                            break;
                        case 0x06:

                            printf("-------------step6.TENB#Receiver ->UE:0x06-------------\n");
                            if(NSP_TDYTH_4G_X2_HandOver_Seventh_TENBSendToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("路径切换请求发送完毕,step: 7 完成.\n");
                            }

                            printf("-------------step7.TENB#Sender ->MME:0x07-------------\n");
                            break;

                        case 0x08:

                            printf("-------------step8.TENB#Receiver ->MME:0x08-------------\n");
                            if(NSP_TDYTH_4G_X2_HandOver_Ninth_TENBSendToSENB(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("UE消息释放请求发送完毕,step: 9 完成.\n");
                            }

                            printf("-------------step9.TENB#Sender ->SENB:0x09-------------\n");
                            break;
                        case 0x09:
                            printf("X2 finish!\n");
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x06:       // S1切换流程
                    printf("-------------目前处于S1切换流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x01:

                            printf("-------------step1.SENB#Receiver ->UE:0x01-------------\n");
                            if(NSP_TDYTH_4G_S1_HandOver_Second_SENBSendToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("切换需要请求发送完毕,step: 2 完成.\n");
                            }

                            printf("-------------step2.SENB#Sender ->MME:0x02-------------\n");
                            break;

                        case 0x03:

                            printf("-------------step3.TENB#Receiver ->MME:0x03-------------\n");
                            if(NSP_TDYTH_4G_S1_HandOver_Fourth_TENBSendToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("切换请求确认发送完毕,step: 4 完成.\n");
                            }

                            printf("-------------step4.TENB#Sender ->MME:0x04-------------\n");
                            break;

                        case 0x05:
                            printf("-------------step5:SENB#Receiver -> MME：0x05-------------\n");

                            if(NSP_TDYTH_4G_S1_HandOver_Sixth_SENBSendToUE(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("RRC连接重新配置请求发送完毕,step: 6 完成.\n");
                            }

                            printf("-------------step6: SENB#Sender -> UE：0x06-------------\n");
                            //sleep(5);
                            NSP_TDYTH_4G_S1_HandOver_Seventh_SENBSendToMME(cRecvBuf, cSendBuf);

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("ENB SN状态转移请求发送完毕,step: 7 完成.\n");
                            }

                            printf("-------------step7:SENB#Sender -> TENB：0x07-------------\n");
                            break;
                        case 0x08:

                            printf("-------------step8.TENB#Receiver ->MME:0x08-------------\n");

                            printf("MME状态转移请求与转发数据！\n");
                            break;
                        case 0x09:

                            printf("-------------step9.TENB#Receiver ->MME:0x09--------------\n");
                            if(NSP_TDYTH_4G_S1_HandOver_Tenth_TENBSendToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("路径切换请求发送完毕,step: 10 完成.\n");
                            }

                            printf("-------------step10.TENB#Sender ->MME:0x0a-------------\n");
                            break;

                        case 0x0b:

                            printf("-------------step11.SENB#Receiver ->MME:0x0b-------------\n");
                            if (NSP_TDYTH_4G_S1_HandOver_Twelfth_SENBSendToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            }
                            else
                            {
                                printf("UE消息释放请求发送完毕,step: 12 完成.\n");
                            }

                            printf("-------------step12.TENB#Sender ->SENB:0x0b-------------\n");
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x07:   //站内切换
                    printf("-------------目前处于站内切换流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x01:
                            printf("-------------step1.ENB#Receiver ->UE:0x01-------------\n");
                            if(NSP_TDYTH_4G_Intra_HandOver_Second_ENBSendToUE(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN,
                                               0, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("HandOver Request发送完毕,step: 2 完成.\n");
                            }

                            printf("-------------step2.ENB#Sender ->UE:0x02-------------\n");
                            break;
                        case 0x03:
                            printf("-------------站内切换成功-------------!\n");
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x08: //MME连接中断
                    printf("-------------目前处于MME连接中断流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x01:
                            printf("-------------step1.ENB#Receiver ->MME:0x01-------------\n");

                            NSP_TDYTH_4G_LinkageInterrupt_Second_ENBtoUE(*iSockfd, cRecvBuf, cSendBuf);

                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x09: //MME连接中断
                    printf("-------------目前处于MME恢复连接流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x01:
                            printf("-------------step1.ENB#Receiver ->MME:0x01-------------\n");

                            NSP_TDYTH_4G_LinkageRecover_Second_ENBtoUE(*iSockfd, cRecvBuf, cSendBuf);

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x0a:
                    printf("-------------目前处接受测量报告阶段！-------------\n");
                    printf("-------------step1.ENB#Receiver ->UE:0x01-------------\n");
                    iRet = NSP_TDYTH_4G_HandOver_Second_ENBSend(cRecvBuf, cSendBuf);
                    switch (iRet)   // 判断步骤
                    {
                        case -1:
                            break;
                        case 0:
                            break;
                        case 1:
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step2.SENB#Sender ->MME:0x02-------------\n");
                            }
                            break;
                        case 2:
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB2Sockaddr, sizeof(ENB2Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step2.SENB#Sender ->TENB:0x02-------------\n");
                            }
                            break;
                        case 3:
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN,
                                               0, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step2.ENB#Sender ->UE:0x02-------------\n");
                            }
                            break;
                        case 4:
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "iRet errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x10: //IP上网请求
                    printf("--------------目前处于上网流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x01:
                            printf("-------------step1.ENB#Receiver ->UE:0x01-------------\n");
         
                            if(NSP_TDYTH_4G_IpNet_Second_ENBSendToSGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } 
                            else 
                            {
                                printf("-------------step2: ENB#Sender -> SGW：0x02-------------\n");
                            }
                            break;

                        case 0x07:
                            printf("-------------step7.ENB#Receiver ->SGW:0x07-------------\n");
         
                            if(NSP_TDYTH_4G_IpNet_Eighth_ENBSendToUE(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } 
                            else 
                            {
                                printf("-------------step8: ENB#Sender -> UE：0x08-------------\n");
                            }
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
                        case 0x04:
                            printf("-------------step4.ENB#Receiver ->MME:0x04-------------\n");
                            if (NSP_TDYTH_4G_BearerSetUp_Fifth_ENBToUE(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step5.ENB#Sender ->UE:0x05-------------\n");
                            }
                            break;
                        case 0x06:
                            printf("-------------step6.ENB#Receiver ->UE:0x06-------------\n");
                            if (NSP_TDYTH_4G_BearerSetUp_Seventh_ENBToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step7.ENB#Sender ->MME:0x07-------------\n");
                            }
                            break;
                        case 0x08:
                            printf("-------------step8.ENB#Receiver ->UE:0x08-------------\n");
                            if (NSP_TDYTH_4G_BearerSetUp_Ninth_ENBToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &MME1Sockaddr, sizeof(MME1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step9.ENB#Sender ->MME:0x09-------------\n");
                            }
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x0c:
                    printf("-------------目前处于IMS流程！-------------\n");
                    switch (pstHeader->ucStep) {
                        case 0x01:
                            printf("-------------step1.ENB#Receiver ->UE:0x01-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_First_ENBToSGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                           0, (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0) {
                                 printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                   strerror(errno));
                                  return NULL;
                            } else {
                            printf("--------------step1.ENB#Sender ->SGW:0x01-------------\n");
                            }
                            break;
                        case 0x0a:
                            printf("-------------step10.ENB#Receiver ->SGW:0x10-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Tenth_ENBToUE(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step10.ENB#Sender ->UE:0x0a-------------\n");
                            }
                            break;
                        case 0x0b:
                            printf("-------------step11.ENB#Receiver ->UE:0x0b-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Eleventh_ENBToSGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step11.ENB#Sender ->UE:0x0b-------------\n");
                            }
                            break;
                        case 0x14:
                            printf("-------------step20.ENB#Receiver ->SGW:0x14-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Twentieth_ENBToUE(cRecvBuf, cSendBuf) < 0) break;
                            if ((iRet = sendto(*iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &UESockaddr, sizeof(UESockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return NULL;
                            } else {
                                printf("-------------step20.ENB#Sender ->UE:0x14-------------\n");
                            }
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x13: //TAU请求
                    printf("\n----------目前处于TAU流程----------\n\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x01:
                            printf("----------TAU: step1. ENB#Receiver -> UE：TAU Request----------\n");
                            if(NSP_TDYTH_4G_TAU_Second_ENBToMME(cRecvBuf, cSendBuf, *iSockfd) < 0 ) {
                                printf("----------TAU: step2. SEND ERROR----------\n");
                                break;
                            }else{
                                printf("----------TAU: step2. ENB#Sender -> New MME：TAU Request----------\n");
                            }
                            break;
                        default:
                            printf("收到TAU包的步骤信息无法识别！\n");
                            break;
                    }
                    break;
                default:
                    printf("%s ReqType=%d,[line=%d],errno=%d:%s\n", "ReqType errno.", pstHeader->ucReqType, __LINE__, errno, strerror(errno));
                    break;
            }
        }
    }
    return NULL;
}

/*
 * 功能：构造广播包
 * 参数： @xxx [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_Broadcast_Enb_Send(char *pcSendBuf)
{
    // 构造广播包内容（以下数据仅针对ENB1，之后可以修改为动态数据）
    //printf("开始构造广播包内容\n");
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memset(pcSendBuf, 0, sizeof(pcSendBuf));

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x11;              // 类型为广播包；其他不重要，置为1
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    gsSelfENB.uiDataLen = D_NSP_TDYTH_NAAAS_4G_BroadcastBody_LEN;
    gsSelfENB.uiENBID = D_NSP_TDYTH_NAAAS_4G_ENB1_ENBID;
    gsSelfENB.uiPCI = D_NSP_TDYTH_NAAAS_4G_ENB1_PCI;
    gsSelfENB.uiEARFCN = D_NSP_TDYTH_NAAAS_4G_ENB1_EARFCN;
    gsSelfENB.iRSRP = D_NSP_TDYTH_NAAAS_4G_ENB1_RSRP;
    gsSelfENB.iCellRP = D_NSP_TDYTH_NAAAS_4G_ENB1_CellRP;
    gsSelfENB.iQrxlevmin = D_NSP_TDYTH_NAAAS_4G_ENB1_Qrxlevmin;
    memcpy(gsSelfENB.aucTAI, D_NSP_TDYTH_NAAAS_4G_ENB1_TAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);

    memcpy(gsSelfENB.aucENBIp, D_NSP_TDYTH_NAAAS_ENB1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    gsSelfENB.uiENBPort = D_NSP_TDYTH_NAAAS_ENB1_PORT;

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &gsSelfENB, D_NSP_TDYTH_NAAAS_4G_BroadcastBody_LEN);
    //printf("广播包内容构造完毕\n");

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}


/*
 * 功能：向所有UE发送广播包，其中包含此基站的相关信息
 * 参数：  无
 * 返回值：  0   成功
 *         <0   失败
 */
int NSP_TDYTH_4G_Broadcast_eNB_Function()
{
    int iRet;
    int ENBSockfd;

    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    ENBSockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // ENB
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);

    // UE1
    struct sockaddr_in UE1Sockaddr;
    bzero(&UE1Sockaddr, sizeof(UE1Sockaddr));
    UE1Sockaddr.sin_family = AF_INET;
    UE1Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UE1Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);
    // UE2
    struct sockaddr_in UE2Sockaddr;
    bzero(&UE2Sockaddr, sizeof(UE2Sockaddr));
    UE2Sockaddr.sin_family = AF_INET;
    UE2Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE2_PORT);
    UE2Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE2_IP);

    /*设置socket属性，端口可以重用*/
    int opt = SO_REUSEADDR;
    setsockopt(ENBSockfd,SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
    setnonblocking(ENBSockfd);

    NSP_TDYTH_4G_Broadcast_Enb_Send(cSendBuf);

    //持续发送广播包
    while(1)
    {
        if ((iRet = sendto(ENBSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                           (struct sockaddr *) &UE1Sockaddr, sizeof(UE1Sockaddr))) < 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "broadcast err", __LINE__, errno, strerror(errno));
            return -1;
        } else {
            //printf("ENB->UE1 广播包发送成功!\n");
        }
        sleep(D_NSP_TDYTH_NAAAS_4G_EnbBroadcast_Interval);
    }

    close(ENBSockfd);

    return 0;
}


/*
 * 功能：采用epoll模型，创建线程处理消息
 * 参数：  无
 * 返回值：  0   成功
 *         <0   失败
 */
int NSP_TDYTH_4G_Epoll_eNB_Function()
{
    int iFD,kdpfd,nfds,n,curfds;

    struct epoll_event ev;
    struct epoll_event events[D_NSP_TDYTH_NAAAS_4G_MAXEPOLLSIZE];
    struct rlimit rt;

    pthread_t thread,thread2;
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
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);

    if (bind(iFD, (struct sockaddr *) &ENBSockaddr, sizeof(struct sockaddr)) == -1)
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
    // 建立一个线程，用来持续发送发广播包
    if(pthread_create(&thread2, NULL, (void*)NSP_TDYTH_4G_Broadcast_eNB_Function, NULL)){
        perror("pthread2_creat error!\n");
        exit(-1);
    }else{
        printf("播放广播包子线程开启成功\n");
    }


    printf("ENB开启监听状态\n");
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
                if(pthread_create(&thread,&attr,(void*)NSP_TDYTH_4G_ListenAccept_eNB_Function,(void*)&(events[n].data.fd)))
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
    NSP_TDYTH_4G_Epoll_eNB_Function();
    return 0;
}



