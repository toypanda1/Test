/*
 * 文件功能：4G接入认证协议结构体定义
 * 作者：董珂
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#ifndef NSP_TDYTH_NAAAS_4G_PROTO_H
#define NSP_TDYTH_NAAAS_4G_PROTO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "NSP_TDYTH_NAAAS_4G_common.h"
/**************************AUTH*******************************/
#define D_NSP_TDYTH_NAAAS_4G_HEADER_LEN                     sizeof(St_NSP_TDYTH_NAAAS_4G_Header)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB)
#define D_NSP_TDYTH_NAAAS_4G_AuBodySecondENBToUE_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodySecondENBToUE)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyFourthENBToMME_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyFourthENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS)
#define D_NSP_TDYTH_NAAAS_4G_AuBodySixthHSSToAUC_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodySixthHSSToAUC)
#define D_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyTenthENBToUE_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyTenthENBToUE)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyTwelfthENBToMME_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyTwelfthENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB_LEN   sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB)
#define D_NSP_TDYTH_NAAAS_4G_AuBodyFourteenthENBToUE_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_AuBodyFourteenthENBToUE)
#define D_NSP_TDYTH_NAAAS_4G_AuResult_LEN                   10
//#define D_NSP_TDYTH_NAAAS_4G_TimeStamp_LEN                  10
#define D_NSP_TDYTH_NAAAS_4G_ID_LEN                         11

/**************************UserInfo*******************************/
#define D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_LEN           sizeof(St_NSP_TDYTH_NAAAS_4G_UserInfo_Request)
#define D_NSP_TDYTH_NAAAS_4G_UserInfo_Response_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_UserInfo_Response)
#define D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS_LEN           sizeof(St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS)
#define D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME)
#define D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE_LEN            sizeof(St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE)
#define D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFourthUEToMME_LEN           sizeof(St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFourthUEToMME)
#define D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_IPArray_LEN            500
#define D_NSP_TDYTH_NAAAS_4G_UserInfo_Request_PortArray_LEN          50

/**************************Register*******************************/

//#define D_NSP_TDYTH_NAAAS_4G_ID_LEN                         15
#define D_NSP_TDYTH_NAAAS_4G_USERTD_LEN                     40
#define D_NSP_TDYTH_NAAAS_4G_IdNum_LEN                      18
#define D_NSP_TDYTH_NAAAS_4G_Addr_LEN                       30
#define D_NSP_TDYTH_NAAAS_4G_Categ_LEN                      10
#define D_NSP_TDYTH_NAAAS_4G_Country_LEN                    20
#define D_NSP_TDYTH_NAAAS_4G_Province_LEN                   20
#define D_NSP_TDYTH_NAAAS_4G_City_LEN                       20
#define D_NSP_TDYTH_NAAAS_4G_SerPWD_LEN                     6
#define D_NSP_TDYTH_NAAAS_4G_AcTIME_LEN                     30

#define D_NSP_TDYTH_NAAAS_4G_ReBodyFirstUEToHSS_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS)
#define D_NSP_TDYTH_NAAAS_4G_ReBodySecondHSSToUE_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_ReBodySecondHSSToUE)

/***************************Attach*********************************/
#define D_NSP_TDYTH_NAAAS_4G_PDNTYPE_LEN   1
#define D_NSP_TDYTH_NAAAS_4G_PCO_LEN       1
#define D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN     2

#define D_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB_LEN   sizeof(St_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB)
#define D_NSP_TDYTH_NAAAS_4G_AttBodySecondeNBToUE_LEN  sizeof(St_NSP_TDYTH_NAAAS_4G_AttBodySecondeNBToUE)
#define D_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB_LEN   sizeof(St_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB)
#define D_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME_LEN sizeof(St_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME)

/***************************IPAllot*********************************/
#define D_NSP_TDYTH_NAAAS_4G_UE_IP_LEN                  15          // 15 Byte
#define D_NSP_TDYTH_NAAAS_4G_DNS_IP_LEN                 15
#define D_NSP_TDYTH_NAAAS_4G_SGW_IP_LEN                 15
#define D_NSP_TDYTH_NAAAS_4G_PGW_IP_LEN                 15
#define D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN         15
#define D_NSP_TDYTH_NAAAS_4G_PDN_TYPE_LEN               1
#define D_NSP_TDYTH_NAAAS_4G_PCO_LEN                    1
#define D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN                  2
#define D_NSP_TDYTH_NAAAS_4G_KENB_LEN                   32          // 256 bits
#define D_NSP_TDYTH_NAAAS_4G_NH_LEN                     32          // 256 bits
#define D_NSP_TDYTH_NAAAS_4G_NCC_LEN                    1           // 3 bits

/***************************守护进程*********************************/
#define D_NSP_TDYTH_NAAAS_4G_DaemonControl_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_DaemonControl)
#define D_NSP_TDYTH_NAAAS_4G_MeasureInfo_LEN            sizeof(St_NSP_TDYTH_NAAAS_4G_MeasureInfo)

/***************************IPAllot*********************************/
#define D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW)
#define D_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW)
#define D_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW)
#define D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME)
#define D_NSP_TDYTH_NAAAS_4G_IPAllotBodyFifthMMEToENB_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFifthMMEToENB)
#define D_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE_LEN           sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE)
#define D_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB)
#define D_NSP_TDYTH_NAAAS_4G_IPAllotBodyEighthENBToMME_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_IPAllotBodyEighthENBToMME)


/***************************X2切换*********************************/

#define D_NSP_TDYTH_NAAAS_4G_HandOverBodyFirstUEToSENB_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodyFirstUEToSENB)
#define D_NSP_TDYTH_NAAAS_4G_HandOverBodySecondSENBToTENB_LEN   sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodySecondSENBToTENB)
#define D_NSP_TDYTH_NAAAS_4G_HandOverBodyThirdTENBToSENB_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodyThirdTENBToSENB)
#define D_NSP_TDYTH_NAAAS_4G_HandOverBodyFourthSENBToUE_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodyFourthSENBToUE)
#define D_NSP_TDYTH_NAAAS_4G_HandOverBodyFifthSENBToTENB_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodyFifthSENBToTENB)
#define D_NSP_TDYTH_NAAAS_4G_HandOverBodySixthUEToTENB_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodySixthUEToTENB)
#define D_NSP_TDYTH_NAAAS_4G_HandOverBodySeventhTENBToMME_LEN   sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodySeventhTENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_HandOverBodyEighthMMEToTENB_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodyEighthMMEToTENB)
#define D_NSP_TDYTH_NAAAS_4G_HandOverBodyNinthTENBToSENB_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_N2_BodyNinthTENBToSENB)

/***************************S1切换*********************************/
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFirstUEToSENB_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFirstUEToSENB)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySeventhSENBToMME_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySeventhSENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEighthMMEToTENB_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEighthMMEToTENB)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEleventhMMEToSENB_LEN   sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEleventhMMEToSENB)
#define D_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTwelfthSENBToMME_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTwelfthSENBToMME)

/**************************站内切换*********************************/
#define D_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB)
#define D_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE)
#define D_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyThirdUEToENB_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyThirdUEToENB)

/**************************MME中断*********************************/
#define D_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB)
#define D_NSP_TDYTH_NAAAS_4G_LinkageInterruptSecondENBToUE_LEN        D_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB_LEN
/**************************MME恢复链接*********************************/
#define D_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB)
#define D_NSP_TDYTH_NAAAS_4G_LinkageRecoverSecondENBToUE_LEN        D_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB_LEN
/**************************测量报告*********************************/
#define St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB_LEN  sizeof(St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB)

/**************************IP上网*********************************/
#define St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB_LEN                 sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB)
#define St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW_LEN               sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW)
#define St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW_LEN                sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW)
#define St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet_LEN               sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet)
#define St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW_LEN                sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW)
#define St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW_LEN                sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW)
#define St_NSP_TDYTH_NAAAS_4G_IpBodySeventhSGWToENB_LEN              sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE)
#define St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE_LEN                sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE)

/**************************专有承载建立*******************************/

#define D_NSP_TDYTH_NAAAS_4G_QoS_LEN                        50
#define D_NSP_TDYTH_NAAAS_4G_TFT_LEN                        50

#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB           sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW)
#define D_NSP_TDYTH_NAAAS_4G_BearerSetUpTwelfthPGWToPCRF_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpTwelfthPGWToPCRF)


/**************************IMS域*******************************/

