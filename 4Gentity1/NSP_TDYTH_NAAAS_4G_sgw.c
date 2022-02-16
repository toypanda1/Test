 /*
 * 文件功能：S-GW服务端主函数
 * 作者：寇飞翔
 * 创建日期：2020/06/22
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


 /**********************************************************
 * 函数名：	 NSP_TDYTH_4G_IPAllot_Second_SGWSendToPGW
 * 功能：    SGW端--IP分配流程第2步，SGW发送至PGW
 * 参数：	 @	 [pcRecvBuf in]	接收缓冲区
 *           @	 [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          <0   失败
 **********************************************************/
int NSP_TDYTH_4G_IPAllot_Second_SGWSendToPGW(char *pcRecvBuf, char *pcSendBuf)
{
    char acbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW stIPAllotBodySecondSGWToPGW;

    memcpy(acbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW_LEN);
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW *stIPAllotBodyFirstMMEToSGW = (St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW*)acbody;
    if(NSP_TDYTH_4G_verification_timestamp(stIPAllotBodyFirstMMEToSGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x02;

    stIPAllotBodySecondSGWToPGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIPAllotBodySecondSGWToPGW.aucIdNumber = 0x0;
    memcpy(stIPAllotBodySecondSGWToPGW.aucDstId, stIPAllotBodyFirstMMEToSGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIPAllotBodySecondSGWToPGW.aucSrcId, stIPAllotBodyFirstMMEToSGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIPAllotBodySecondSGWToPGW.aucDstPort = stIPAllotBodyFirstMMEToSGW->aucDstPort;
    stIPAllotBodySecondSGWToPGW.aucSrcPort = stIPAllotBodyFirstMMEToSGW->aucSrcPort;
    stIPAllotBodySecondSGWToPGW.iDataLen = 0;

    memcpy(stIPAllotBodySecondSGWToPGW.aucIMSI, stIPAllotBodyFirstMMEToSGW->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    stIPAllotBodySecondSGWToPGW.ucPDNType = 0x11;
    memcpy(stIPAllotBodySecondSGWToPGW.aucPDNAddress, "0.0.0.0", D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN);
    stIPAllotBodySecondSGWToPGW.ucPCO = 0x01;

    memcpy(stIPAllotBodySecondSGWToPGW.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("请求会话创建数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    printf("the send step is IP Allot %x\n",stHeader.ucStep);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIPAllotBodySecondSGWToPGW,
            D_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW_LEN); //数据体填入发送缓冲区

    return  0;
}



/**********************************************************
 * 函数名：	 NSP_TDYTH_4G_IPAllot_Fourth_SGWSendToMME
 * 功能：    SGW端--IP分配流程第4步，SGW发送至MME
 * 参数：	 @ 	 [pcRecvBuf in]	接收缓冲区
 *           @	 [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          <0   失败
 *********************************************************/
int NSP_TDYTH_4G_IPAllot_Fourth_SGWSendToMME(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME stIPAllotBodyFourthSGWToMME;
    char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW_LEN);
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW *stIPAllotBodyThirdPGWToSGW = (St_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIPAllotBodyThirdPGWToSGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x04;

//    memcpy(stIPAllotBodyFourthSGWToMME.aucTimeStamp, "2020-06-06", D_NSP_TDYTH_NAAAS_4G_TimeStamp_LEN);
//    stIPAllotBodyFourthSGWToMME.aucIdNumber = 0x0;
//    memcpy(stIPAllotBodyFourthSGWToMME.aucDstId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
//    memcpy(stIPAllotBodyFourthSGWToMME.aucSrcId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);
//    stIPAllotBodyFourthSGWToMME.uiDataLen = 0;
//
//    memcpy(stIPAllotBodyFourthSGWToMME.aucIMSI, "000111000000000", D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
//    stIPAllotBodyFourthSGWToMME.ucPDNType = 0x11;
//    memcpy(stIPAllotBodyFourthSGWToMME.aucPDNAddress, "192.168.3.156", D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN);
//    stIPAllotBodyFourthSGWToMME.ucPCO = 0x01;
//
//    memcpy(stIPAllotBodyFourthSGWToMME.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    memcpy(&stIPAllotBodyFourthSGWToMME, stIPAllotBodyThirdPGWToSGW, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME_LEN);

    printf("请求会话创建响应数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    printf("the send step is IP Allot %x\n",stHeader.ucStep);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIPAllotBodyFourthSGWToMME, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME_LEN); //数据体填入发送缓冲区

    return  0;
}


/*
 * 功能：上网请求第三步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet_Third_SGWSendToPGW(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;
    
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW));

    St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW *stIpBodySecondENBToSGW = (St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIpBodySecondENBToSGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW stIpBodyThirdSGWToPGW;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x03;
    
    stIpBodyThirdSGWToPGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIpBodyThirdSGWToPGW.aucIdNumber = 0x0;
    memcpy(stIpBodyThirdSGWToPGW.aucDstId, stIpBodySecondENBToSGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN); 
    memcpy(stIpBodyThirdSGWToPGW.aucSrcId, stIpBodySecondENBToSGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIpBodyThirdSGWToPGW.aucSrcPort = stIpBodySecondENBToSGW->aucSrcPort;
    stIpBodyThirdSGWToPGW.aucDstPort = stIpBodySecondENBToSGW->aucDstPort;
    stIpBodyThirdSGWToPGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW);

    stIpBodyThirdSGWToPGW.uiS5_TEID_UL = 2000;
    memcpy(stIpBodyThirdSGWToPGW.aucDstIp, stIpBodySecondENBToSGW->aucDstIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyThirdSGWToPGW.aucSrcIp, stIpBodySecondENBToSGW->aucSrcIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyThirdSGWToPGW.aucGTPDstIp, D_NSP_TDYTH_NAAAS_PGW_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyThirdSGWToPGW.aucGTPSrcIp, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyThirdSGWToPGW.aucGUTI, stIpBodySecondENBToSGW->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stIpBodyThirdSGWToPGW.aucRequest, stIpBodySecondENBToSGW->aucRequest, D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN);   

    memcpy(stIpBodyThirdSGWToPGW.aucTerminalId, stIpBodySecondENBToSGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("上网请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIpBodyThirdSGWToPGW, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：上网请求第七步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet_Seventh_SGWToENB(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW));

    St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW *stIpBodySixthPGWToSGW = (St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIpBodySixthPGWToSGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IpBodySeventhSGWToENB stIpBodySeventhSGWToENB;

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x07;

    stIpBodySeventhSGWToENB.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIpBodySeventhSGWToENB.aucIdNumber = 0x0;
    memcpy(stIpBodySeventhSGWToENB.aucDstId, stIpBodySixthPGWToSGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIpBodySeventhSGWToENB.aucSrcId, stIpBodySixthPGWToSGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIpBodySeventhSGWToENB.aucSrcPort = stIpBodySixthPGWToSGW->aucSrcPort;
    stIpBodySeventhSGWToENB.aucDstPort = stIpBodySixthPGWToSGW->aucDstPort;
    stIpBodySeventhSGWToENB.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySeventhSGWToENB);

    stIpBodySeventhSGWToENB.uiS1_TEID_DL = 1000;                         
    memcpy(stIpBodySeventhSGWToENB.aucDstIp, stIpBodySixthPGWToSGW->aucDstIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySeventhSGWToENB.aucSrcIp, stIpBodySixthPGWToSGW->aucSrcIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySeventhSGWToENB.aucGTPDstIp, D_NSP_TDYTH_NAAAS_ENB1_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySeventhSGWToENB.aucGTPSrcIp, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySeventhSGWToENB.aucGUTI, stIpBodySixthPGWToSGW->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stIpBodySeventhSGWToENB.aucResult, stIpBodySixthPGWToSGW->aucResult, D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN);

    memcpy(stIpBodySeventhSGWToENB.aucTerminalId,stIpBodySixthPGWToSGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("上网请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIpBodySeventhSGWToENB, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySeventhSGWToENB));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：专有承载建立第三步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
 int NSP_TDYTH_4G_BearerSetUp_Third_SGWToMME(char *pcRecvBuf, char *pcSendBuf)
 {
     int iRet;

     unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW));

     St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW *stBearerSetUpSecondPGWToSGW = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW *)aucbody;

     if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpSecondPGWToSGW->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }

     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME stBearerSetUpThirdSGWToMME;
     memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x03;

     stBearerSetUpThirdSGWToMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
     stBearerSetUpThirdSGWToMME.aucIdNumber = 0x0;
     memcpy(stBearerSetUpThirdSGWToMME.aucDstId, stBearerSetUpSecondPGWToSGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     memcpy(stBearerSetUpThirdSGWToMME.aucSrcId, stBearerSetUpSecondPGWToSGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stBearerSetUpThirdSGWToMME.aucSrcPort = stBearerSetUpSecondPGWToSGW->aucSrcPort;
     stBearerSetUpThirdSGWToMME.aucDstPort = stBearerSetUpSecondPGWToSGW->aucDstPort;
     stBearerSetUpThirdSGWToMME.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME);

     memcpy(stBearerSetUpThirdSGWToMME.aucIMSI, stBearerSetUpSecondPGWToSGW->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
     stBearerSetUpThirdSGWToMME.ucPTI = stBearerSetUpSecondPGWToSGW->ucPTI;
     memcpy(stBearerSetUpThirdSGWToMME.aucQoS, stBearerSetUpSecondPGWToSGW->aucQoS, D_NSP_TDYTH_NAAAS_4G_QoS_LEN);
     memcpy(stBearerSetUpThirdSGWToMME.aucTFT, stBearerSetUpSecondPGWToSGW->aucTFT, D_NSP_TDYTH_NAAAS_4G_TFT_LEN);
     stBearerSetUpThirdSGWToMME.uiS1_TEID = 0x00000000;//隧道端点标识???(与上一步不同上一步是S5_TEID，这个是S1_TEID)
     stBearerSetUpThirdSGWToMME.ucLBI = stBearerSetUpSecondPGWToSGW->ucLBI;

     memcpy(stBearerSetUpThirdSGWToMME.aucTerminalId, stBearerSetUpSecondPGWToSGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     printf("专有承载建立数据构造完毕.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpThirdSGWToMME, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME));

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }

 /*
 * 功能：专有承载建立第十一步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
 int NSP_TDYTH_4G_BearerSetUp_Eleventh_SGWToPGW(char *pcRecvBuf, char *pcSendBuf)
 {

     int iRet;

     unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW));

     St_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW *stBearerSetUpTenthMMEToSGW = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW *)aucbody;

     if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpTenthMMEToSGW->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }

     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     St_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW stBearerSetUpEleventhSGWToPGW;
     memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x0b;

     stBearerSetUpEleventhSGWToPGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
     stBearerSetUpEleventhSGWToPGW.aucIdNumber = 0x0;
     memcpy(stBearerSetUpEleventhSGWToPGW.aucDstId, stBearerSetUpTenthMMEToSGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     memcpy(stBearerSetUpEleventhSGWToPGW.aucSrcId, stBearerSetUpTenthMMEToSGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stBearerSetUpEleventhSGWToPGW.aucSrcPort = stBearerSetUpTenthMMEToSGW->aucSrcPort;
     stBearerSetUpEleventhSGWToPGW.aucDstPort = stBearerSetUpTenthMMEToSGW->aucDstPort;
     stBearerSetUpEleventhSGWToPGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME);

     stBearerSetUpEleventhSGWToPGW.uiS5_TEID = 2000;//写什么？是SGW在第三步存储下来，然后发送第十一步直接使用吗？还是自己构造一个
     stBearerSetUpEleventhSGWToPGW.ucBearerID=stBearerSetUpTenthMMEToSGW->ucBearerID;

     memcpy(stBearerSetUpEleventhSGWToPGW.aucTerminalId, stBearerSetUpTenthMMEToSGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     printf("专有承载建立数据构造完毕.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpEleventhSGWToPGW, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW));

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }

 /***********************************************************************IMS***********************************************************************/
 int NSP_TDYTH_4G_IMSRegister_First_SGWToPGW(char *pcRecvBuf, char *pcSendBuf){
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
     //stHeader.ucStep = 0x01;

     //构建第二层头
     St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
     memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

     printf("IMS-SGW 第一步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, imsbody,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF_LEN);


     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

 }

 int NSP_TDYTH_4G_IMSRegister_Tenth_SGWToENB(char *pcRecvBuf, char *pcSendBuf)
 {
     unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];

     memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

     unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
     memcpy(sipbody, pcRecvBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN+ D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *) sipbody;
     NSP_TDYTH_4G_printSIP(gSipHeader , D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

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
     stHeader.ucStep = 0x0a;

     //构建第二层头
     St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
     memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

     printf("IMS-SGW第十步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, imsbody,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE_LEN);

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }

 int NSP_TDYTH_4G_IMSRegister_Eleventh_SGWToPGW(char *pcRecvBuf, char *pcSendBuf){
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

     printf("IMS-ENB 第11步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &imsbody,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF_LEN);

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

 }

 int NSP_TDYTH_4G_IMSRegister_Twentieth_SGWToENB(char *pcRecvBuf, char *pcSendBuf)
 {
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
 * 功能：  TAU流程第8步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
 NSP_TDYTH_4G_TAU_Eighth_NewSGWToPGW(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
 {
     int iRet;
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     St_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW stTauBodyEighthNewSGWToPGW;
     unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW_LEN);
     St_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW *stTauBodySeventhNewMMEToNewSGW = (St_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW *)aucbody;

     if(NSP_TDYTH_4G_verification_timestamp(stTauBodySeventhNewMMEToNewSGW->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }

     memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x08;

     stTauBodyEighthNewSGWToPGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
     memcpy(stTauBodyEighthNewSGWToPGW.aucDstId, D_NSP_TDYTH_NAAAS_PGW_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stTauBodyEighthNewSGWToPGW.uiDstPort = D_NSP_TDYTH_NAAAS_PGW_PORT;
     memcpy(stTauBodyEighthNewSGWToPGW.aucSrcId, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stTauBodyEighthNewSGWToPGW.uiSrcPort = D_NSP_TDYTH_NAAAS_SGW1_PORT;
     stTauBodyEighthNewSGWToPGW.uiDataLen = 0;

     memcpy(stTauBodyEighthNewSGWToPGW.aucTerminalId, stTauBodySeventhNewMMEToNewSGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     memcpy(stTauBodyEighthNewSGWToPGW.aucIMSI, stTauBodySeventhNewMMEToNewSGW->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyEighthNewSGWToPGW, D_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW_LEN);

     if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                             stTauBodyEighthNewSGWToPGW.aucDstId, stTauBodyEighthNewSGWToPGW.uiDstPort, 0)) < 0)
     {
         printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
         return NULL;
     }

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }

 /*
 * 功能：  TAU流程第10步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
 NSP_TDYTH_4G_TAU_Tenth_NewSGWToNewMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
 {
     int iRet;
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     St_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME stTauBodyTenthNewSGWToNewMME;
     unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW_LEN);
     St_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW *stTauBodyNinthPGWToNewSGW = (St_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW *)aucbody;

     if(NSP_TDYTH_4G_verification_timestamp(stTauBodyNinthPGWToNewSGW->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }

     memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x0a;

     stTauBodyTenthNewSGWToNewMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
     memcpy(stTauBodyTenthNewSGWToNewMME.aucDstId, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stTauBodyTenthNewSGWToNewMME.uiDstPort = D_NSP_TDYTH_NAAAS_MME1_PORT;
     memcpy(stTauBodyTenthNewSGWToNewMME.aucSrcId, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stTauBodyTenthNewSGWToNewMME.uiSrcPort = D_NSP_TDYTH_NAAAS_SGW1_PORT;
     stTauBodyTenthNewSGWToNewMME.uiDataLen = 0;

     memcpy(stTauBodyTenthNewSGWToNewMME.aucTerminalId, stTauBodyNinthPGWToNewSGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     memcpy(stTauBodyTenthNewSGWToNewMME.aucIMSI, stTauBodyNinthPGWToNewSGW->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyTenthNewSGWToNewMME, D_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME_LEN);

     if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                             stTauBodyTenthNewSGWToNewMME.aucDstId, stTauBodyTenthNewSGWToNewMME.uiDstPort, 0)) < 0)
     {
         printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
         return NULL;
     }

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }

 /*
 * 功能：  TAU流程第16步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
 NSP_TDYTH_4G_TAU_Sixteenth_OldSGWToOldMME(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
 {
     int iRet;
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     St_NSP_TDYTH_NAAAS_4G_TauBodySixteenthOldSGWToOldMME stTauBodySixteenthOldSGWToOldMME;
     unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW_LEN);
     St_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW *stTauBodyFifteenthOldMMEToOldSGW = (St_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW *)aucbody;

     if(NSP_TDYTH_4G_verification_timestamp(stTauBodyFifteenthOldMMEToOldSGW->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }

     memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x10;

     stTauBodySixteenthOldSGWToOldMME.TimeStamp = NSP_TDYTH_4G_get_time_now();
     memcpy(stTauBodySixteenthOldSGWToOldMME.aucDstId, stTauBodyFifteenthOldMMEToOldSGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stTauBodySixteenthOldSGWToOldMME.uiDstPort = stTauBodyFifteenthOldMMEToOldSGW->uiSrcPort;
     memcpy(stTauBodySixteenthOldSGWToOldMME.aucSrcId, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     stTauBodySixteenthOldSGWToOldMME.uiSrcPort = D_NSP_TDYTH_NAAAS_SGW1_PORT;
     stTauBodySixteenthOldSGWToOldMME.uiDataLen = 0;

     memcpy(stTauBodySixteenthOldSGWToOldMME.aucTerminalId, stTauBodyFifteenthOldMMEToOldSGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
     memcpy(stTauBodySixteenthOldSGWToOldMME.aucIMSI, stTauBodyFifteenthOldMMEToOldSGW->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodySixteenthOldSGWToOldMME, D_NSP_TDYTH_NAAAS_4G_TauBodySixteenthOldSGWToOldMME_LEN);

     if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                             stTauBodySixteenthOldSGWToOldMME.aucDstId, stTauBodySixteenthOldSGWToOldMME.uiDstPort, 0)) < 0)
     {
         printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
         return NULL;
     }

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }
 /***********************************************************************TAU结束***********************************************************************/

 /**********************************************************
 * 函数名：	 NSP_TDYTH_4G_SGW_ListenAccept_Function
 * 功能：    SGW端--接受监听函数
 * 参数：	 void
 * 返回值：  0   成功
 *          <0   失败
 *********************************************************/
int NSP_TDYTH_4G_SGW_ListenAccept_Function()
{
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];

    struct sockaddr_in ENB1Sockaddr;
    bzero(&ENB1Sockaddr, sizeof(ENB1Sockaddr));
    ENB1Sockaddr.sin_family = AF_INET;
    ENB1Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB1_PORT);
    ENB1Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB1_IP);

    // MME
    //int iSockfdMME = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    // PGW
    //int iSockfdPGW = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in PGWSockaddr;
    bzero(&PGWSockaddr, sizeof(PGWSockaddr));
    PGWSockaddr.sin_family = AF_INET;
    PGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PGW_PORT);
    PGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PGW_IP);

    // SGW
    int iSockfdSGW = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in SGWSockaddr;
    bzero(&SGWSockaddr, sizeof(SGWSockaddr));
    SGWSockaddr.sin_family = AF_INET;
    SGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_SGW1_PORT);
    SGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_SGW1_IP);
    int iSGWAddrLen = sizeof(SGWSockaddr);

    // bind，成功返回0，出错返回-1
    if (bind(iSockfdSGW, (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));

        iRet = recvfrom(iSockfdSGW, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_SGW_BUFF_LEN, 0,
                        (struct sockaddr *) &SGWSockaddr, &iSGWAddrLen);

        if (iRet > 0)
        {
            memcpy(cHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *)cHeader;             // 解析从mme发来的包头

            switch (pstHeader->ucReqType)   // 判断请求类别
            {
                case 0x04:          // IP分配流程 0x04
                    printf("-------------目前处于IP分配流程！--------------\n");
                    switch (pstHeader->ucStep)     // 判断步骤码
                    {
                        case 0x01:      // 第 1 步  接收从MME发送的用户会话创建请求
                            printf("-------------step1. SGW#Receiver <- MME：0x01-------------\n");

                            if(NSP_TDYTH_4G_IPAllot_Second_SGWSendToPGW(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_SGW_BUFF_LEN, 0, (struct sockaddr *) &PGWSockaddr, sizeof(PGWSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("用户注册响应发送完毕,step: 2 完成.\n");
                            }
                            printf("-------------step2: SGW#Sender -> PGW：0x02-------------\n");

                            break;

                        case 0x03:      // 第 3 步  接收从PGW发送的用户会话创建请求
                            printf("-------------step3. SGW#Receiver <- PGW：0x03-------------\n");

                            if(NSP_TDYTH_4G_IPAllot_Fourth_SGWSendToMME(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_SGW_BUFF_LEN, 0, (struct sockaddr *) &MMESockaddr, sizeof(MMESockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("用户注册响应发送完毕,step: 4 完成.\n");
                            }
                            printf("-------------step4: SGW#Sender -> MME：0x04-------------\n");

                            break;


                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;

                    }

                    break;

                case 0x10: //IP上网
                    printf("-------------目前处于上网流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x02:      // 第 2 步  接收从ENB发送的上网数据
                            printf("-------------step2.SGW#Receiver ->ENB:0x02-------------\n");
         
                            if(NSP_TDYTH_4G_IpNet_Third_SGWSendToPGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &PGWSockaddr, sizeof(PGWSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } 
                            else 
                            {
                                printf("-------------step3: SGW#Sender -> PGW：0x03--------------\n");
                            }
                            break;

                        case 0x06:     
                            printf("-------------step6.SGW#Receiver ->PGW:0x06-------------\n");
         
                            if(NSP_TDYTH_4G_IpNet_Seventh_SGWToENB(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } 
                            else 
                            {
                                printf("-------------step7: SGW#Sender -> ENB：0x07--------------\n");
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
                        case 0x02:
                            printf("-------------step2.SGW#Receiver ->PGW:0x02-------------\n");

                            if(NSP_TDYTH_4G_BearerSetUp_Third_SGWToMME(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &MMESockaddr, sizeof(MMESockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("-------------step3: SGW#Sender -> MME：0x03-------------\n");
                            }
                            break;
                        case 0x0a:
                            printf("-------------step10.SGW#Receiver ->MME:0x0a-------------\n");

                            if(NSP_TDYTH_4G_BearerSetUp_Eleventh_SGWToPGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &PGWSockaddr, sizeof(PGWSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("-------------step11: SGW#Sender -> PGW：0x0b-------------\n");
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
                            printf("-------------step1.SGW#Receiver ->ENB1:0x01-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_First_SGWToPGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &PGWSockaddr, sizeof(PGWSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step1.SGW#Sender ->PGW:0x01-------------\n");
                            }
                            break;
                        case 0x0a:
                            printf("-------------step10.SGW#Receiver ->PGW:0x0a-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Tenth_SGWToENB(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step10.SGW#Sender ->ENB1:0x0a-------------\n");
                            }
                            break;
                        case 0x0b:
                            printf("-------------step11.SGW#Receiver ->ENB1:0x0b-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Eleventh_SGWToPGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &PGWSockaddr, sizeof(PGWSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step11.SGW#Sender ->PGW:0x0b-------------\n");
                            }
                            break;
                        case 0x14:
                            printf("-------------step20.SGW#Receiver ->PGW:0x14-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Twentieth_SGWToENB(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdSGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &ENB1Sockaddr, sizeof(ENB1Sockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step20.SGW#Sender ->ENB1:0x14-------------\n");
                            }
                            break;
                    }
                    break;
                case 0x13:
                    printf("\n----------目前处于TAU流程----------\n\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x07:
                            printf("----------TAU: step7. New SGW#Receiver -> New MME：Create Session Request----------\n");
                            if(NSP_TDYTH_4G_TAU_Eighth_NewSGWToPGW(cRecvBuf, cSendBuf, iSockfdSGW) < 0 ) {
                                printf("----------TAU: step8. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step8. New SGW#Sender -> PGW：Modify Bearer Request----------\n");
                            }
                            break;
                        case 0x09:
                            printf("----------TAU: step9. New SGW#Receiver -> PGW：Modify Bearer Response----------\n");
                            if(NSP_TDYTH_4G_TAU_Tenth_NewSGWToNewMME(cRecvBuf, cSendBuf, iSockfdSGW) < 0 ) {
                                printf("----------TAU: step10. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step10. New SGW#Sender -> New MME：Create Session Response----------\n");
                            }
                            break;
                        case 0x0f:
                            printf("----------TAU: step15. Old SGW#Receiver -> Old MME：Delete Session Request----------\n");
                            if(NSP_TDYTH_4G_TAU_Sixteenth_OldSGWToOldMME(cRecvBuf, cSendBuf, iSockfdSGW) < 0 ) {
                                printf("----------TAU: step16. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step16. Old SGW#Sender -> Old MME：Delete Session Response----------\n");
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
    }

    close(iSockfdSGW);

    return  iRet;
}


int main()
{
    puts("SGW开启监听状态");
    NSP_TDYTH_4G_SGW_ListenAccept_Function();
    return 0;
}
