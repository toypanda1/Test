/*
 * 文件功能：4G认证协议中相关算法函数
 * 作者：董珂 
 * 创建日期：2020/02/10
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#ifndef NSP_TDYTH_NAAAS_FN_H
#define NSP_TDYTH_NAAAS_FN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "NSP_TDYTH_NAAAS_sm3.h"
#include "NSP_TDYTH_NAAAS_4G_common.h"


#define TUAK_ADD_PADDING()		INOUT[96] = 0x1F, INOUT[135] = 0x80

typedef unsigned char			uint8;
typedef unsigned long			uint32;
typedef unsigned long long	   uint64;


#define NSP_TDYTH_KEY_BITS              128
//#define NSP_TDYTH_DIGEST_BITS           256
#define NSP_TDYTH_RAND_BITS             128
#define NSP_TDYTH_SQN_BITS              48
#define NSP_TDYTH_AMF_BITS              16
#define NSP_TDYTH_SUPI_BITS             64
#define NSP_TDYTH_MACA_BITS             128
#define NSP_TDYTH_RES_BITS              128
#define NSP_TDYTH_CK_BITS               128
#define NSP_TDYTH_IK_BITS               128
#define NSP_TDYTH_AK_BITS               48
#define NSP_TDYTH_KEY_LEN               ((NSP_TDYTH_KEY_BITS + 7) / 8)
#define NSP_TDYTH_DIGEST_LEN            ((NSP_TDYTH_DIGEST_BITS + 7) / 8)
#define NSP_TDYTH_RAND_LEN              ((NSP_TDYTH_RAND_BITS + 7) / 8)
#define NSP_TDYTH_SQN_LEN               ((NSP_TDYTH_SQN_BITS + 7) / 8)
#define NSP_TDYTH_AMF_LEN               ((NSP_TDYTH_AMF_BITS + 7) / 8)
#define NSP_TDYTH_SUPI_LEN              ((NSP_TDYTH_SUPI_BITS + 7) / 8)
#define NSP_TDYTH_MACA_LEN              ((NSP_TDYTH_MACA_BITS + 7) / 8)
#define NSP_TDYTH_RES_LEN               ((NSP_TDYTH_RES_BITS + 7) / 8)
#define NSP_TDYTH_CK_LEN                ((NSP_TDYTH_CK_BITS + 7) / 8)
#define NSP_TDYTH_IK_LEN                ((NSP_TDYTH_IK_BITS + 7) / 8)
#define NSP_TDYTH_AK_LEN                ((NSP_TDYTH_AK_BITS + 7) / 8)

//错误码
#define NSP_TDYTH_SUCCESS               0x0
#define NSP_TDYTH_INDATA_ERR            0x00000001
#define NSP_TDYTH_MALLOC_ERR            0x00000002
#define NSP_TDYTH_SM3HMAC_ERR           0x00000003

/*
功能：	  以杂凑算法SM3构造HMAC
参数：	  @pucK[in 16]            输入密钥K
        @pucMessage[in]	        输入消息S
        @usMessageLen[in]       输入消息长度
        @pucSM3HMAC[out 16]     输出SM3_HMAC值
        @usSM3HMACLen[in 2]     输入SM3_HMAC值长度
返回值:  0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_sm3_hmac(unsigned char *pucK, unsigned char *pucMessage, unsigned short usMessageLen, unsigned char *pucSM3HMAC, unsigned short usSM3HMACLen);

/*
功能：	  随机数生成算法
参数：	  @pucRandom[out]		  输出随机数RAND
        @usRandomLen[in 2]      输入随机数RAND长度
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f0(unsigned char *pucRandom, unsigned short usRandomLen);

/*
功能：	 网络认证函数，计算MAC-A
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucSQN[in 6]			输入序列号SQN
		@pucAMF[in 2]			输入AMF
		@pucMACA[out 16]	    输出MAC-A
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f1(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucSQN, unsigned char *pucAMF, unsigned char *pucMACA);

/*
功能：	重同步的消息认证算法
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucSQN[in 6]			输入序列号SQN
		@pucAMF[in 2]			输入AMF
		@pucMACA[out 16]	    输出MAC-A
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f1x(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucSQN, unsigned char *pucAMF, unsigned char *pucMACA);

/*
功能：	  消息认证码算法, 计算RES
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucRES[out 16]			输出RES
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f2(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucRES);

/*
功能：	  加密密钥生成算法, 计算CK
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucCK[out 16]			输出CK
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f3(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucCK);

/*
功能：	  完整性密钥生成算法, 计算IK
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucIK[out 16]			输出IK
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f4(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucIK);

/*
功能：	  掩盖密钥生成算法, 计算AK
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucAK[out 6]			输出AK
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f5(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucAK);

/*
功能：	重同步消息的掩盖密钥生成算法, 计算AK【文档中暂无协议格式说明，暂时以f5函数为准】
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucAK[out 6]			输出AK
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f5x(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucAK);

#ifdef __cplusplus
}
#endif

#endif  //NSP_TDYTH_NAAAS_FN_H