#define D_NSP_TDYTH_NAAAS_4G_IMPI_LEN                       60
#define D_NSP_TDYTH_NAAAS_4G_IMPU_LEN                       60
#define D_NSP_TDYTH_NAAAS_4G_HomeNetworkDomainName_LEN      60
#define D_NSP_TDYTH_NAAAS_4G_UEIp_LEN                       16
#define D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN             60
#define D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN         60
#define D_NSP_TDYTH_NAAAS_4G_SCSCFName_LEN                  60
#define D_NSP_TDYTH_NAAAS_4G_Contact_LEN                    60
#define D_NSP_TDYTH_NAAAS_4G_SecHeader_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_SecHeader)
#define D_NSP_TDYTH_NAAAS_4G_SIPHeader_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_SIPHeader)
#define D_NSP_TDYTH_NAAAS_4G_Diameter_Header_LEN    sizeof(St_NSP_TDYTH_NAAAS_4G_Diameter_Header)

#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF_LEN               sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterSecondPCSCFToICSCF_LEN           sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterSecondPCSCFToICSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS_LEN              sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF_LEN             sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterFifthICSCFToSCSCF_LEN            sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterFifthICSCFToSCSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterSixthSCSCFToHSS_LEN              sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixthSCSCFToHSS)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventhHSSToSCSCF_LEN            sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventhHSSToSCSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterEighthSCSCFToICSCF_LEN           sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighthSCSCFToICSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterNinthICSCFToPCSCF_LEN            sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterNinthICSCFToPCSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE_LEN               sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF_LEN            sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterTwelfthPCSCFToICSCF_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterTwelfthPCSCFToICSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterFifteenthICSCFToSCSCF_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterFifteenthICSCFToSCSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterSixteenthSCSCFToHSS_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixteenthSCSCFToHSS)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventeenthHSSToSCSCF_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventeenthHSSToSCSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterEighteenthSCSCFToICSCF_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighteenthSCSCFToICSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterNineteenthICSCFToPCSCF_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterNineteenthICSCFToPCSCF)
#define D_NSP_TDYTH_NAAAS_4G_IMSRegisterTwentiethPCSCFToUE_LEN               sizeof(St_NSP_TDYTH_NAAAS_4G_IMSRegisterTwentiethPCSCFToUE)


/**************************广播包*******************************/
#define D_NSP_TDYTH_NAAAS_4G_BroadcastBody_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_BroadcastBody)

/**************************TAU*******************************/
#define D_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB_LEN                sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB)
#define D_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME_LEN              sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyFifthNewMMEToOldMME_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyFifthNewMMEToOldMME)
#define D_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW_LEN           sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW_LEN            sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS_LEN         sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME_LEN          sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW)
#define D_NSP_TDYTH_NAAAS_4G_TauBodySixteenthOldSGWToOldMME_LEN     sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodySixteenthOldSGWToOldMME)
#define D_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE_LEN       sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE)
#define D_NSP_TDYTH_NAAAS_4G_TauBodyEighteenthUEToNewMME_LEN        sizeof(St_NSP_TDYTH_NAAAS_4G_TauBodyEighteenthUEToNewMME)


/**************************攻击相关*******************************/
#define D_NSP_TDYTH_NAAAS_4G_AucRefuseBody_LEN      sizeof(St_NSP_TDYTH_NAAAS_4G_AucRefuseBody)
#define D_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE_LEN   sizeof(St_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE)

typedef struct{
    unsigned int NCC;
    unsigned char NH[D_NSP_TDYTH_NAAAS_4G_NH_LEN];
} KeyPair;


