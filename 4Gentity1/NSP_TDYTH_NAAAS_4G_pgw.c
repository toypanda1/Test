/*
 * 文件功能：4G接入认证 P-GW服务端主函数
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


/***************************************************
 * 功能：    PGW端--IP分配流程第3步，PGW发送至SGW
 * 参数：	 @ 	[pcRecvBuf in]	接收缓冲区
 *           @ 	[pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          <0   失败
 ***************************************************/
int NSP_TDYTH_4G_IPAllot_Third_PGWSendToSGW(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW stIPAllotBodyThirdPGWToSGW;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW_LEN);
    St_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW *stIPAllotBodySecondSGWToPGW = (St_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIPAllotBodySecondSGWToPGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x04;
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x03;

    stIPAllotBodyThirdPGWToSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIPAllotBodyThirdPGWToSGW.aucIdNumber = 0x0;
    memcpy(stIPAllotBodyThirdPGWToSGW.aucDstId, stIPAllotBodySecondSGWToPGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIPAllotBodyThirdPGWToSGW.aucSrcId, stIPAllotBodySecondSGWToPGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIPAllotBodyThirdPGWToSGW.aucDstPort = stIPAllotBodySecondSGWToPGW->aucDstPort;
    stIPAllotBodyThirdPGWToSGW.aucSrcPort = stIPAllotBodySecondSGWToPGW->aucSrcPort;
    stIPAllotBodyThirdPGWToSGW.iDataLen = 0;

    memcpy(stIPAllotBodyThirdPGWToSGW.aucIMSI, "000111000000000", D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);
    stIPAllotBodyThirdPGWToSGW.ucPDNType = 0x11;
    memcpy(stIPAllotBodyThirdPGWToSGW.aucPDNAddress, "192.168.3.156", D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN);
    stIPAllotBodyThirdPGWToSGW.ucPCO = 0x01;

    memcpy(stIPAllotBodyThirdPGWToSGW.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    printf("请求会话创建数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    printf("the send step is IP Allot %x\n",stHeader.ucStep);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIPAllotBodyThirdPGWToSGW, D_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW_LEN); //数据体填入发送缓冲区

    return  0;
}

/*
 * 功能：上网请求第四步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet_Fourth_PGWSendToNet(char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW));

    St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW *stIpBodyThirdSGWToPGW = (St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIpBodyThirdSGWToPGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet stIpBodyFourthPGWToNet;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x04;

    stIpBodyFourthPGWToNet.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIpBodyFourthPGWToNet.aucIdNumber = 0x0;
    memcpy(stIpBodyFourthPGWToNet.aucDstId, stIpBodyThirdSGWToPGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIpBodyFourthPGWToNet.aucSrcId, stIpBodyThirdSGWToPGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIpBodyFourthPGWToNet.aucSrcPort = stIpBodyThirdSGWToPGW->aucSrcPort;
    stIpBodyFourthPGWToNet.aucDstPort = stIpBodyThirdSGWToPGW->aucDstPort;
    stIpBodyFourthPGWToNet.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet);

    memcpy(stIpBodyFourthPGWToNet.aucDstIp, stIpBodyThirdSGWToPGW->aucDstIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyFourthPGWToNet.aucSrcIp, stIpBodyThirdSGWToPGW->aucSrcIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyFourthPGWToNet.aucGUTI, stIpBodyThirdSGWToPGW->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stIpBodyFourthPGWToNet.aucRequest, stIpBodyThirdSGWToPGW->aucRequest, D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN);   

    memcpy(stIpBodyFourthPGWToNet.aucTerminalId, stIpBodyThirdSGWToPGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("上网请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIpBodyFourthPGWToNet, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：上网请求第六步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet_Sixth_PGWSendToSGW(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW));

    St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW *stIpBodyFifthNetToPGW = (St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIpBodyFifthNetToPGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW stIpBodySixthPGWToSGW;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x06;

    stIpBodySixthPGWToSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIpBodySixthPGWToSGW.aucIdNumber = 0x04;
    memcpy(stIpBodySixthPGWToSGW.aucDstId, stIpBodyFifthNetToPGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stIpBodySixthPGWToSGW.aucSrcId, stIpBodyFifthNetToPGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIpBodySixthPGWToSGW.aucSrcPort = stIpBodyFifthNetToPGW->aucSrcPort;
    stIpBodySixthPGWToSGW.aucDstPort = stIpBodyFifthNetToPGW->aucDstPort;
    stIpBodySixthPGWToSGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW);

    stIpBodySixthPGWToSGW.uiS5_TEID_DL = 2000;
    memcpy(stIpBodySixthPGWToSGW.aucDstIp, stIpBodyFifthNetToPGW->aucDstIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySixthPGWToSGW.aucSrcIp, stIpBodyFifthNetToPGW->aucSrcIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySixthPGWToSGW.aucGTPDstIp, D_NSP_TDYTH_NAAAS_SGW1_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySixthPGWToSGW.aucGTPSrcIp, D_NSP_TDYTH_NAAAS_PGW_IP, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodySixthPGWToSGW.aucGUTI, stIpBodyFifthNetToPGW->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stIpBodySixthPGWToSGW.aucResult, stIpBodyFifthNetToPGW->aucResult, D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN);

    memcpy(stIpBodySixthPGWToSGW.aucTerminalId, stIpBodyFifthNetToPGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("上网请求数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIpBodySixthPGWToSGW, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：专有承载建立第二步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Second_PGWToSGW(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW));

    St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW *stBearerSetUpFirstPCRFToPGW = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpFirstPCRFToPGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW stBearerSetUpSecondPGWToSGW;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x02;

    stBearerSetUpSecondPGWToSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpSecondPGWToSGW.aucIdNumber = 0x0;
    memcpy(stBearerSetUpSecondPGWToSGW.aucDstId, stBearerSetUpFirstPCRFToPGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBearerSetUpSecondPGWToSGW.aucSrcId, stBearerSetUpFirstPCRFToPGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpSecondPGWToSGW.aucSrcPort = stBearerSetUpFirstPCRFToPGW->aucSrcPort;
    stBearerSetUpSecondPGWToSGW.aucDstPort = stBearerSetUpFirstPCRFToPGW->aucDstPort;
    stBearerSetUpSecondPGWToSGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW);

    memcpy(stBearerSetUpSecondPGWToSGW.aucIMSI, "000111000000000", D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);//IMSI哪里来？
    stBearerSetUpSecondPGWToSGW.ucPTI='0';//过程事务标识
    memcpy(stBearerSetUpSecondPGWToSGW.aucQoS, "00000000000000000000000000000000000000000000000000", D_NSP_TDYTH_NAAAS_4G_QoS_LEN);
    memcpy(stBearerSetUpSecondPGWToSGW.aucTFT, "00000000000000000000000000000000000000000000000000", D_NSP_TDYTH_NAAAS_4G_TFT_LEN);
    stBearerSetUpSecondPGWToSGW.uiS5_TEID = 2000;
    stBearerSetUpSecondPGWToSGW.ucLBI=0;

    memcpy(stBearerSetUpSecondPGWToSGW.aucTerminalId, stBearerSetUpFirstPCRFToPGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpSecondPGWToSGW, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：专有承载建立第十二步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_Twelfth_PGWToPCRF(char *pcRecvBuf, char *pcSendBuf)
{

    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW));

    St_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW *stBearerSetUpEleventhSGWToPGW = (St_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stBearerSetUpEleventhSGWToPGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpTwelfthPGWToPCRF stBearerSetUpTwelfthPGWToPCRF;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x0c;

    stBearerSetUpTwelfthPGWToPCRF.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpTwelfthPGWToPCRF.aucIdNumber = 0x0;
    memcpy(stBearerSetUpTwelfthPGWToPCRF.aucDstId, stBearerSetUpEleventhSGWToPGW->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBearerSetUpTwelfthPGWToPCRF.aucSrcId, stBearerSetUpEleventhSGWToPGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpTwelfthPGWToPCRF.aucSrcPort = stBearerSetUpEleventhSGWToPGW->aucSrcPort;
    stBearerSetUpTwelfthPGWToPCRF.aucDstPort = stBearerSetUpEleventhSGWToPGW->aucDstPort;
    stBearerSetUpTwelfthPGWToPCRF.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME);

    //没有填东西

    memcpy(stBearerSetUpTwelfthPGWToPCRF.aucTerminalId, stBearerSetUpEleventhSGWToPGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpTwelfthPGWToPCRF, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpTwelfthPGWToPCRF));
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/***********************************************************************IMS***********************************************************************/
int NSP_TDYTH_4G_IMSRegister_First_PGWToPCSCF(char *pcRecvBuf, char *pcSendBuf){
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

    printf("IMS-PGW 第一步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, imsbody,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF_LEN);


    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}

int NSP_TDYTH_4G_IMSRegister_Tenth_PGWToSGW(char *pcRecvBuf, char *pcSendBuf)
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
    stHeader.ucStep = 0x0a;

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

    printf("IMS-PGW第十步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, imsbody,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

int NSP_TDYTH_4G_IMSRegister_Eleventh_PGWToPCSCF(char *pcRecvBuf, char *pcSendBuf){
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

    printf("IMS-PGW 第11步完成.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, imsbody,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN + D_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF_LEN);


    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

int NSP_TDYTH_4G_IMSRegister_Twentieth_PGWToSGW(char *pcRecvBuf, char *pcSendBuf)
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
 * 功能：  TAU流程第9步
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
int NSP_TDYTH_4G_TAU_Ninth_PGWToNewSGW(char *pcRecvBuf, char *pcSendBuf, int iSockfd)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW stTauBodyNinthPGWToNewSGW;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW *stTauBodyEighthNewSGWToPGW = (St_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyEighthNewSGWToPGW->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x09;

    stTauBodyNinthPGWToNewSGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauBodyNinthPGWToNewSGW.aucDstId, stTauBodyEighthNewSGWToPGW->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyNinthPGWToNewSGW.uiDstPort = stTauBodyEighthNewSGWToPGW->uiSrcPort;
    memcpy(stTauBodyNinthPGWToNewSGW.aucSrcId, D_NSP_TDYTH_NAAAS_PGW_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauBodyNinthPGWToNewSGW.uiSrcPort = D_NSP_TDYTH_NAAAS_PGW_PORT;
    stTauBodyNinthPGWToNewSGW.uiDataLen = 0;

    memcpy(stTauBodyNinthPGWToNewSGW.aucTerminalId, stTauBodyEighthNewSGWToPGW->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stTauBodyNinthPGWToNewSGW.aucIMSI, stTauBodyEighthNewSGWToPGW->aucIMSI, D_NSP_TDYTH_NAAAS_4G_IMSI_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauBodyNinthPGWToNewSGW, D_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauBodyNinthPGWToNewSGW.aucDstId, stTauBodyNinthPGWToNewSGW.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return -1;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/***********************************************************************TAU结束***********************************************************************/

/**********************************************************
 * 函数名：	 NSP_TDYTH_4G_PGW_ListenAccept_Function
 * 功能：    PGW端--接受监听函数
 * 参数：	 void
 * 返回值：  0   成功
 *         	<0   失败
 *********************************************************/
int NSP_TDYTH_4G_PGW_ListenAccept_Function()
{
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];


    // PGW
    int iSockfdPGW = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in PGWSockaddr;
    bzero(&PGWSockaddr, sizeof(PGWSockaddr));
    PGWSockaddr.sin_family = AF_INET;
    PGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PGW_PORT);
    PGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PGW_IP);
    int iPGWAddrLen = sizeof(PGWSockaddr);

    // SGW
    struct sockaddr_in SGWSockaddr;
    bzero(&SGWSockaddr, sizeof(SGWSockaddr));
    SGWSockaddr.sin_family = AF_INET;
    SGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_SGW1_PORT);
    SGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_SGW1_IP);
    // NET
    struct sockaddr_in NETSockaddr;
    bzero(&NETSockaddr, sizeof(NETSockaddr));
    NETSockaddr.sin_family = AF_INET;
    NETSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_NET_PORT);
    NETSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_NET_IP);

    // PCRF
    struct sockaddr_in PCRFSockaddr;
    bzero(&PCRFSockaddr, sizeof(PCRFSockaddr));
    PCRFSockaddr.sin_family = AF_INET;
    PCRFSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PCRF_PORT);
    PCRFSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PCRF_IP);

    //PCSCF
    struct sockaddr_in PCSCFSockaddr;
    bzero(&PCSCFSockaddr, sizeof(PCSCFSockaddr));
    PCSCFSockaddr.sin_family = AF_INET;
    PCSCFSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PCSCF_PORT);
    PCSCFSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PCSCF_IP);


    // bind，成功返回0，出错返回-1
    if (bind(iSockfdPGW, (struct sockaddr *) &PGWSockaddr, sizeof(PGWSockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));

        iRet = recvfrom(iSockfdPGW, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_PGW_BUFF_LEN, 0,
                        (struct sockaddr *) &PGWSockaddr, &iPGWAddrLen);

        if (iRet > 0)
        {
            memcpy(cHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *)cHeader;             // 解析从mme发来的包头

            switch (pstHeader->ucReqType)   // 判断请求类别
            {
                case 0x04:          // IP分配流程 0x04
                    printf("-------------目前处于IP分配流程！-------------\n");
                    switch (pstHeader->ucStep)     // 判断步骤码
                    {
                        case 0x02:      // 第 2 步  接收从SGW发送的用户会话创建请求
                            printf("-------------step1. SGW#Receiver <- MME：0x01-------------\n");

                            if(NSP_TDYTH_4G_IPAllot_Third_PGWSendToSGW(cRecvBuf, cSendBuf) < 0) break;

                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_PGW_BUFF_LEN, 0,
                                    (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("请求会话创建响应数据发送完毕,step: 3 完成.\n");
                            }
                            printf("-------------step3: PGW#Sender -> SGW：0x03-------------\n");

                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;

                    }

                    break;
                case 0x10: //IP上网请求
                    printf("-------------目前处于上网流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x03:      // 第 3 步  接收从SGW发送的上网数据
                            printf("-------------step3.PGW#Receiver ->SGW:0x03-------------\n");
         
                            if(NSP_TDYTH_4G_IpNet_Fourth_PGWSendToNet(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &NETSockaddr, sizeof(NETSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } 
                            else 
                            {
                                printf("-------------step4: PGW#Sender -> SGW：0x04-------------\n");
                            }

                            break;

                        case 0x05:      // 第 3 步  接收从SGW发送的上网数据
                            printf("-------------step5.PGW#Receiver ->NET:0x05-------------\n");
         
                            if(NSP_TDYTH_4G_IpNet_Sixth_PGWSendToSGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } 
                            else 
                            {
                                printf("-------------step6: PGW#Sender -> SGW：0x06-------------\n");
                            }

                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case  0x0b:
                    printf("-------------目前处于专有承载建立流程！-------------\n");
                    switch (pstHeader->ucStep)   // 判断步骤
                    {
                        case 0x01:
                            printf("-------------step1.PGW#Receiver ->PCRF:0x01-------------\n");

                            if(NSP_TDYTH_4G_BearerSetUp_Second_PGWToSGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("-------------step2: PGW#Sender -> SGW：0x02-------------\n");
                            }

                            break;
                        case 0x0b:
                            printf("--------------step11.PGW#Receiver ->SGW:0x0b-------------\n");

                            if(NSP_TDYTH_4G_BearerSetUp_Twelfth_PGWToPCRF(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &PCRFSockaddr, sizeof(PCRFSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            }
                            else
                            {
                                printf("-------------step12: PGW#Sender -> PCRF：0x0c-------------\n");
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
                            printf("-------------step1.PGW#Receiver ->SGW:0x01-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_First_PGWToPCSCF(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &PCSCFSockaddr, sizeof(PCSCFSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step1.PGW#Sender ->PCSCF:0x01-------------\n");
                            }
                            break;
                        case 0x0a:
                            printf("--------------step10.PGW#Receiver ->PCSCF:0x0a-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Tenth_PGWToSGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step10.PGW#Sender ->SGW:0x0a-------------\n");
                            }
                            break;
                        case 0x0b:
                            printf("-------------step11.PGW#Receiver ->SGW:0x0b-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Eleventh_PGWToPCSCF(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &PCSCFSockaddr, sizeof(PCSCFSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step11.PGW#Sender ->PCSCF:0x0b-------------\n");
                            }
                            break;
                        case 0x14:
                            printf("-------------step20.PGW#Receiver ->PCSCF:0x14-------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Twentieth_PGWToSGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfdPGW, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &SGWSockaddr, sizeof(SGWSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("-------------step20.PGW#Sender ->PCSCF:0x0b-------------\n");
                            }
                            break;


                            break;
                    }
                    break;
                case 0x13:
                    printf("\n----------目前处于TAU流程----------\n\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x08:
                            printf("----------TAU: step8. PGW#Receiver -> New SGW：Modify Bearer Request----------\n");
                            //此处应有一个根据请求包中的IMSI更改对应UE承载的步骤
                            if(NSP_TDYTH_4G_TAU_Ninth_PGWToNewSGW(cRecvBuf, cSendBuf, iSockfdPGW) < 0 ) {
                                printf("----------TAU: step9. SEND ERROR----------\n");
                                break;
                            }else {
                                printf("----------TAU: step9. PGW#Sender -> New SGW：Modify Bearer Response----------\n");
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

    close(iSockfdPGW);

    return  iRet;
}


int main()
{
    puts("PGW开启监听状态");
    NSP_TDYTH_4G_PGW_ListenAccept_Function();
    return 0;
}

