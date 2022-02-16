#ifndef NSP_TDYTH_NAAAS_4G_COMMON_H
#define NSP_TDYTH_NAAAS_4G_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

//字段长度大小定义
#define D_NSP_TDYTH_NAAAS_4G_IMSI_BITS	    49
#define D_NSP_TDYTH_NAAAS_4G_GUTI_BITS	    76
#define D_NSP_TDYTH_NAAAS_4G_HSSID_BITS	    8
#define D_NSP_TDYTH_NAAAS_4G_SNID_BITS	    24
#define D_NSP_TDYTH_NAAAS_4G_KEY_BITS	    256
#define D_NSP_TDYTH_NAAAS_4G_MAC_BITS	    128
#define D_NSP_TDYTH_NAAAS_4G_SQN_BITS	    48
#define D_NSP_TDYTH_NAAAS_4G_RES_BITS	    128
#define D_NSP_TDYTH_NAAAS_4G_RAND_BITS	    256
#define D_NSP_TDYTH_NAAAS_4G_IK_BITS	    128
#define D_NSP_TDYTH_NAAAS_4G_CK_BITS	    128
#define D_NSP_TDYTH_NAAAS_4G_AK_BITS	    48
#define D_NSP_TDYTH_NAAAS_4G_KASME_BITS	    256
#define D_NSP_TDYTH_NAAAS_4G_KSIASME_BITS	8
#define D_NSP_TDYTH_NAAAS_4G_AMF_BITS	    16
#define D_NSP_TDYTH_NAAAS_4G_AUTN_BITS 	    D_NSP_TDYTH_NAAAS_4G_SQN_BITS + D_NSP_TDYTH_NAAAS_4G_AMF_BITS + D_NSP_TDYTH_NAAAS_4G_MAC_BITS  //192
#define D_NSP_TDYTH_NAAAS_4G_AV_BITS	    D_NSP_TDYTH_NAAAS_4G_RAND_BITS + D_NSP_TDYTH_NAAAS_4G_RES_BITS + D_NSP_TDYTH_NAAAS_4G_KASME_BITS + D_NSP_TDYTH_NAAAS_4G_AUTN_BITS  //832


#define D_NSP_TDYTH_NAAAS_4G_IMSI_LEN       15
#define D_NSP_TDYTH_NAAAS_4G_GUTI_LEN	    ((D_NSP_TDYTH_NAAAS_4G_GUTI_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_HSSID_LEN      ((D_NSP_TDYTH_NAAAS_4G_HSSID_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_SNID_LEN	    ((D_NSP_TDYTH_NAAAS_4G_SNID_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_KEY_LEN	    ((D_NSP_TDYTH_NAAAS_4G_KEY_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_MAC_LEN	    ((D_NSP_TDYTH_NAAAS_4G_MAC_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_SQN_LEN	    ((D_NSP_TDYTH_NAAAS_4G_SQN_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_RES_LEN	    ((D_NSP_TDYTH_NAAAS_4G_RES_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_RAND_LEN	    ((D_NSP_TDYTH_NAAAS_4G_RAND_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_IK_LEN		    ((D_NSP_TDYTH_NAAAS_4G_IK_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_CK_LEN		    ((D_NSP_TDYTH_NAAAS_4G_CK_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_AK_LEN		    ((D_NSP_TDYTH_NAAAS_4G_AK_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_KASME_LEN	    ((D_NSP_TDYTH_NAAAS_4G_KASME_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_AMF_LEN	    ((D_NSP_TDYTH_NAAAS_4G_AMF_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_AV_LEN		    ((D_NSP_TDYTH_NAAAS_4G_AV_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_AUTN_LEN 	    ((D_NSP_TDYTH_NAAAS_4G_AUTN_BITS + 7)/8)
#define D_NSP_TDYTH_NAAAS_4G_KSIASME_LEN    ((D_NSP_TDYTH_NAAAS_4G_KSIASME_BITS + 7)/8)

