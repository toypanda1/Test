/*
 * 文件功能：P-CSCF
 * 作者：
 * 创建日期：2020/12/04
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

unsigned static int guiMax_Forwards;
unsigned static char gPCSCFNetworkId[D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN];
unsigned static char gPCSCF_address_name[D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN];
unsigned static char gIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
unsigned static char gIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
unsigned static char gFrom[D_NSP_TDYTH_NAAAS_4GSIP_From_LEN];
unsigned static char gTo[D_NSP_TDYTH_NAAAS_4GSIP_To_LEN];
unsigned static char gCall_ID[D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN];
unsigned static char gCSeq[D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN];
unsigned static char gRES[D_NSP_TDYTH_NAAAS_4G_aucRES_LEN];
unsigned static char gVia[D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN];

/*
 * 功能：IMS注册第三步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IMSRegister_Third_ICSCFToHSS(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
    unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
    memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

    memcpy(sipbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
           D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeadereader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *)sipbody;

    memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +
                D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterSecondPCSCFToICSCF_LEN);
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterSecondPCSCFToICSCF *stImsBodySecondPCSCFToICSCF = (St_NSP_TDYTH_NAAAS_4G_IMSRegisterSecondPCSCFToICSCF * )imsbody;
    //检查时间戳
    if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    //检查sip头的合法性
    if(gSipHeadereader->uiMax_Forwards == 0 ){
        printf("IMS第二步出错,sip非法");
        return -1;
    }
    //todo
    //检查data合法性
    if(stImsBodySecondPCSCFToICSCF->iDataLen <= 0){
        printf("IMS第二步出错,数据包长度非法");
        return -1;
    }
    //保存数据到本地
    guiMax_Forwards = gSipHeadereader->uiMax_Forwards;
    printf("TTL: %d",guiMax_Forwards);
    memcpy(gPCSCFNetworkId, stImsBodySecondPCSCFToICSCF->aucPCSCFNetworkId, D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN);
    memcpy(gPCSCF_address_name, stImsBodySecondPCSCFToICSCF->aucPCSCF_address_name,D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN);
    memcpy(gIMPI, stImsBodySecondPCSCFToICSCF->aucIMPI, D_NSP_TDYTH_NAAAS_4G_IMPI_LEN);
    memcpy(gIMPU, stImsBodySecondPCSCFToICSCF->aucIMPU, D_NSP_TDYTH_NAAAS_4G_IMPU_LEN);

    NSP_TDYTH_4G_printSIP(gSipHeadereader,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
    //构建4G头
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x03;
    printf("ReqType:%d\n", stHeader.ucReqType);
    printf("Step:%d\n",stHeader.ucStep);

    //构建第二层头
    St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
    memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
    stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();
    printf("time:%ld\n",stSecHeader.TimeStamp);

     //构建第三层头
     St_NSP_TDYTH_NAAAS_4G_Diameter_Header stDiaHeader;
     stDiaHeader.ucVersion = 0x01;
     stDiaHeader.ucCommandFlags = 0x80;
     memcpy(stDiaHeader.aucCommandCode, "300",D_NSP_TDYTH_NAAAS_4G_CommandCode_LEN);
     stDiaHeader.uiApplicationID = 0;
     stDiaHeader.uiHopbyHopID = random()%65535;
     stDiaHeader.uiEndtoEndID = 100000000;

    //构建包体
    St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS stImsbodyThirdICSCFToHSS;
    memcpy(stImsbodyThirdICSCFToHSS.aucPCSCFNetworkId, stImsBodySecondPCSCFToICSCF->aucPCSCFNetworkId, D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN);
    memcpy(stImsbodyThirdICSCFToHSS.aucIMPI, stImsBodySecondPCSCFToICSCF->aucIMPI, D_NSP_TDYTH_NAAAS_4G_IMPI_LEN);
    memcpy(stImsbodyThirdICSCFToHSS.aucIMPU, stImsBodySecondPCSCFToICSCF->aucIMPU, D_NSP_TDYTH_NAAAS_4G_IMPU_LEN);
    memcpy(stImsbodyThirdICSCFToHSS.aucEnd, stImsBodySecondPCSCFToICSCF->aucEnd, D_NSP_TDYTH_NAAAS_4G_END_LEN);
    stImsbodyThirdICSCFToHSS.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS_LEN;

     printf("IMS-UE第三步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stDiaHeader, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN,
            &stImsbodyThirdICSCFToHSS,D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/*
 * 功能：IMS注册第五步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
 int NSP_TDYTH_4G_IMSRegister_Fifth_ICSCFToSCSCF(char *pcRecvBuf, char *pcSendBuf)
 {
     unsigned char header[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];
     unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
     unsigned char diabody[D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN];

     memcpy(&header, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);

     pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_HEADER_LEN;
     memcpy(&secbody, pcRecvBuf , D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SecHeader gSecHeader ={0};
     memcpy(&gSecHeader, &secbody,D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);

     pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN;
     memcpy(&diabody, pcRecvBuf ,D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
     St_NSP_TDYTH_NAAAS_4G_Diameter_Header gDiaHeadereader ={0} ;
     memcpy(&gDiaHeadereader, &diabody, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);


     pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN;
     memcpy(&imsbody, pcRecvBuf ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF_LEN);
     St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF stImsBodyFourthHSSToICSCF = {0};
     memcpy(&stImsBodyFourthHSSToICSCF, &imsbody,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF_LEN);


     //检查时间戳
     if(NSP_TDYTH_4G_verification_timestamp(gSecHeader.TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }
     //todo
     //检查data合法性
     if(stImsBodyFourthHSSToICSCF.iDataLen <= 0){
         printf("IMS第一步出错,数据包长度非法");
         return -1;
     }

     //构建4G头
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     memcpy(&stHeader, &header, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x05;

     //构建第二层头
     St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
     memcpy(&stSecHeader, &gSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

     //构建第三层头
     St_NSP_TDYTH_NAAAS_4G_SIPHeader stSipHeader;
     memcpy(stSipHeader.aucFrom, "sip:NSP_TDYTH_NAAAS_4G2@xidian.edu",D_NSP_TDYTH_NAAAS_4GSIP_From_LEN);
     //fixme
     memcpy(stSipHeader.aucTo,"sip:NSP_TDYTH_NAAAS_4G1@xidian.edu",D_NSP_TDYTH_NAAAS_4GSIP_To_LEN);
     memcpy(stSipHeader.aucCall_ID,"123123123123@127.0.0.1",D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN);
     memcpy(stSipHeader.aucCSeq,"12345678",D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN);
     //todo
     memcpy(stSipHeader.aucVia,"SIP/2,0/UDP 127.0.0.1:16664",D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN);
     stSipHeader.uiMax_Forwards = guiMax_Forwards -1 ;

     NSP_TDYTH_4G_printSIP(&stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

     //构建包体
     St_NSP_TDYTH_NAAAS_4G_IMSRegisterFifthICSCFToSCSCF stImsbodyFifthICSCFToSCSCF;
     memcpy(stImsbodyFifthICSCFToSCSCF.aucPCSCFNetworkId, gPCSCFNetworkId, D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN);
     memcpy(stImsbodyFifthICSCFToSCSCF.aucPCSCF_address_name, gPCSCF_address_name, D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN);
     memcpy(stImsbodyFifthICSCFToSCSCF.aucIMPI, gIMPI, D_NSP_TDYTH_NAAAS_4G_IMPI_LEN);
     memcpy(stImsbodyFifthICSCFToSCSCF.aucIMPU, gIMPU, D_NSP_TDYTH_NAAAS_4G_IMPU_LEN);
     memcpy(stImsbodyFifthICSCFToSCSCF.aucEnd, D_NSP_TDYTH_NAAAS_4G_END, D_NSP_TDYTH_NAAAS_4G_END_LEN);
     stImsbodyFifthICSCFToSCSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterFifthICSCFToSCSCF_LEN;

     printf("IMS-UE第5步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN,
            &stImsbodyFifthICSCFToSCSCF,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFifthICSCFToSCSCF_LEN);

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }
/*
 * 功能：IMS注册第九步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
 int NSP_TDYTH_4G_IMSRegister_Ninth_ICSCFToPCSCF(char *pcRecvBuf, char *pcSendBuf)
 {
     unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
     unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
     memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

     memcpy(sipbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeadereader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *)sipbody;

     memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +
                     D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterEighthSCSCFToICSCF_LEN);
     St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighthSCSCFToICSCF *stImsBodyEighthSCSCFToICSCF = (St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighthSCSCFToICSCF * )imsbody;
     //检查时间戳
     if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }
     //检查sip头的合法性
     if(gSipHeadereader->uiMax_Forwards == 0 ){
         printf("IMS第八步出错,sip非法");
         return -1;
     }
     //todo
     //检查data合法性
     if(stImsBodyEighthSCSCFToICSCF->iDataLen <= 0){
         printf("IMS第八步出错,数据包长度非法");
         return -1;
     }
     NSP_TDYTH_4G_printSIP(gSipHeadereader,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     //构建4G头
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x09;

     //构建第二层头
     St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
     memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

     //构建第三层头
     St_NSP_TDYTH_NAAAS_4G_SIPHeader stSipHeader;
     memcpy(stSipHeader.aucFrom, "sip:NSP_TDYTH_NAAAS_4G2@xidian.edu",D_NSP_TDYTH_NAAAS_4GSIP_From_LEN);
     memcpy(stSipHeader.aucTo,"sip:NSP_TDYTH_NAAAS_4G1@xidian.edu",D_NSP_TDYTH_NAAAS_4GSIP_To_LEN);
     memcpy(stSipHeader.aucCall_ID,"123123123123@127.0.0.1",D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN);
     memcpy(stSipHeader.aucCSeq,"12345678",D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN);
     memcpy(stSipHeader.aucVia,"SIP/2,0/UDP 127.0.0.1:16664",D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN);
     stSipHeader.uiMax_Forwards = gSipHeadereader->uiMax_Forwards-1;
     printf("TTL: %d", stSipHeader.uiMax_Forwards);

     St_NSP_TDYTH_NAAAS_4G_IMSRegisterNinthICSCFToPCSCF stIMSbodyNinthICSCFToPCSCF;
     memcpy(stIMSbodyNinthICSCFToPCSCF.aucIMPI, stImsBodyEighthSCSCFToICSCF->aucIMPI, D_NSP_TDYTH_NAAAS_4G_IMPI_LEN);
     memcpy(stIMSbodyNinthICSCFToPCSCF.aucCK,stImsBodyEighthSCSCFToICSCF->aucCK, D_NSP_TDYTH_NAAAS_4G_CK_LEN);
     memcpy(stIMSbodyNinthICSCFToPCSCF.aucIK,stImsBodyEighthSCSCFToICSCF->aucIK, D_NSP_TDYTH_NAAAS_4G_IK_LEN);
     memcpy(stIMSbodyNinthICSCFToPCSCF.aucRAND,stImsBodyEighthSCSCFToICSCF->aucRAND, D_NSP_TDYTH_NAAAS_4G_RAND_LEN);
     memcpy(stIMSbodyNinthICSCFToPCSCF.aucAUTN,stImsBodyEighthSCSCFToICSCF->aucAUTN, D_NSP_TDYTH_NAAAS_4G_AUTN_LEN);
     memcpy(stIMSbodyNinthICSCFToPCSCF.aucEnd, stImsBodyEighthSCSCFToICSCF->aucEnd, D_NSP_TDYTH_NAAAS_4G_END_LEN);
     stIMSbodyNinthICSCFToPCSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterNinthICSCFToPCSCF_LEN;

     printf("IMS-UE第九步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN,
            &stIMSbodyNinthICSCFToPCSCF ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterNinthICSCFToPCSCF_LEN);

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }
/*
 * 功能：IMS注册第十三步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
 int NSP_TDYTH_4G_IMSRegister_Thirteenth_ICSCFToHSS(char *pcRecvBuf, char *pcSendBuf)
 {
     unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
     unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
     memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

     memcpy(sipbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeadereader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *)sipbody;

     NSP_TDYTH_4G_printSIP(gSipHeadereader,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

     memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +
                     D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterTwelfthPCSCFToICSCF_LEN);
     St_NSP_TDYTH_NAAAS_4G_IMSRegisterTwelfthPCSCFToICSCF *stImsBodyTwelfthPCSCFToICSCF = (St_NSP_TDYTH_NAAAS_4G_IMSRegisterTwelfthPCSCFToICSCF * )imsbody;

     memcpy(&gFrom, gSipHeadereader->aucFrom, D_NSP_TDYTH_NAAAS_4GSIP_From_LEN);
     memcpy(&gTo, gSipHeadereader->aucTo,D_NSP_TDYTH_NAAAS_4GSIP_To_LEN);
     memcpy(&gCall_ID, gSipHeadereader->aucCall_ID, D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN);
     memcpy(&gCSeq, gSipHeadereader->aucCSeq, D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN);
     memcpy(&gVia, gSipHeadereader->aucVia, D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN);
     guiMax_Forwards = gSipHeadereader->uiMax_Forwards;

     //检查时间戳
     if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }
     //检查sip头的合法性
     if(gSipHeadereader->uiMax_Forwards == 0 ){
         printf("IMS第十二步出错,sip非法");
         return -1;
     }
     //todo
     //检查data合法性
     if(stImsBodyTwelfthPCSCFToICSCF->iDataLen <= 0){
         printf("IMS第十二步出错,数据包长度非法");
         return -1;
     }
     NSP_TDYTH_4G_printSIP(gSipHeadereader,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);

     //构建4G头
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x0d;

     //构建第二层头
     St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
     memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

     //构建第三层头
     St_NSP_TDYTH_NAAAS_4G_Diameter_Header stDiaHeader;
     stDiaHeader.ucVersion = 0x01;
     stDiaHeader.ucCommandFlags = 0x80;
     memcpy(stDiaHeader.aucCommandCode, "300",D_NSP_TDYTH_NAAAS_4G_CommandCode_LEN);
     stDiaHeader.uiApplicationID = 0;
     stDiaHeader.uiHopbyHopID = random()%65535;
     stDiaHeader.uiEndtoEndID = 100000000;

     //构建包体
     St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS stImsbodyThirteenthICSCFToHSS;
     memcpy(stImsbodyThirteenthICSCFToHSS.aucPCSCFNetworkId, stImsBodyTwelfthPCSCFToICSCF->aucPCSCFNetworkId, D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN);
     memcpy(stImsbodyThirteenthICSCFToHSS.aucIMPI, stImsBodyTwelfthPCSCFToICSCF->aucIMPI, D_NSP_TDYTH_NAAAS_4G_IMPI_LEN);
     memcpy(stImsbodyThirteenthICSCFToHSS.aucIMPU, stImsBodyTwelfthPCSCFToICSCF->aucIMPU, D_NSP_TDYTH_NAAAS_4G_IMPU_LEN);
     memcpy(stImsbodyThirteenthICSCFToHSS.aucEnd, stImsBodyTwelfthPCSCFToICSCF->aucEnd, D_NSP_TDYTH_NAAAS_4G_END_LEN);
     stImsbodyThirteenthICSCFToHSS.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS_LEN;
     memcpy(gRES, stImsBodyTwelfthPCSCFToICSCF->aucRes, D_NSP_TDYTH_NAAAS_4G_aucRES_LEN);//保存RES为15步所用

     printf("IMS-UE第十三步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stDiaHeader, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN,
            &stImsbodyThirteenthICSCFToHSS,D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS_LEN);

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }
/*
 * 功能：IMS注册第五步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
 int NSP_TDYTH_4G_IMSRegister_Fifteenth_ICSCFToSCSCF(char *pcRecvBuf, char *pcSendBuf)
 {
     unsigned char header[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];
     unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
     unsigned char diabody[D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN];

     memcpy(&header, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);

     pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_HEADER_LEN;
     memcpy(&secbody, pcRecvBuf , D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SecHeader gSecHeader ={0};
     memcpy(&gSecHeader, &secbody,D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);

     pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN;
     memcpy(&diabody, pcRecvBuf ,D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);
     St_NSP_TDYTH_NAAAS_4G_Diameter_Header gDiaHeadereader ={0} ;
     memcpy(&gDiaHeadereader, &diabody, D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN);


     pcRecvBuf += D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN;
     memcpy(&imsbody, pcRecvBuf ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF_LEN);
     St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF stImsBodyFourteenthHSSToICSCF = {0};
     memcpy(&stImsBodyFourteenthHSSToICSCF, &imsbody,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF_LEN);//检查时间戳

     //构建4G头
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     memcpy(&stHeader, &header, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x0f;

     //构建第二层头
     St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
     memcpy(&stSecHeader, &gSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

     //构建第三层头
     St_NSP_TDYTH_NAAAS_4G_SIPHeader stSipHeader;
     memcpy(stSipHeader.aucFrom, gFrom,D_NSP_TDYTH_NAAAS_4GSIP_From_LEN);
     memcpy(stSipHeader.aucTo,gTo,D_NSP_TDYTH_NAAAS_4GSIP_To_LEN);
     memcpy(stSipHeader.aucCall_ID,gCall_ID,D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN);
     memcpy(stSipHeader.aucCSeq,gCSeq,D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN);
     //todo
     memcpy(stSipHeader.aucVia,"SIP/2,0/UDP 127.0.0.1:16664",D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN);//要改变的
     stSipHeader.uiMax_Forwards = guiMax_Forwards-1;

     NSP_TDYTH_4G_printSIP(&stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);


     //构建包体
     St_NSP_TDYTH_NAAAS_4G_IMSRegisterFifteenthICSCFToSCSCF  stImsbodyFifteenthICSCFToSCSCF;
     memcpy(stImsbodyFifteenthICSCFToSCSCF.aucPCSCFNetworkId, gPCSCFNetworkId, D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN);
     memcpy(stImsbodyFifteenthICSCFToSCSCF.aucPCSCF_address_name, gPCSCF_address_name, D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN);
     memcpy(stImsbodyFifteenthICSCFToSCSCF.aucIMPI, gIMPI, D_NSP_TDYTH_NAAAS_4G_IMPI_LEN);
     memcpy(stImsbodyFifteenthICSCFToSCSCF.aucIMPU, gIMPU, D_NSP_TDYTH_NAAAS_4G_IMPU_LEN);
     memcpy(stImsbodyFifteenthICSCFToSCSCF.aucEnd, D_NSP_TDYTH_NAAAS_4G_END, D_NSP_TDYTH_NAAAS_4G_END_LEN);
     stImsbodyFifteenthICSCFToSCSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterFifteenthICSCFToSCSCF_LEN;
     memcpy(stImsbodyFifteenthICSCFToSCSCF.aucRes,gRES,D_NSP_TDYTH_NAAAS_4G_aucRES_LEN);

     printf("IMS-UE第15步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN,
            &stImsbodyFifteenthICSCFToSCSCF,D_NSP_TDYTH_NAAAS_4G_IMSRegisterFifthICSCFToSCSCF_LEN);

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }

/*
* 功能：IMS注册第十九步
* 参数：   @pcRecvBuf [in] 收到的数据
*         @pcSendBuf [out] 发送的数据
* 返回值：    0   成功
*/

 int NSP_TDYTH_4G_IMSRegister_Nineteenth_ICSCFToPCSCF(char *pcRecvBuf, char *pcSendBuf)
 {
     unsigned char imsbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
     unsigned char secbody[D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN];
     unsigned char sipbody[D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN];
     memcpy(secbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SecHeader *gSecHeader = (St_NSP_TDYTH_NAAAS_4G_SecHeader *)secbody;

     //todo
     memcpy(sipbody, pcRecvBuf +D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN,
            D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     St_NSP_TDYTH_NAAAS_4G_SIPHeader *gSipHeadereader = (St_NSP_TDYTH_NAAAS_4G_SIPHeader *)sipbody;

     memcpy(imsbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN +D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN +
                     D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterEighteenthSCSCFToICSCF_LEN);
     St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighteenthSCSCFToICSCF *stImsBodyEighteenthSCSCFToICSCF = (St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighteenthSCSCFToICSCF * )imsbody;
     //检查时间戳
     if(NSP_TDYTH_4G_verification_timestamp(gSecHeader->TimeStamp) < 0)
     {
         printf("time error!\n");
         return -1;
     }
     //检查sip头的合法性
     if(gSipHeadereader->uiMax_Forwards == 0 ){
         printf("IMS第十八步出错,sip非法");
         return -1;
     }
     //todo
     //检查data合法性
     if(stImsBodyEighteenthSCSCFToICSCF->iDataLen <= 0){
         printf("IMS第十八步出错,数据包长度非法");
         return -1;
     }
     NSP_TDYTH_4G_printSIP(gSipHeadereader,D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     //构建4G头
     St_NSP_TDYTH_NAAAS_4G_Header stHeader;
     memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     stHeader.ucStep = 0x13;//19

     //构建第二层头
     St_NSP_TDYTH_NAAAS_4G_SecHeader stSecHeader;
     memcpy(&stSecHeader, pcRecvBuf+D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     stSecHeader.TimeStamp = NSP_TDYTH_4G_get_time_now();

     //构建第三层头
     St_NSP_TDYTH_NAAAS_4G_SIPHeader stSipHeader;
     memcpy(stSipHeader.aucFrom, gSipHeadereader->aucFrom,D_NSP_TDYTH_NAAAS_4GSIP_From_LEN);
     memcpy(stSipHeader.aucTo,gSipHeadereader->aucTo,D_NSP_TDYTH_NAAAS_4GSIP_To_LEN);
     memcpy(stSipHeader.aucCall_ID,gSipHeadereader->aucCall_ID,D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN);
     memcpy(stSipHeader.aucCSeq,gSipHeadereader->aucCSeq,D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN);
     //todo
     memcpy(stSipHeader.aucVia,"SIP/2,0/UDP 127.0.0.1:16664",D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN);//是需要改变的
     stSipHeader.uiMax_Forwards = gSipHeadereader->uiMax_Forwards - 1;

     St_NSP_TDYTH_NAAAS_4G_IMSRegisterNineteenthICSCFToPCSCF stIMSbodyNineteenthICSCFToPCSCF;
     //todo
     //contact信息的具体内容
     memcpy(stIMSbodyNineteenthICSCFToPCSCF.aucContact, stImsBodyEighteenthSCSCFToICSCF->aucContact, D_NSP_TDYTH_NAAAS_4G_Contact_LEN);

     memcpy(stIMSbodyNineteenthICSCFToPCSCF.aucEnd, stImsBodyEighteenthSCSCFToICSCF->aucEnd, D_NSP_TDYTH_NAAAS_4G_END_LEN);
     stIMSbodyNineteenthICSCFToPCSCF.iDataLen = D_NSP_TDYTH_NAAAS_4G_IMSRegisterNineteenthICSCFToPCSCF_LEN;

     printf("IMS-UE第19步完成.\n");
     memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
     memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stSecHeader, D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN, &stSipHeader, D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN);
     memcpy(pcSendBuf+ D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN + D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN,
            &stIMSbodyNineteenthICSCFToPCSCF ,D_NSP_TDYTH_NAAAS_4G_IMSRegisterNineteenthICSCFToPCSCF_LEN);

     return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
 }


/**********************************************************
 * 函数名：	 NSP_TDYTH_4G_ICSCF_ListenAccept_Function
 * 功能：    ICSCF--接受监听函数
 * 参数：	 void
 * 返回值：  0   成功
 *          <0   失败
 *********************************************************/
int NSP_TDYTH_4G_ICSCF_ListenAccept_Function()
{
    int iRet;
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];

    //PCSCF
    struct sockaddr_in PCSCFSockaddr;
    bzero(&PCSCFSockaddr, sizeof(PCSCFSockaddr));
    PCSCFSockaddr.sin_family = AF_INET;
    PCSCFSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PCSCF_PORT);
    PCSCFSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PCSCF_IP);

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

    //ICSCF
    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in ICSCFSockaddr;
    bzero(&ICSCFSockaddr, sizeof(ICSCFSockaddr));
    ICSCFSockaddr.sin_family = AF_INET;
    ICSCFSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ICSCF_PORT);
    ICSCFSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ICSCF_IP);
    int iAddrLen = sizeof(ICSCFSockaddr);



    // bind，成功返回0，出错返回-1
    if (bind(iSockfd, (struct sockaddr *) &ICSCFSockaddr, sizeof(ICSCFSockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));

        iRet = recvfrom(iSockfd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, 0,
                        (struct sockaddr *) &ICSCFSockaddr, &iAddrLen);

        if (iRet > 0)
        {
            memcpy(cHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *)cHeader;             // 解析包头

            switch (pstHeader->ucReqType)   // 判断请求类别
            {
                case 0x0c:          
                    printf("----------------------目前处于IMS注册流程！---------------------\n");
                    switch (pstHeader->ucStep)     // 判断步骤码
                    {
                        case 0x02:      
                            printf("------------------------step2.ICSCF#Receiver <- PCSCF：0x02------------------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Third_ICSCFToHSS(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &HSSSockaddr, sizeof(HSSSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("------------------------step3.ICSCF#Sender ->HSS:0x03------------------------\n");
                            }
                            break;
                        case 0x04:
                            printf("------------------------step4.ICSCF#Receiver <- HSS：0x04------------------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Fifth_ICSCFToSCSCF(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &SCSCFSockaddr, sizeof(SCSCFSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("------------------------step5.ICSCF#Sender ->SCSCF:0x05------------------------\n");
                            }
                            break;
                        case 0x08:
                            printf("------------------------step8.ICSCF#Receiver <- SCSCF：0x08------------------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Ninth_ICSCFToPCSCF(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &PCSCFSockaddr, sizeof(PCSCFSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("------------------------step9.ICSCF#Sender ->PCSCF:0x09------------------------\n");
                            }
                            break;
                        case 0x0c:
                            printf("------------------------step12.ICSCF#Receiver <- PCSCF：0x0c------------------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Thirteenth_ICSCFToHSS(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &HSSSockaddr, sizeof(HSSSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("------------------------step13.ICSCF#Sender ->HSS:0x0d------------------------\n");
                            }
                            break;

                        case 0x0e:
                            printf("------------------------step14.ICSCF#Receiver <- HSS：0x0e------------------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Fifteenth_ICSCFToSCSCF(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &SCSCFSockaddr, sizeof(SCSCFSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("------------------------step15.ICSCF#Sender ->SCSCF:0x0f------------------------\n");
                            }
                            break;

                        case 0x12:
                            printf("------------------------step18.ICSCF#Receiver <- SCSCF：0x12------------------------\n");
                            if(NSP_TDYTH_4G_IMSRegister_Nineteenth_ICSCFToPCSCF(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN,
                                               0, (struct sockaddr *) &PCSCFSockaddr, sizeof(PCSCFSockaddr))) < 0) {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } else {
                                printf("------------------------step19.ICSCF#Sender ->PCSCF:0x13------------------------\n");
                            }
                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
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

    close(iSockfd);

    return  iRet;
}

int main()
{
    printf("ICSCF开启监听状态\n");
    NSP_TDYTH_4G_ICSCF_ListenAccept_Function();
    return 0;
}