/***********************************************************************
* 结构体名： St_NSP_TDYTH_NAAAS_4G_Header
* 结构体用途：				      协议数据域首部
* @param  ucVersion               协议版本号
* @param  usModuleCode            模块码
* @param  ucReqType               请求类型
* @param  ucSubType               子类型
* @param  ucUserCategory		  用户类别
* @param  ucStep                  步骤码
************************************************************************/
typedef struct
{
    unsigned char ucVersion;		//0x01
    unsigned short usModuleCode; 	//0xA105
    unsigned char ucReqType;		//请求类型 0x12
    unsigned char ucSubType;		//子类型 0x04标识4G网络
    unsigned char ucUserCategory;   //用户类型 UE 0x01 MME 0x02 HSS 0x03
    unsigned char ucStep;			//步骤 1-7
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_Header;

//注：目前ucReqType代表不同的类型，0x00注册、0x01认证、0x02请求用户信息、0x03UE附着、0x04IP分配、0x05X2切换、0x06 S1切换、0x07 站内切换、0x08  MME中断
//0x10上网请求, 0x11ENB广播包, 0x12错误信息，0x13TAU流程
//当ucReqType为0x12时，ucStep表示具体错误原因. 0x00 ENB无法完成附着. 0x01 拒绝TAU请求......

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB
* 结构体用途：				4G接入认证第1步MME到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  ucReqAuth			用户身份请求标识
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucDstPort;
    unsigned int  aucSrcPort;
    unsigned int  iDataLen;


    unsigned char ucReqAuth;    //暂时定义0x01来定义用户身份请求标识
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyFirstMMEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_AuBodySecondENBToUE
* 结构体用途：                4G接入认证第2步ENB到UE数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号\
* @param  iDataLen          数据长度
* @param  ucReqAuth         用户身份请求标识
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucDstPort;
    unsigned int  aucSrcPort;
    unsigned int iDataLen;

	
    unsigned char ucReqAuth;    //暂时定义0x01来定义用户身份请求标识
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodySecondENBToUE;
/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB
* 结构体用途：				4G接入认证第3步UE到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucIMSI	 		国际移动订阅用户身份
* @param  pucGUTI           临时标识符
* @param  pucHSSID 		    HSS ID
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int iDataLen;

	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucHSSID;
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyThirdUEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_AuBodyFourthENBToMME
* 结构体用途：                4G接入认证第4步ENB到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucIMSI           国际移动订阅用户身份
* @param  pucGUTI           临时标识符
* @param  pucHSSID          HSS ID
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucHSSID;
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyFourthENBToMME;
/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS
* 结构体用途：				4G接入认证第5步MME到HSS数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucIMSI	 		国际移动订阅用户身份
* @param  pucGUTI           临时标识符
* @param  pucSNID 		    SN ID归属域标识
* @param  ucNetworkType		网络类型
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int iDataLen;

	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucSNID[D_NSP_TDYTH_NAAAS_4G_SNID_LEN];
    unsigned char ucNetworkType;
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyFifthMMEToHSS;

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodySixthHSSToAUC
* 结构体用途：				4G接入认证第6步HSS到AUC数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucIMSI	 		国际移动订阅用户身份
* @param  pucGUTI           临时标识符
* @param  pucSNID 		    SN ID归属域标识
* @param  ucNetworkType		网络类型
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucSNID[D_NSP_TDYTH_NAAAS_4G_SNID_LEN];
	
    unsigned char ucNetworkType;
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodySixthHSSToAUC;

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS
* 结构体用途：				4G接入认证第7步AUC到HSS数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucSQN	 		SQN
* @param  aucCK			    加密密钥CA
* @param  aucIK			    完整性密钥IA
* @param  aucAK			    认证密钥AK
* @param  aucRAND	 		随机数
* @param  aucAUTN			认证数据
* @param  aucXRES 		    期望响应
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucSQN[D_NSP_TDYTH_NAAAS_4G_SQN_LEN];
    unsigned char aucCK[D_NSP_TDYTH_NAAAS_4G_CK_LEN];
    unsigned char aucIK[D_NSP_TDYTH_NAAAS_4G_IK_LEN];
    unsigned char aucAK[D_NSP_TDYTH_NAAAS_4G_AK_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];
    unsigned char aucXRES[D_NSP_TDYTH_NAAAS_4G_RES_LEN];
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS;

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME
* 结构体用途：				4G接入认证第8步HSS到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucAV             认证向量
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucAV[D_NSP_TDYTH_NAAAS_4G_AV_LEN];   //暂时认定只发送一个AV：RAND||XRES||KASME||AUTN 共104字节
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyEighthHSSToMME;


/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB
* 结构体用途：				4G接入认证第9步MME到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucRandom	 		随机数
* @param  pucAUTN			认证数据
* @param  pucKSIasme 		KSIasme
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucAuthReq[D_NSP_TDYTH_NAAAS_4G_RAND_LEN + D_NSP_TDYTH_NAAAS_4G_AUTN_LEN + D_NSP_TDYTH_NAAAS_4G_KASME_LEN];   //按理应该定义一个变量用来存储：RAND（32）||AUTN（24）||KSIASME（3bits） 级联后的信息
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyNinethMMEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_AuBodyTenthENBToUE
* 结构体用途：                4G接入认证第10步ENB到UE数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucRandom         随机数
* @param  pucAUTN           认证数据
* @param  pucKSIasme        KSIasme
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucAuthReq[D_NSP_TDYTH_NAAAS_4G_RAND_LEN + D_NSP_TDYTH_NAAAS_4G_AUTN_LEN + D_NSP_TDYTH_NAAAS_4G_KASME_LEN];   //按理应该定义一个变量用来存储：RAND（32）||AUTN（24）||KSIASME（3bits） 级联后的信息
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyTenthENBToUE;
/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB
* 结构体用途：				4G接入认证第11步UE到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucRes			用户认证响应
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucRes[D_NSP_TDYTH_NAAAS_4G_RES_LEN];
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyEleventhUEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_AuBodyTwelfthENBToMME
* 结构体用途：                4G接入认证第12步ENB到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucRes            用户认证响应
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucRes[D_NSP_TDYTH_NAAAS_4G_RES_LEN];
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyTwelfthENBToMME;

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB
* 结构体用途：				4G接入认证第13步MME到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucRes			用户认证响应
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucResult[D_NSP_TDYTH_NAAAS_4G_AuResult_LEN];
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyThirteenthMMEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_AuBodyFourteenthENBToUE
* 结构体用途：                4G接入认证第14步ENB到UE数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  pucRes            用户认证响应
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int iDataLen;
	
    unsigned char aucResult[D_NSP_TDYTH_NAAAS_4G_AuResult_LEN];
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AuBodyFourteenthENBToUE;

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_UserInfo_Request
* 结构体用途：				MME向用户请求用户信息
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  ucUserInfo	    用户信息请求标识
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char ucUserInfo;
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_UserInfo_Request;

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_UserInfo_Response
* 结构体用途：				UE返回用户信息给MME
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI	 		国际移动订阅用户身份
* @param  aucUserName       用户姓名
* @param  aucUserID         用户身份证号
* @param  aucUserGps        用户GPS位置信息
* @param  pucUserStatus 	用户认证状态
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucUserName[20];
    unsigned char aucUserID[25];         //身份证号
    unsigned char aucUserGps[30];
    unsigned char aucUserStatus;       //暂定0代表未认证，1代表认证

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_UserInfo_Response;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS
* 结构体用途：                MME向HSS请求用户IP和端口
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  iDataLen          数据长度
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  ucUserInfo        用户信息请求标识
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char ucUserInfo;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFirstMMEToHSS;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME
* 结构体用途：                HSS向MME发送用户IP和端口
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  iDataLen          数据长度
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  aucIPs            所有用户IP
* @param  aiPorts           所有用户端口
* @param  iCount            用户个数
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
	unsigned int  aucDstPort;
    unsigned int  iDataLen;
    
    unsigned char aucIPs[500];
    int  aiPorts[50];
    int iCount;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodySecondHSSToMME;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE
* 结构体用途：                MME向UE发送用户信息请求
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  iDataLen          数据长度
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  ucUserInfo        用户信息请求标识
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char ucUserInfo;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyThirdMMEToUE;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFourthUEToMME;
* 结构体用途：                UE返回用户信息给MME
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  iDataLen          数据长度
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  aucIMSI           国际移动订阅用户身份
* @param  aucUserName       用户姓名
* @param  aucUserID         用户身份证号
* @param  aucUserGps        用户GPS位置信息
* @param  pucUserStatus     用户认证状态
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucUserName[20];
    unsigned char aucUserID[25];         //身份证号
    unsigned char aucUserGps[30];
    unsigned char aucUserStatus;       //暂定0代表未认证，1代表认证
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_UserInfo_Request_BodyFourthUEToMME;

/***********************************************************************
* 结构体名： St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS
* 结构体用途：				      4G注册过程第1步UE到HSS数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI	 		国际移动订阅用户身份
* @param  aucUserId         用户姓名
* @param  aucIdNum          身份证号
* @param  aucSex            性别
* @param  aucAddr           地址
* @param  aucKind 	        人员类别
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucCardID[11];
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN+1];
    unsigned char aucHSSID[15];
    unsigned char aucPIN[4];
    unsigned char aucPUK[8];

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
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_RebodyFirstUEToHSS;

/************************************************************
* 结构体名：				St_NSP_TDYTH_NAAAS_4G_ReBodySecondHSSToUE
* 结构体用途：				4G注册过程第2步HSS到UE数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucReqReg		    注册响应
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char aucReqReg;
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_ReBodySecondHSSToUE;

/***************************************************************附着*************************************************************************************/
/***********************************************************************
* 结构体名： St_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB
* 结构体用途：               4G附着过程第1步UE到eNB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI           国际移动订阅用户身份
* @param  aucPDN            PDN Type
* @param  aucPCO            PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB;

/***********************************************************************
* 结构体名： St_NSP_TDYTH_NAAAS_4G_AttBodySecondeNBToUE
* 结构体用途：               4G附着过程第2步eNB到UE数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucCRNTI          CRNTI
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char aucCRNTI[D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN];
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AttBodySecondeNBToUE;

/***********************************************************************
* 结构体名： St_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB
 * ToUE
* 结构体用途：               4G附着过程第3步UE到eNB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI           国际移动订阅用户身份
* @param  aucPDN            PDN Type
* @param  aucPCO            PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucPDN[D_NSP_TDYTH_NAAAS_4G_PDNTYPE_LEN ];
    unsigned char aucPCO[D_NSP_TDYTH_NAAAS_4G_PCO_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AttBodyThirdUEToeNB;

/***********************************************************************
* 结构体名： St_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME
 * ToUE
* 结构体用途：               4G附着过程第4步eNB到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI           国际移动订阅用户身份
* @param  aucPDN            PDN Type
* @param  aucPCO            PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucPDN[D_NSP_TDYTH_NAAAS_4G_PDNTYPE_LEN ];
    unsigned char aucPCO[D_NSP_TDYTH_NAAAS_4G_PCO_LEN];
    unsigned int uiEnb_Ue_Id;
	
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AttBodyFourtheNBToMME;

/***************************************************************ip分配*************************************************************************************/
/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW
* 结构体用途：                4G IP分配过程第1步MME到SGW数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI           国际移动用户识别码
* @param  ucPDNType         PDN 类型
* @param  aucPDNAddress     PDN 地址
* @param  ucPCO             PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char ucPDNType;
    unsigned char aucPDNAddress[D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN];
    unsigned char ucPCO;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFirstMMEToSGW;


/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW
* 结构体用途：                4G IP分配过程第2步SGW到PGW数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI           国际移动用户识别码
* @param  ucPDNType         PDN 类型
* @param  aucPDNAddress     PDN 地址
* @param  ucPCO             PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char ucPDNType;
    unsigned char aucPDNAddress[D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN];
    unsigned char ucPCO;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IPAllotBodySecondSGWToPGW;


/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW
* 结构体用途：                4G IP分配过程第3步PGW到SGW数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI           国际移动用户识别码
* @param  ucPDNType         PDN 类型
* @param  aucPDNAddress     PDN 地址
* @param  ucPCO             PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char ucPDNType;
    unsigned char aucPDNAddress[D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN];
    unsigned char ucPCO;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IPAllotBodyThirdPGWToSGW;


/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME
* 结构体用途：                4G IP分配过程第4步SGW到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucIMSI           国际移动用户识别码
* @param  ucPDNType         PDN 类型
* @param  aucPDNAddress     PDN 地址
* @param  ucPCO             PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char ucPDNType;
    unsigned char aucPDNAddress[D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN];
    unsigned char ucPCO;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFourthSGWToMME;


/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFifthMMEToENB
* 结构体用途：                4G IP分配过程第5步MME到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucGUTI           全球唯一临时UE标识
* @param  pucKenb           基站密钥
* @param  pucNH             下一跳密钥
* @param  pucNCC            下一跳链接数
* @param  ucPDNType         PDN 类型
* @param  aucPDNAddress     PDN 地址
* @param  ucPCO             PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char pucKenb[D_NSP_TDYTH_NAAAS_4G_KEY_LEN];
    unsigned char pucNH[D_NSP_TDYTH_NAAAS_4G_NH_LEN];
    unsigned char pucNCC;
    unsigned char ucPDNType;
    unsigned char aucPDNAddress[D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN];
    unsigned char ucPCO;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IPAllotBodyFifthMMEToENB;


/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE
* 结构体用途：                4G IP分配过程第6步ENB到UE数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucGUTI           全球唯一临时UE标识
* @param  ucPDNType         PDN 类型
* @param  aucPDNAddress     PDN 地址
* @param  ucPCO             PCO
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char ucPDNType;
    unsigned char aucPDNAddress[D_NSP_TDYTH_NAAAS_4G_PDN_ADDRESS_IP_LEN];
    unsigned char ucPCO;


    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IPAllotBodySixthENBToUE;


/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB
* 结构体用途：                4G IP分配过程第7步UE到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  flag              附着完成标识
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char flag;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IPAllotBodySeventhUEToENB;


/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_IPAllotBodyEighthENBToMME
* 结构体用途：                4G IP分配过程第8步ENB到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  flag              附着完成标识
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char flag;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IPAllotBodyEighthENBToMME;

/***********************************************************************守护进程***********************************************************************/
/***********************************************************************
* 结构体名： St_NSP_TDYTH_NAAAS_4G_DaemonControl
* 结构体用途：                    守护进程与前端、后台之间功能控制结构体
* @param  ucFuncType               请求功能类型
* @param  ucFrontend               前端类型
* @param  ucFunction               需要执行的功能
************************************************************************/
typedef struct
{
    unsigned char ucFuncType;        //请求功能类型
    unsigned char ucFrontend;        //前端类型
    unsigned int  uiFunction;        //需要执行的功能
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_DaemonControl;

typedef struct 
{
    int X_coordinates;
    int Y_coordinates;
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_MeasureInfo;

/***********************************************************************X2切换***********************************************************************/
/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_N2_BodyFirstUEToSENB
* 结构体用途：                4G eNB切换第1步UE到源ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  uiCommunityId     测量小区ID
* @param  uiRSCP            RSCP
* @param  uiRSCP            RSRQ
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int uiCommunityId;
    unsigned int uiRSCP;
    unsigned int uiRSRQ;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodyFirstUEToSENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_N2_BodySecondSENBToTENB
* 结构体用途：                4G eNB切换第2步源ENB到目标ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiOld_Enb_Ue_X2Id     Old_eNB_UE_X2AP_ID
* @param  uiMme_Ue_S1Id         MME_UE_S1AP_ID
* @param  aucGUMMEI_Id          GUMMEI
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiOld_Enb_Ue_X2Id;
    unsigned int  uiMME_Ue_S1Id;
    unsigned char aucGUMMEI_Id[D_NSP_TDYTH_NAAAS_4G_GUMMEI_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned int  aucNCC;
    unsigned char aucKenb[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucOldC_RNTI[D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodySecondSENBToTENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_N2_BodyThirdTENBToSENB
* 结构体用途：                4G eNB切换第3步目标ENB到源ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiNew_Enb_Ue_X2Id     New_eNB_UE_X2AP_ID
* @param  uiOld_Enb_Ue_X2Id     Old_eNB_UE_X2AP_ID
* @param  aucCRNTI              CRNTI
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiNew_Enb_Ue_X2Id;
    unsigned int  uiOld_Enb_Ue_X2Id;
    unsigned char aucCRNTI[D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN];
    unsigned int  aucNCC;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodyThirdTENBToSENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_N2_BodyFourthSENBToUE
* 结构体用途：                4G eNB切换第4步源ENB到UE数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  aucCRNTI              CRNTI
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucCRNTI[D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN];
    unsigned int aucNCC;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodyFourthSENBToUE;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_N2_BodyFifthSENBToTENB
* 结构体用途：                4G eNB切换第5步源ENB到目标ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiNew_Enb_Ue_X2Id     New_eNB_UE_X2AP_ID
* @param  uiOld_Enb_Ue_X2Id     Old_eNB_UE_X2AP_ID
* @param  ucPdcpSn              序号，保证顺序提交以及检测重复的包
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiNew_Enb_Ue_X2Id;
    unsigned int  uiOld_Enb_Ue_X2Id;
    unsigned char ucPdcpSn;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodyFifthSENBToTENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_N2_BodySixthUEToTENB
* 结构体用途：                4G eNB切换第6步UE到目标ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  ucResponese           响应
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char ucResponese;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodySixthUEToTENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_N2_BodySeventhTENBToMME
* 结构体用途：                4G eNB切换第7步目标ENB到MME数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiEnb_Ue_S1Id         使eNB区分每个UE
* @param  aucEcgi               在PLMN中全局标识一个小区
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiEnb_Ue_S1Id;
    unsigned char aucECGI[D_NSP_TDYTH_NAAAS_4G_ECGI_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodySeventhTENBToMME;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_HandOverBodyEighthTENBToMME
* 结构体用途：                4G eNB切换第8步MME到目标ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiEnb_Ue_S1Id         使eNB区分每个UE
* @param  ucCellId              区分小区
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiEnb_Ue_S1Id;
    unsigned char ucCellId;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodyEighthMMEToTENB;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_N2_BodyNinthTENBToSENB
* 结构体用途：                4G eNB切换第9步目标ENB到源ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiNew_Enb_Ue_X2Id     New_eNB_UE_X2AP_ID
* @param  uiOld_Enb_Ue_X2Id     Old_eNB_UE_X2AP_ID
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiNew_Enb_Ue_X2Id;
    unsigned int  uiOld_Enb_Ue_X2Id;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_N2_BodyNinthTENBToSENB;

/***********************************************************************S1切换***********************************************************************/
/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFirstUEToSENB
* 结构体用途：                4G eNB S1切换第1步UE到源ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  uiCommunityId     测量小区ID
* @param  uiRSCP            RSCP
* @param  uiRSCP            RSRQ
* @param  usPCI             uiPCI
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct {
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int uiCommunityId;
    unsigned int uiRSCP;
    unsigned int uiRSRQ;
    unsigned short usPCI;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];

} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFirstUEToSENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME
* 结构体用途：                4G eNB S1切换第2步源ENB到MME数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  uiMme_Ue_S1Id         MME_UE_S1AP_ID
* @param  uiEnb_Ue_S1Id         ENB_UE_S1AP_ID
* @param  ucTarget_Cell_Id      Target_Cell_Id
* @param  aucKenb1              K*enb
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiMme_Ue_S1Id;
    unsigned int  uiEnb_Ue_S1Id;
    unsigned int  ucTarget_Cell_Id;
    unsigned char aucKenb1[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned int  aucNCC;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB
* 结构体用途：                4G eNB S1切换第3步MME到目标ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  aucKenb2              K*+enb
* @param  aucNH                 NH*NCC
* @param  ucNCC                 NCC
* @param  uiMme_Ue_S1Id         MME_UE_S1AP_ID
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucKenb2[D_NSP_TDYTH_NAAAS_4G_KENB_LEN];
    unsigned char aucNH[D_NSP_TDYTH_NAAAS_4G_NH_LEN];
    unsigned char ucNCC;
    unsigned int  uiMme_Ue_S1Id;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME
* 结构体用途：                4G eNB S1切换第4步ENB到MME数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  uiEnb_Ue_S1Id         ENB_UE_S1AP_ID
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiNewEnb_Ue_S1Id;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB
* 结构体用途：                4G eNB S1切换第5步MME到源ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  ucNCC                 NCC
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
	
    unsigned char ucNCC;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFifthMMEToSENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE
* 结构体用途：                4G eNB S1切换第6步源ENB到UE数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  ucTarget_Cell_Id      Target_Cell_Id
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char ucTarget_Cell_Id;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySixthSENBToUE;
/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySeventhSENBToMME
* 结构体用途：                4G eNB S1切换第7步源ENB到MME数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  aucPDCP_SN            PDCP_SN
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucPDCP_SN;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySeventhSENBToMME;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEighthTENBToMME
* 结构体用途：                4G eNB S1切换第8步目标ENB到MME数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  aucPDCP_SN            PDCP_SN
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucPDCP_SN;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEighthMMEToTENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB
* 结构体用途：                4G eNB S1切换第9步UE到目标ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  ucResponese           响应
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char ucResponese;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyNinthUEToTENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME
* 结构体用途：                4G eNB S1切换第10步目标ENB到MME数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  uiMME_UE_S1AP_Id      MME_UE_S1AP_Id
* @param  uieNB_UE_S1AP_Id      eNB_UE_S1AP_Id
* @param  aucECGI               ECGI
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiMME_UE_S1Id;
    unsigned int  uieNB_UE_S1Id;
    unsigned char aucECGI[D_NSP_TDYTH_NAAAS_4G_ECGI_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTenthTENBToMME;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEleventhMMEToSENB
* 结构体用途：                4G eNB S1切换第11步MME到源ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  uiMME_UE_S1AP_Id      MME_UE_S1AP_Id
* @param  uieNB_UE_S1AP_Id      eNB_UE_S1AP_Id
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiMME_UE_S1Id;
    unsigned int  uieNB_UE_S1Id;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyEleventhMMEToSENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTwelfthMMEToSENB
* 结构体用途：                4G eNB S1切换第12步源ENB到MME数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  uiMME_UE_S1AP_Id      MME_UE_S1AP_Id
* @param  uieNB_UE_S1AP_Id      eNB_UE_S1AP_Id
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiMME_UE_S1Id;
    unsigned int  uieNB_UE_S1Id;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyTwelfthSENBToMME;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB
* 结构体用途：                4G eNB Intra切换第1步UE到ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  uiCommunityId         测量小区ID
* @param  uiRSCP                RSCP
* @param  uiRSCP                RSRQ
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int uiCommunityId;
    unsigned int uiRSCP;
    unsigned int uiRSRQ;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyFirstUEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE
* 结构体用途：                4G eNB Intra切换第2步ENB到UE数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  ucTarget_Cell_Id      Target_Cell_Id
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char ucTarget_Cell_Id;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodySecondENBToUE;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyThirdUEToENB
* 结构体用途：                4G eNB Intra切换第2步ENB到UE数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort     	    源端口号
* @param  iDataLen              数据长度
* @param  ucResponese           响应
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char ucResponese;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_Intra_HandOverBodyThirdUEToENB;

/***********************************************************************MME中断***********************************************************************/
/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_LinkageInterruptMMEToENB
* 结构体用途：                MME中断数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort     	源端口号
* @param  iDataLen          数据长度
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_LinkageInterruptFirstMMEToENB;
/***********************************************************************MME恢复链接***********************************************************************/
/************************************************************
* 结构体名：                 St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB
* 结构体用途：               MME中断恢复数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort     	源端口号
* @param  iDataLen          数据长度
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_LinkageRecoverFirstMMEToENB;

/***********************************************************************测量报告***********************************************************************/

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB
* 结构体用途：                4G 发送测量报告第1步UE到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort     	源端口号
* @param  iDataLen          数据长度
* @param  uiCommunityId     测量小区ID
* @param  X_coordinates     X坐标
* @param  Y_coordinates     Y坐标
* @param  uiRSCP            RSCP
* @param  uiRSCP            RSRQ
* @param  usPCI             uiPCI
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct {
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned int uiCommunityId;
    int X_coordinates;
    int Y_coordinates;
    unsigned int uiRSCP;
    unsigned int uiRSRQ;
    unsigned int usPCI;
    unsigned int ucEARFCN;
    unsigned int aucBestEnbNum;


    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];

} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_HandOverMeasureBodyFirstUEToSENB;

/***********************************************************************IP上网***********************************************************************/

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB
* 结构体用途：                4G 上网第1步UE到源ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  uiDRBId           无线承载ID
* @param  aucDstIp          目的IP
* @param  aucSrcIp          源IP
* @param  aucGUTI           GUTI
* @param  aucRequest        请求内容
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct 
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiDRBId;
    unsigned char aucDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucRequest[D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];

} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IpBodyFirstUEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodySecondSENBToMME
* 结构体用途：                4G 上网第2步ENB到SGW数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiS1_TEID_UL          隧道端点标识
* @param  aucDstIp              目的IP
* @param  aucSrcIp              源IP
* @param  aucGTPDstId           GTP目的IP
* @param  aucGTPSrcId           GTP源IP
* @param  aucGUTI               GUTI
* @param  aucRequest            请求内容
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiS1_TEID_UL;
    unsigned char aucDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGTPDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGTPSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucRequest[D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IpBodySecondENBToSGW;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyThirdMMEToTENB
* 结构体用途：                4G 上网第3步SGW到目标PGW数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiS5_TEID_UL          隧道端点标识
* @param  aucDstIp              目的IP
* @param  aucSrcIp              源IP
* @param  aucGTPDstIp           GTP目的IP
* @param  aucGTPSrcIp           GTP源IP
* @param  aucGUTI               GUTI
* @param  aucRequest            请求内容
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiS5_TEID_UL;
    unsigned char aucDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGTPDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGTPSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucRequest[D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IpBodyThirdSGWToPGW;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_HandOverBodyFourthTENBToMME
* 结构体用途：                4G 上网第4步PGW到NET数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  aucDstIp              目的IP
* @param  aucSrcIp              源IP
* @param  aucGUTI               GUTI
* @param  aucRequest            请求内容
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucRequest[D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW
* 结构体用途：                4G 上网第5步NET到PGW数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  aucDstIp              目的IP
* @param  aucSrcIp              源IP
* @param  aucGUTI               GUTI
* @param  aucResult             返回内容
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucResult[D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S5_IpBodySixthPGWToSGW
* 结构体用途：                4G 上网第6步到PGW目标SGW数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiS5_TEID_DL          隧道端点标识
* @param  aucDstIp              目的IP
* @param  aucSrcIp              源IP
* @param  aucGTPDstIp           GTP目的IP
* @param  aucGTPSrcIp           GTP源IP
* @param  aucGUTI               GUTI
* @param  aucReslt              返回内容
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiS5_TEID_DL;
    unsigned char aucDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGTPDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGTPSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucResult[D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IpBodySixthPGWToSGW;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_S1_IpBodySeventhSGWToENB
* 结构体用途：                4G 上网第7步SGW到ENB数据包
* @param  aucTimeStamp          时戳
* @param  aucIdNumber           标识位
* @param  aucDstId              目的ID
* @param  aucSrcId              源ID
* @param  aucDstPort            目的端口号
* @param  aucSrcPort            源端口号
* @param  iDataLen              数据长度
* @param  uiS1_TEID_DL          隧道端点标识
* @param  aucDstIp              目的IP
* @param  aucSrcIp              源IP
* @param  aucGTPDstId           GTP目的IP
* @param  aucGTPSrcId           GTP源IP
* @param  aucGUTI               GUTI
* @param  aucResult             返回内容
* @param  aucTerminalId         终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiS1_TEID_DL;
    unsigned char aucDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGTPDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGTPSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucResult[D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IpBodySeventhSGWToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE
* 结构体用途：                4G 上网第8步ENB到UE数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  uiDRBId           无线承载ID
* @param  aucDstIp          目的IP
* @param  aucSrcIp          源IP
* @param  aucGUTI           GUTI
* @param  aucResult         返回内容
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiDRBId;
    unsigned char aucDstIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucSrcIp[D_NSP_TDYTH_NAAAS_4G_IP_LEN];
    unsigned char aucGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char aucResult[D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN];

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];

} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IpBodyEighthENBToUE;

/***********************************************************************专有承载建立***********************************************************************/
/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW
* 结构体用途：                4G专有承载的建立第1步PCRF到PGW数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW
* 结构体用途：                4G专有承载的建立第2步PGW到SGW数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度

* @param  aucIMSI           国际移动订阅用户身份
* @param  ucPTI             过程事物标识
* @param  aucQoS            EPS承载服务质量   
* @param  aucTFT            传输流模板/业务流模板  
* @param  uiS5_TEID         隧道端点标识
* @param  ucLBI             链接 EPS 承载标识

* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int iDataLen;

    
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char ucPTI;
    unsigned char aucQoS[D_NSP_TDYTH_NAAAS_4G_QoS_LEN];
    unsigned char aucTFT[D_NSP_TDYTH_NAAAS_4G_TFT_LEN];
    unsigned int  uiS5_TEID;
    unsigned char ucLBI:4;
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpSecondPGWToSGW;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME
* 结构体用途：                4G专有承载的建立第3步SGW到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度

* @param  aucIMSI           国际移动订阅用户身份
* @param  ucPTI             过程事物标识
* @param  aucQoS            EPS承载服务质量   
* @param  aucTFT            传输流模板/业务流模板  
* @param  uiS1_TEID         隧道端点标识
* @param  ucLBI             链接 EPS 承载标识

* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    
    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char ucPTI;
    unsigned char aucQoS[D_NSP_TDYTH_NAAAS_4G_QoS_LEN];
    unsigned char aucTFT[D_NSP_TDYTH_NAAAS_4G_TFT_LEN];
    unsigned int  uiS1_TEID;
    unsigned char ucLBI:4; 
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpThirdSGWToMME;





/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB
* 结构体用途：                4G专有承载的建立第4步MME到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度

* @param  ucPTI             过程事物标识
* @param  aucQoS            EPS承载服务质量   
* @param  aucTFT            传输流模板/业务流模板  
* @param  uiS1_TEID         隧道端点标识
* @param  ucLBI             链接 EPS 承载标识
* @param  ucBearerID        EPS承载标识

* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int iDataLen;

    unsigned char ucPTI;
    unsigned char aucQoS[D_NSP_TDYTH_NAAAS_4G_QoS_LEN];
    unsigned char aucTFT[D_NSP_TDYTH_NAAAS_4G_TFT_LEN];
    unsigned int  uiS1_TEID;
    unsigned char ucLBI:4; 
    unsigned char ucBearerID:4;
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpFourthMMEToENB;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE
* 结构体用途：                4G专有承载的建立第5步ENB到UE数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度

* @param  aucQoS            EPS承载服务质量   
* @param  aucTFT            传输流模板/业务流模板  
* @param  ucLBI             链接 EPS 承载标识
* @param  ucBearerID        EPS承载标识

* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int iDataLen;

    unsigned char aucQoS[D_NSP_TDYTH_NAAAS_4G_QoS_LEN];
    unsigned char aucTFT[D_NSP_TDYTH_NAAAS_4G_TFT_LEN];
    unsigned char ucLBI:4;  
    unsigned char ucBearerID:4;
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpFifthENBToUE;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB
* 结构体用途：                4G专有承载的建立第6步UE到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpSixthUEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME
* 结构体用途：                4G专有承载的建立第7步ENB到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度


* @param  uiS1_TEID         隧道端点标识
* @param  ucBearerID        EPS承载标识

* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiS1_TEID;
    unsigned char ucBearerID:4;
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpSeventhENBToMME;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB
* 结构体用途：                4G专有承载的建立第8步UE到ENB数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度

* @param  ucBearerID        EPS承载标识

* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
    
    unsigned char ucBearerID:4;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpEighthUEToENB;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME
* 结构体用途：                4G专有承载的建立第9步ENB到MME数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  ucResponse       响应消息
* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;
    
    unsigned char ucResponse;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpNinthENBToMME;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW
* 结构体用途：                4G专有承载的建立第10步MME到SGW数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度


* @param  uiS1_TEID         隧道端点标识
* @param  ucBearerID        EPS承载标识

* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiS1_TEID;
    unsigned char ucBearerID:4;
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpTenthMMEToSGW;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW
* 结构体用途：                4G专有承载的建立第11步SGW到PGW数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度


* @param  uiS5_TEID         隧道端点标识
* @param  ucBearerID        EPS承载标识

* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned int  uiS5_TEID;
    unsigned char ucBearerID:4;
    
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpEleventhSGWToPGW;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BearerSetUpTwelfthPGWToPCRF
* 结构体用途：                4G专有承载的建立第12步PGW到PCRF数据包
* @param  aucTimeStamp      时戳
* @param  aucIdNumber       标识位
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  aucDstPort        目的端口号
* @param  aucSrcPort        源端口号
* @param  iDataLen          数据长度
* @param  aucTerminalId     终端ID
************************************************************/

typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned int  iDataLen;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BearerSetUpTwelfthPGWToPCRF;
/***********************************************************************IMS域***********************************************************************/

//以前数据包中的一些固定部分,当做第二层头；可能需要改动，目前先这样
typedef struct
{
    time_t TimeStamp;
    unsigned char aucIdNumber:4;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  aucSrcPort;
    unsigned int  aucDstPort;
    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_SecHeader;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_SIPHeader
* 结构体用途：                IMS SIP头部
* @param  aucFrom            发起请求方的地址
* @param  aucTo              被注册的用户标识
* @param  aucCall_ID         标识对话
* @param  aucCSeq            标识事务，以及标识同一对话中请求消息的先后顺序
* @param  aucVia             记录请求消息路径，用于响应消息路由
* @param  uiMax_Forwards     消息的剩余跳数     
************************************************************/
typedef struct
{
    unsigned char aucFrom[D_NSP_TDYTH_NAAAS_4GSIP_From_LEN];//长度都还没有定义
    unsigned char aucTo[D_NSP_TDYTH_NAAAS_4GSIP_To_LEN];
    unsigned char aucCall_ID[D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN];
    unsigned char aucCSeq[D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN];
    unsigned char aucVia[D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN];
    unsigned int  uiMax_Forwards;//70
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_SIPHeader;



/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_Diameter_Header
* 结构体用途：                IMS Cx接口协议Diametert头部
* @param  ucVersion             协议版本号
* @param  aucMessageLength      diameter头的消息长度
* @param  ucCommandFlags        命令标记
* @param  aucCommandCode        命令码
* @param  uiApplicationID       应用标识
* @param  uiHopbyHopID          逐跳标识
* @param  uiEndtoEndID          端对端标识
************************************************************/
typedef struct
{
 
    unsigned char ucVersion;//1个字节，协议版本号，设置为1
    unsigned char aucMessageLength[D_NSP_TDYTH_NAAAS_4G_MessageLength_LEN];//diameter头的消息长度
    unsigned char ucCommandFlags;//命令标记8比特,前四位根据情况设定，后四位是0000。
    unsigned char aucCommandCode[D_NSP_TDYTH_NAAAS_4G_CommandCode_LEN];//3个8位组，24bit的命令码空间地址由IANA 统一管理。命令码16,777,214 和16,777,215是用作试验目的的。
    unsigned int  uiApplicationID;//4个字节。16777216,IANA分配给CX/DX接口的Diameter应用标识为16777216
    unsigned int  uiHopbyHopID;//逐跳标识。起始值是随机的，通常是单调递增。
    unsigned int  uiEndtoEndID;//端对端标识。用于检测重复消息。
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_Diameter_Header;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUETOPCSCF
* 结构体用途：                IMS注册第1步UE到PCSCF数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucHomeNetworkDomainName      用户归属网络域名
* @param  aucUEIp                       UE的IP地址
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucHomeNetworkDomainName[D_NSP_TDYTH_NAAAS_4G_HomeNetworkDomainName_LEN];
    unsigned char aucUEIp[D_NSP_TDYTH_NAAAS_4G_UEIp_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterFirstUEToPCSCF;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterSecondPCSCFTOICSCF
* 结构体用途：                IMS注册第2步PCSCF到ICSCF数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucHomeNetworkDomainName      用户归属网络域名
* @param  aucUEIp                       UE的IP地址
* @param  aucPCSCFNetworkId             PCSCF所在网络的标识信息
* @param  aucPCSCF_address_name         PCSCF的地址或域名
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucHomeNetworkDomainName[D_NSP_TDYTH_NAAAS_4G_HomeNetworkDomainName_LEN];
    unsigned char aucUEIp[D_NSP_TDYTH_NAAAS_4G_UEIp_LEN];
    unsigned char aucPCSCFNetworkId[D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN];
    unsigned char aucPCSCF_address_name[D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterSecondPCSCFToICSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFTOHSS
* 结构体用途：                IMS注册第3步ICSCF到HSS数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucPCSCFNetworkId             PCSCF所在网络的标识信息
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucPCSCFNetworkId[D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirdICSCFToHSS;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSTOICSCF
* 结构体用途：                IMS注册第4步HSS到ICSCF数据包
* @param  iDataLen                      数据长度
* @param  aucSCSCFName                  SCSCF的名字

* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucSCSCFName[D_NSP_TDYTH_NAAAS_4G_SCSCFName_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourthHSSToICSCF;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterFifthICSCFTOSCSCF
* 结构体用途：                IMS注册第5步ICSCF到SCSCF数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucUEIp                       UE的IP地址
* @param  aucPCSCFNetworkId             PCSCF所在网络的标识信息
* @param  aucPCSCF_address_name         PCSCF的地址或域名
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucUEIp[D_NSP_TDYTH_NAAAS_4G_UEIp_LEN];
    unsigned char aucPCSCFNetworkId[D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN];
    unsigned char aucPCSCF_address_name[D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterFifthICSCFToSCSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixthSCSCFTOHSS
* 结构体用途：                IMS注册第6步SCSCF到HSS数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucSCSCFName                  SCSCF的名字
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucSCSCFName[D_NSP_TDYTH_NAAAS_4G_SCSCFName_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixthSCSCFToHSS;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventhHSSTOSCSCF
* 结构体用途：                IMS注册第7步HSS到SCSCF数据包
* @param  iDataLen          数据长度
* @param  aucCK             加密密钥CA
* @param  aucIK             完整性密钥IA
* @param  aucRAND           随机数
* @param  aucAUTN           认证数据
* @param  aucXRES           期望响应
* @param  aucEnd            数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;
    
    unsigned char aucCK[D_NSP_TDYTH_NAAAS_4G_CK_LEN];
    unsigned char aucIK[D_NSP_TDYTH_NAAAS_4G_IK_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];
    unsigned char aucXRES[D_NSP_TDYTH_NAAAS_4G_RES_LEN];
    //其实是在一个认证向量AV里面，包含的上述内容。
    
    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventhHSSToSCSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighthSCSCFTOICSCF
* 结构体用途：                IMS注册第8步SCSCF到ICSCF数据包
* @param  iDataLen          数据长度
* @param  aucIMPI           私有用户标识
* @param  aucCK             加密密钥CA
* @param  aucIK             完整性密钥IA
* @param  aucRAND           随机数
* @param  aucAUTN           认证数据
* @param  aucEnd            数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;
    
    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucCK[D_NSP_TDYTH_NAAAS_4G_CK_LEN];
    unsigned char aucIK[D_NSP_TDYTH_NAAAS_4G_IK_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];
    //其实XRES存储在SCSCF里面。
    
    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighthSCSCFToICSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterNinthICSCFTOPCSCF
* 结构体用途：                IMS注册第9步ICSCF到PCSCF数据包
* @param  iDataLen          数据长度
* @param  aucIMPI           私有用户标识
* @param  aucCK             加密密钥CA
* @param  aucIK             完整性密钥IA
* @param  aucRAND           随机数
* @param  aucAUTN           认证数据
* @param  aucEnd            数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;
    
    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucCK[D_NSP_TDYTH_NAAAS_4G_CK_LEN];
    unsigned char aucIK[D_NSP_TDYTH_NAAAS_4G_IK_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];
    //P-CSCF从消息中取出并保存IK,CK。
    
    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterNinthICSCFToPCSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFTOUE
* 结构体用途：                IMS注册第10步PCSCF到UE数据包
* @param  iDataLen          数据长度
* @param  aucIMPI           私有用户标识
* @param  aucRAND           随机数
* @param  aucAUTN           认证数据

* @param  aucEnd            数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;
    
    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterTenthPCSCFToUE;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUETOPCSCF
* 结构体用途：                IMS注册第11步UE到PCSCF数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucHomeNetworkDomainName      用户归属网络域名
* @param  aucUEIp                       UE的IP地址
* @param  pucRes                        用户认证响应
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucHomeNetworkDomainName[D_NSP_TDYTH_NAAAS_4G_HomeNetworkDomainName_LEN];
    unsigned char aucUEIp[D_NSP_TDYTH_NAAAS_4G_UEIp_LEN];
    unsigned char aucRes[D_NSP_TDYTH_NAAAS_4G_RES_LEN];//长度都没有改变

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterEleventhUEToPCSCF;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterTwelfthPCSCFTOICSCF
* 结构体用途：                IMS注册第12步PCSCF到ICSCF数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucHomeNetworkDomainName      用户归属网络域名
* @param  aucUEIp                       UE的IP地址
* @param  aucPCSCFNetworkId             PCSCF所在网络的标识信息
* @param  aucPCSCF_address_name         PCSCF的地址或域名
* @param  pucRes                        用户认证响应
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucHomeNetworkDomainName[D_NSP_TDYTH_NAAAS_4G_HomeNetworkDomainName_LEN];
    unsigned char aucUEIp[D_NSP_TDYTH_NAAAS_4G_UEIp_LEN];
    unsigned char aucPCSCFNetworkId[D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN];
    unsigned char aucPCSCF_address_name[D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN];
    unsigned char aucRes[D_NSP_TDYTH_NAAAS_4G_RES_LEN];//长度都没有改变

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterTwelfthPCSCFToICSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFTOHSS
* 结构体用途：                IMS注册第13步ICSCF到HSS数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucPCSCFNetworkId             PCSCF所在网络的标识信息
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucPCSCFNetworkId[D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterThirteenthICSCFToHSS;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSTOICSCF
* 结构体用途：                IMS注册第14步HSS到ICSCF数据包
* @param  iDataLen                      数据长度
* @param  aucSCSCFName                  SCSCF的名字

* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucSCSCFName[D_NSP_TDYTH_NAAAS_4G_SCSCFName_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterFourteenthHSSToICSCF;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterFifteenthICSCFTOSCSCF
* 结构体用途：                IMS注册第15步ICSCF到SCSCF数据包
* @param  iDataLen                      数据长度
* @param  aucIMPI                       私有用户标识
* @param  aucIMPU                       公有用户标识
* @param  aucUEIp                       UE的IP地址
* @param  aucPCSCFNetworkId             PCSCF所在网络的标识信息
* @param  aucPCSCF_address_name         PCSCF的地址或域名
* @param  pucRes                        用户认证响应
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPI[D_NSP_TDYTH_NAAAS_4G_IMPI_LEN];
    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];
    unsigned char aucUEIp[D_NSP_TDYTH_NAAAS_4G_UEIp_LEN];
    unsigned char aucPCSCFNetworkId[D_NSP_TDYTH_NAAAS_4G_PCSCFNetworkId_LEN];
    unsigned char aucPCSCF_address_name[D_NSP_TDYTH_NAAAS_4G_PCSCF_address_name_LEN];
    unsigned char aucRes[D_NSP_TDYTH_NAAAS_4G_RES_LEN];//长度都没有改变

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterFifteenthICSCFToSCSCF;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixteenthSCSCFTOHSS
* 结构体用途：                IMS注册第16步SCSCF到HSS数据包
* @param  iDataLen                      数据长度
* @param  aucIMPU                       公有用户标识
* @param  aucEnd                        数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucIMPU[D_NSP_TDYTH_NAAAS_4G_IMPU_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterSixteenthSCSCFToHSS;

/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventeenthHSSTOSCSCF
* 结构体用途：               IMS注册第17步HSS到SCSCF数据包
* @param  iDataLen          数据长度

* @param  aucEnd            数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;
    
    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterSeventeenthHSSToSCSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighthSCSCFTOICSCF
* 结构体用途：                IMS注册第18步SCSCF到ICSCF数据包
* @param  iDataLen          数据长度
* @param  aucContact        用户归属网络的Contact信息

* @param  aucEnd            数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;
    
    unsigned char aucContact[D_NSP_TDYTH_NAAAS_4G_Contact_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterEighteenthSCSCFToICSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterNineteenthICSCFTOPCSCF
* 结构体用途：                IMS注册第19步ICSCF到PCSCF数据包
* @param  iDataLen          数据长度
* @param  aucContact        用户归属网络的Contact信息

* @param  aucEnd            数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucContact[D_NSP_TDYTH_NAAAS_4G_Contact_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterNineteenthICSCFToPCSCF;


/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_IMSRegisterTwentiethPCSCFTOUE
* 结构体用途：                IMS注册第20步PCSCF到UE数据包
* @param  iDataLen          数据长度
* @param  aucContact        用户归属网络的Contact信息

* @param  aucEnd            数据包结尾
************************************************************/
typedef struct
{
    unsigned int iDataLen;

    unsigned char aucContact[D_NSP_TDYTH_NAAAS_4G_Contact_LEN];

    unsigned char aucEnd[D_NSP_TDYTH_NAAAS_4G_END_LEN];  //数据包结尾，目前固定为end字符串来表示数据结尾
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_IMSRegisterTwentiethPCSCFToUE;

typedef struct
{
    unsigned char aucVersion;

    unsigned char aucCommand[2];

    unsigned int aucDataLength;

    unsigned char aucBody[500];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_NoticeNAAAS;

typedef struct
{
    unsigned char aucVersion;

    unsigned char  aucMode;

    unsigned char aucReserved;

    unsigned int aucLength;

    unsigned char aucSQN[4];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_NAAASHEADER;


/***********************************************************************小区选择***********************************************************************/
/************************************************************
* 结构体名：             St_NSP_TDYTH_NAAAS_4G_BroadcastBody
* 结构体用途：                    ENB向UE发送广播包，用来标明自己的一些信息
* @param  uiDataLen             数据长度
* @param  uiENBID               基站唯一标识符（6位数）
* @param  uiPCI                 物理小区标识（0-503）
* @param  uiEARFCN              绝对频点号（暂时不考虑异频）暂定38400
* @param  iRSRP                 信号强度
* @param  iCellRP               全称cellReselectionPriority，小区重选优先级，LTE中默认5
* @param  iQrxlevmin            本小区最小接入电平，默认-128
* @param  aucTAI                本eNB所属TA的TAI

//有待添加。。。。。。
* @param  aucENBIp              ENB的IP      //实际中并无这两个参数,而是物理层上同步信号所需要的相关参数
* @param  uiENBPort             ENB的Port
************************************************************/
typedef struct
{
    unsigned int uiDataLen;

    unsigned int uiENBID;
    unsigned int uiPCI;
    unsigned int uiEARFCN;
    int iRSRP;
    int iCellRP;
    int iQrxlevmin;
    unsigned char aucTAI[D_NSP_TDYTH_NAAAS_4G_TAI_LEN];

    unsigned char aucENBIp[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  uiENBPort;
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_BroadcastBody;

/***********************************************************************攻击相关***********************************************************************/
/***********************************************************************
* 结构体名： St_NSP_TDYTH_NAAAS_4G_AucRefuseBody
* 结构体用途：    用于ENB拒绝UE的认证请求（该ENB无法进行认证）
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  uiSrcPort         目的端口号
* @param  uiDstPort         源端口号
* @param  iPCI              该ENB的PCI
* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int  uiSrcPort;
    unsigned int  uiDstPort;
    int iPCI;

    unsigned char aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_AucRefuseBody;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE
* 结构体用途：				4G TAU流程第2步ENB到UE数据包    /   TAU Reject
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  uiSrcPort         目的端口号
* @param  uiDstPort         源端口号
* @param  uiDataLen         数据长度

* @param  iEMM_Cause        拒绝原因

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDstPort;
    unsigned int    uiDataLen;

    int iEMM_Cause;

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE;


/***********************************************************************TAU***********************************************************************/
/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB
* 结构体用途：				4G TAU流程第1步UE到ENB数据包    /   TAU Request
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  aucSrcId          源ID
* @param  uiSrcPort         目的端口号
* @param  uiDstPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucOldGUTI        旧GUTI
* @param  aucOldTAI         旧TAI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDstPort;
    unsigned int    uiDataLen;

    unsigned char   aucOldGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char   aucOldTAI[D_NSP_TDYTH_NAAAS_4G_TAI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME
* 结构体用途：				4G TAU流程第2步ENB到New MME数据包   /   TAU Request
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucOldGUTI        旧GUTI
* @param  aucOldTAI         旧TAI
* @param  aucNewTAI         新TAI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucOldGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char   aucOldTAI[D_NSP_TDYTH_NAAAS_4G_TAI_LEN];
    unsigned char   aucNewTAI[D_NSP_TDYTH_NAAAS_4G_TAI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodySecondENBToMME;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME
* 结构体用途：				4G TAU流程第3步New MME到Old MME数据包   /   Context Request
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucOldGUTI        旧GUTI
* @param  aucOldTAI         旧TAI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucOldGUTI[D_NSP_TDYTH_NAAAS_4G_GUTI_LEN];
    unsigned char   aucOldTAI[D_NSP_TDYTH_NAAAS_4G_TAI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyThirdNewMMEToOldMME;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME
* 结构体用途：				4G TAU流程第4步Old MME到New MME数据包   /   Context Response
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	通过OldGUTI查询到的UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
	time_t TimeStamp;
	unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
	unsigned int    uiDstPort;
	unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
	unsigned int    uiSrcPort;
	unsigned int    uiDataLen;

	unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

	unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyFourthOldMMEToNewMME;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyFifthNewMMEToOldMME
* 结构体用途：				4G TAU流程第5步New MME到Old MME数据包   /   Context Acknowledge
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyFifthNewMMEToOldMME;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW
* 结构体用途：				4G TAU流程第7步New MME到New SGW数据包   /   Create Session Request
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodySeventhNewMMEToNewSGW;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW
* 结构体用途：				4G TAU流程第8步New SGW到PGW数据包   /   Modify Bearer Request
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyEighthNewSGWToPGW;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW
* 结构体用途：				4G TAU流程第9步PGW到New SGW数据包   /   Modify Bearer Response
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyNinthPGWToNewSGW;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME
* 结构体用途：				4G TAU流程第10步New SGW到New MME数据包   /   Create Session Response
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyTenthNewSGWToNewMME;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS
* 结构体用途：				4G TAU流程第11步New MME到HSS数据包   /   Update Location Request
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyEleventhNewMMEToHSS;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME
* 结构体用途：				4G TAU流程第12步HSS到Old MME数据包   /   Cancel Location Request
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyTwelfthHSSToOldMME;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS
* 结构体用途：				4G TAU流程第13步Old MME到HSS数据包   /   Cancel Location Ack
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyThirteenthOldMMEToHSS;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME
* 结构体用途：				4G TAU流程第14步HSS到New MME数据包   /   Update Location Ack
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyFourteenthHSSToNewMME;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW
* 结构体用途：				4G TAU流程第15步Old MME到Old SGW数据包   /   Delete Session Request
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyFifteenthOldMMEToOldSGW;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodySixteenthOldSGWToOldMME
* 结构体用途：				4G TAU流程第16步Old SGW到Old MME数据包   /   Delete Session Response
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucIMSI       	UE的IMSI

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodySixteenthOldSGWToOldMME;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE
* 结构体用途：				4G TAU流程第17步New MME到UE数据包   /   TAU Accept
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucTAIList        新的TAI表
* @param  iTAIList_Size     TAI表的长度

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucTAIList[10][D_NSP_TDYTH_NAAAS_4G_TAI_LEN];
    int             iTAIList_Size;

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodySeventeenthNewMMEToUE;

/************************************************************
* 结构体名：		            St_NSP_TDYTH_NAAAS_4G_TauBodyEighteenthUEToNewMME
* 结构体用途：				4G TAU流程第18步UE到New MME数据包   /   TAU Complete
* @param  aucTimeStamp      时戳
* @param  aucDstId          目的ID
* @param  uiDstPort         目的端口号
* @param  aucSrcId          源ID
* @param  uiSrcPort         源端口号
* @param  uiDataLen         数据长度

* @param  aucTerminalId     终端ID
************************************************************/
typedef struct
{
    time_t TimeStamp;
    unsigned char   aucDstId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiDstPort;
    unsigned char   aucSrcId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
    unsigned int    uiSrcPort;
    unsigned int    uiDataLen;

    unsigned char   aucTerminalId[D_NSP_TDYTH_NAAAS_4G_ID_LEN];
} __attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_TauBodyEighteenthUEToNewMME;



//UE端存储ENB基本信息
typedef struct {
    unsigned int uiENBID;       //基站唯一标识
    unsigned int uiPCI;         //物理小区标识
    unsigned int uiEARFCN;      //载波频点号（相当于小区的频段）
    int iRSRP;                  //信号强度（Qrxlevmeas）
    int iCellRP;                //cellReselectionPriority，小区重选优先级参数LTE默认为5
    int iQrxlevmin;             //本小区最小接入电平，默认-128
    unsigned char aucTAI[D_NSP_TDYTH_NAAAS_4G_TAI_LEN];

    int iIsBAN;                 //表示此ENB是否被禁用。1为禁用，0为可用。
    int iIsUpdate;              //用于标识该ENB是否为最近更新；0为未更新，1为已经更新。
    //收到ENB广播包后将此参数置为1，表示已经更新。进行小区重选时检查此参数，若为0，删除此基站信息，若为1，将其置为0。
    //该标识用于删除基站信息，前提是基站广播间隔比UE小区选择间隔短。

    unsigned char aucENBIp[D_NSP_TDYTH_NAAAS_4G_ID_LEN];    //标识基站的IP地址（现实中没有，仿真需要）
    unsigned int  uiENBPort;                                //标识基站的端口号（现实中没有，仿真需要）
}__attribute__((packed)) St_NSP_TDYTH_NAAAS_4G_EnbInfo;




#ifdef __cplusplus
}
#endif

#endif //NSP_TDYTH_NAAAS_4G_PROTO_H