/***************************X2切换*********************************/
#define D_NSP_TDYTH_NAAAS_4G_Measure_Threshold           	 80   //测量阈值
#define D_NSP_TDYTH_NAAAS_4G_RSCP_SCOPE_LEN                  1500
#define D_NSP_TDYTH_NAAAS_4G_RSRQ_SCOPE_LEN                  1500

#define D_NSP_TDYTH_NAAAS_4G_GUMMEI_LEN                      10
#define D_NSP_TDYTH_NAAAS_4G_CRNTI_LEN     					 2
#define D_NSP_TDYTH_NAAAS_4G_ECGI_LEN                        6
/***************************S1切换*********************************/
#define D_NSP_TDYTH_NAAAS_4G_PCI_LEN                        2
#define D_NSP_TDYTH_NAAAS_4G_EARFCN_LEN                     2	

#define D_NSP_TDYTH_NAAAS_4G_ComLength_S1SCOPE_LEN                  10000
#define D_NSP_TDYTH_NAAAS_4G_ComLength_X2SCOPE_LEN                  1000
#define D_NSP_TDYTH_NAAAS_4G_ComLength_INSCOPE_LEN                  300

//UE、MME、HSS 缓冲区、IP和端口宏定义
#define D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN        1024
#define D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN       1024
#define D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN       1024
#define D_NSP_TDYTH_NAAAS_4G_AUC_BUFF_LEN       1024
#define D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN       1024
#define D_NSP_TDYTH_NAAAS_4G_SGW_BUFF_LEN       1024
#define D_NSP_TDYTH_NAAAS_4G_PGW_BUFF_LEN       1024
#define D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN    1024
	
#define D_NSP_TDYTH_NAAAS_UE_IP                 "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_UE2_IP                "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_MME1_IP               "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_MME2_IP               "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_HSS_IP                "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_AUC_IP                "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_ENB1_IP               "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_ENB2_IP               "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_ENB3_IP               "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_SGW1_IP               "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_SGW2_IP               "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_PGW_IP                "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_NET_IP                "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_PCRF_IP               "127.0.0.1"	
#define D_NSP_TDYTH_NAAAS_PCSCF_IP              "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_ICSCF_IP              "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_SCSCF_IP              "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_DAEMON_IP             "127.0.0.1"   //守护进程IP
#define D_NSP_TDYTH_NAAAS_UEFRONT_IP            "127.0.0.1"   //UE前端IP
#define D_NSP_TDYTH_NAAAS_MMEFRONT_IP           "127.0.0.1"   //MME前端IP
#define D_NSP_TDYTH_NAAAS_IP                    "127.0.0.1"   //NAAAS系统的IP
//#define D_NSP_TDYTH_NAAAS_UE_IP               "192.168.1.138"
//#define D_NSP_TDYTH_NAAAS_MME1_IP             "192.168.1.115"
//#define D_NSP_TDYTH_NAAAS_HSS_IP              "192.168.1.129"
//#define D_NSP_TDYTH_NAAAS_AUC_IP              "192.168.1.129"
#define D_NSP_TDYTH_NAAAS_UE_PORT               16664
#define D_NSP_TDYTH_NAAAS_UE2_PORT              16679
#define D_NSP_TDYTH_NAAAS_MME1_PORT             16665
#define D_NSP_TDYTH_NAAAS_MME2_PORT             16687
#define D_NSP_TDYTH_NAAAS_HSS_PORT              16666
#define D_NSP_TDYTH_NAAAS_AUC_PORT              16667

#define D_NSP_TDYTH_NAAAS_ENB1_PORT             16668
#define D_NSP_TDYTH_NAAAS_ENB2_PORT             16678
#define D_NSP_TDYTH_NAAAS_ENB3_PORT             16686

#define D_NSP_TDYTH_NAAAS_SGW1_PORT             16669
#define D_NSP_TDYTH_NAAAS_SGW2_PORT             16688
#define D_NSP_TDYTH_NAAAS_PGW_PORT              16670
#define D_NSP_TDYTH_NAAAS_DAEMON_PORT           16671
#define D_NSP_TDYTH_NAAAS_DAEMONUE_PORT         16672
#define D_NSP_TDYTH_NAAAS_DAEMONMME_PORT        16673	
#define D_NSP_TDYTH_NAAAS_UEFRONT_PORT          16674 
#define D_NSP_TDYTH_NAAAS_MMEFRONT_PORT         16675
#define D_NSP_TDYTH_NAAAS_UEF_PORT              16676
#define D_NSP_TDYTH_NAAAS_MMEF_PORT             16677

#define D_NSP_TDYTH_NAAAS_NET_PORT              16680
#define D_NSP_TDYTH_NAAAS_PCRF_PORT             16681

#define D_NSP_TDYTH_NAAAS_PCSCF_PORT            16682 
#define D_NSP_TDYTH_NAAAS_ICSCF_PORT            16683  
#define D_NSP_TDYTH_NAAAS_SCSCF_PORT            16684

#define D_NSP_TDYTH_NAAAS_PORT                  16685

//mysql数据库参数相关宏定义
#define D_NSP_TDYTH_NAAAS_4G_DB_IP      "127.0.0.1"
#define D_NSP_TDYTH_NAAAS_4G_DB_PORT     3306
#define D_NSP_TDYTH_NAAAS_4G_DB_USER    "root"
#define D_NSP_TDYTH_NAAAS_4G_DB_PASSWD  "123456"
#define D_NSP_TDYTH_NAAAS_4G_DB_DB      "TDYTH"

//错误码
#define D_NSP_TDYTH_NAAAS_4G_SUCCESS					0x0
#define D_NSP_TDYTH_NAAAS_4G_ERR_AUTH                   0x00000001
#define D_NSP_TDYTH_NAAAS_4G_ERR_LEN_MISMATCH			0x00000002
#define D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL				0x00000003
#define D_NSP_TDYTH_NAAAS_4G_ERR_OUT_OF_RANGE			0x00000004
#define D_NSP_TDYTH_NAAAS_4G_ERR_FILE_OPEN_FAILED		0x00000005
#define D_NSP_TDYTH_NAAAS_4G_ERR_FILE_READ_FAILED		0x00000006
#define D_NSP_TDYTH_NAAAS_4G_ERR_FILE_WRITE_FAILED		0x00000007
#define D_NSP_TDYTH_NAAAS_4G_ERR_NET_ESTBLSH_ERROR		0x00000008
#define D_NSP_TDYTH_NAAAS_4G_ERR_NET_SEND_ERROR			0x00000009
#define D_NSP_TDYTH_NAAAS_4G_ERR_NET_RECV_ERROR			0x00000010
#define D_NSP_TDYTH_NAAAS_4G_ERR_NET_RECV_TIMEOUT       0x00000011
#define D_NSP_TDYTH_NAAAS_4G_ERR_NET_SOCKET             0x00000012
#define D_NSP_TDYTH_NAAAS_4G_ERR_NET_SETSOCKETOPT       0x00000013
#define D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND               0x00000014
#define D_NSP_TDYTH_NAAAS_4G_ERR_GEN                    0x00000015

#define D_NSP_TDYTH_NAAAS_4G_PROTO_INVALID				0x01000001
#define D_NSP_TDYTH_NAAAS_4G_PROTO_SQN_ERR				0x01000002
#define D_NSP_TDYTH_NAAAS_4G_PROTO_MAC_MISMATCH			0x01000003

#define D_NSP_TDYTH_NAAAS_4G_ID_NOTEXIST				0x02000001
#define D_NSP_TDYTH_NAAAS_4G_ID_INVALID					0x02000002
#define D_NSP_TDYTH_NAAAS_4G_ID_MISMATCH				0x02000003

#define D_NSP_TDYTH_NAAAS_4G_DB_ID_NOTEXIST				0x03000001
#define D_NSP_TDYTH_NAAAS_4G_DB_ID_INVALID				0x03000002
#define D_NSP_TDYTH_NAAAS_4G_DB_ID_MISMATCH				0x03000003
#define D_NSP_TDYTH_NAAAS_4G_DB_CONNECT_ERR				0x03000004
#define D_NSP_TDYTH_NAAAS_4G_DB_SELECT_ERR				0x03000005
#define D_NSP_TDYTH_NAAAS_4G_DB_INSERT_ERR				0x03000006
#define D_NSP_TDYTH_NAAAS_4G_DB_UPDATE_ERR				0x03000007
#define D_NSP_TDYTH_NAAAS_4G_DB_DELETE_ERR				0x03000008

//UE状态机宏定义
#define D_NSP_TDYTH_NAAAS_4G_UE_debug_off		0x10000001		//正常模式，调试关闭
#define D_NSP_TDYTH_NAAAS_4G_UE_debug_on		0x10000002		//调试模式
#define D_NSP_TDYTH_NAAAS_4G_UE_exit		    0x10000099	        //退出选择操作


/********************************守护进程(0x09..开头)*********************************/
#define D_NSP_TDYTH_NAAAS_4G_UE_POWERON			0x09000001
#define D_NSP_TDYTH_NAAAS_4G_UE_REGISTER		0x09000002
#define D_NSP_TDYTH_NAAAS_4G_UE_PHONE			0x09000003
#define D_NSP_TDYTH_NAAAS_4G_UE_LOCATION		0x09000004
#define D_NSP_TDYTH_NAAAS_4G_UE_POWEROFF		0x09000005
#define D_NSP_TDYTH_NAAAS_4G_UE_RESTART			0x09000006
#define D_NSP_TDYTH_NAAAS_4G_UE_ATTACH			0x09000007
#define D_NSP_TDYTH_NAAAS_4G_UE_WAITAUTH		0x09000008
#define D_NSP_TDYTH_NAAAS_4G_UE_SIMINFO		    0x0900000c
#define D_NSP_TDYTH_NAAAS_4G_UE_SWITCH		    0x0900000d	

#define D_NSP_TDYTH_NAAAS_4G_MME_ONLINEUSERINFO		0x0900000a
#define D_NSP_TDYTH_NAAAS_4G_MME_REGUSERINFO		0x0900000b
#define D_NSP_TDYTH_NAAAS_4G_MME_IPALLOT			0x09000010
#define D_NSP_TDYTH_NAAAS_4G_MME_AUTHBEGIN			0x09000011
#define D_NSP_TDYTH_NAAAS_4G_MME_LINKBREAK			0x09000012
#define D_NSP_TDYTH_NAAAS_4G_MME_LINKRECOVER		0x09000013
#define D_NSP_TDYTH_NAAAS_4G_MME_NETTOPOLOGY		0x09000014


#define D_NSP_TDYTH_NAAAS_4G_MAXEPOLLSIZE  		10000

/*************************************读取SIM卡***************************************/
#define D_NSP_TDYTH_NAAAS_4G_BUFF_LEN       	1024
#define D_NSP_TDYTH_NAAAS_4G_SIM_ISMILINE   	330
#define D_NSP_TDYTH_NAAAS_4G_SIM_KILINE     	335
#define D_NSP_TDYTH_NAAAS_4G_SIM_DATELINE     	355
#define D_NSP_TDYTH_NAAAS_4G_SIM_PHONELINE     	360
#define D_NSP_TDYTH_NAAAS_4G_SIM1_Location     	"./sim1.txt"
#define D_NSP_TDYTH_NAAAS_4G_SIM2_Location     	"./sim2.txt"
/*************************************字段长度(非特定ID)***************************************/
#define D_NSP_TDYTH_NAAAS_4G_RegisterInfo_LEN       	77
#define D_NSP_TDYTH_NAAAS_4G_IP_LEN       	16
#define D_NSP_TDYTH_NAAAS_4G_URL_LEN       	16
#define D_NSP_TDYTH_NAAAS_4G_LEGAL_TIME_INTERVAL 5000
#define D_NSP_TDYTH_NAAAS_4G_IP_Request_LEN 500
#define D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN  500

/*************************************IMS域相关定义***************************************/
//Diameter结构体头消息长度
#define D_NSP_TDYTH_NAAAS_4G_MessageLength_LEN 	3
#define D_NSP_TDYTH_NAAAS_4G_CommandCode_LEN	3
//SIP头消息长度
#define D_NSP_TDYTH_NAAAS_4GSIP_From_LEN        40
#define D_NSP_TDYTH_NAAAS_4GSIP_To_LEN          40  
#define D_NSP_TDYTH_NAAAS_4GSIP_Call_ID_LEN     40
#define D_NSP_TDYTH_NAAAS_4GSIP_CSeq_LEN        20
#define D_NSP_TDYTH_NAAAS_4GSIP_Via_LEN         40
//IMS数据包相关长度
#define  D_NSP_TDYTH_NAAAS_4G_END_LEN 			3
#define  D_NSP_TDYTH_NAAAS_4G_END 				"end"
#define  D_NSP_TDYTH_NAAAS_4G_aucRES_LEN        40

/*************************************小区选择相关定义***************************************/
#define D_NSP_TDYTH_NAAAS_4G_ENB1_ENBID         100001
#define D_NSP_TDYTH_NAAAS_4G_ENB2_ENBID         100002
#define D_NSP_TDYTH_NAAAS_4G_ENB3_ENBID         100003
#define D_NSP_TDYTH_NAAAS_4G_ENB1_PCI           501
#define D_NSP_TDYTH_NAAAS_4G_ENB2_PCI           502
#define D_NSP_TDYTH_NAAAS_4G_ENB3_PCI           66
#define D_NSP_TDYTH_NAAAS_4G_ENB1_EARFCN        38400
#define D_NSP_TDYTH_NAAAS_4G_ENB2_EARFCN        38400
#define D_NSP_TDYTH_NAAAS_4G_ENB3_EARFCN        38400
#define D_NSP_TDYTH_NAAAS_4G_ENB1_RSRP          -95
#define D_NSP_TDYTH_NAAAS_4G_ENB2_RSRP          -90
#define D_NSP_TDYTH_NAAAS_4G_ENB3_RSRP          -80
#define D_NSP_TDYTH_NAAAS_4G_ENB1_CellRP        5
#define D_NSP_TDYTH_NAAAS_4G_ENB2_CellRP        5
#define D_NSP_TDYTH_NAAAS_4G_ENB3_CellRP        5
#define D_NSP_TDYTH_NAAAS_4G_ENB1_Qrxlevmin     -128
#define D_NSP_TDYTH_NAAAS_4G_ENB2_Qrxlevmin     -128
#define D_NSP_TDYTH_NAAAS_4G_ENB3_Qrxlevmin     -128

//eNB广播包和UE小区重选的间隔时间定义，注意前者需要比后者小
#define D_NSP_TDYTH_NAAAS_4G_EnbBroadcast_Interval              5
#define D_NSP_TDYTH_NAAAS_4G_UECellReselection_Interval         6

/*************************************TAU相关定义***************************************/
#define D_NSP_TDYTH_NAAAS_4G_TAI_LEN            10
//假定3个ENB属于3个不同的TA，具有不同的TAI，后面可以根据需要进行更改
#define D_NSP_TDYTH_NAAAS_4G_ENB1_TAI           "4600060001"
#define D_NSP_TDYTH_NAAAS_4G_ENB2_TAI           "4600060002"
#define D_NSP_TDYTH_NAAAS_4G_ENB3_TAI           "4600060003"

/*************************************宏定义开关***************************************/
//#define	DATABASE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>
#include <sys/resource.h>
#include <pthread.h>
#include <assert.h>
#include <stdbool.h>
	
#ifdef __cplusplus
}
#endif

#endif //NSP_TDYTH_NAAAS_4G_COMMON_H
