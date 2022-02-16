/*
 * 文件功能：4G接入认证协议计算相关文件
 * 作者：董珂
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#ifndef NSP_TDYTH_NAAAS_4G_GEN_H
#define NSP_TDYTH_NAAAS_4G_GEN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "NSP_TDYTH_NAAAS_fn.h"
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>
#include <time.h>
#define STEP 1

typedef unsigned char u8;
/*************************************************
* 函数名：                    NSP_TDYTH_NAAAS_KDF
* 函数功能描述：               生成指定长度的密钥
* 函数参数：
* @param  		[in] md 	  指定需要使用的HMAC算法
* @param  		[in] outLen	  指定要输出的密钥长度
* @param  		[out]out      输出缓冲区
* @param  		[in] inKeyLen 输入密钥长度
* @param  		[in] inKey    输入密钥
* @param  		[in] inputNum 输入参数个数
* @param  		[in] ...      输入参数列表
* @return      0             成功
*              -1            失败
* 备注: 调用示例
* NSP_TDYTH_NAAAS_KDF(EVP_sha256(), 32, buf, sizeof(key), key, 3, 5, "P0abc", 3, "P1.", 6, "p2....");
**************************************************/
int NSP_TDYTH_NAAAS_KDF(const EVP_MD *md, int outLen, u8 *out, int inKeyLen, u8 *inKey, int inputNum, ...);

/*
 * 功能：计算随机数RAND（f0）
 * 参数：	  @pucRandom[out 32]        输出随机数RAND
 *            @usRandomLen[in 2]        输入随机数RAND长度
 * 返回值：	0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_gen_RAND(unsigned char *pucRandom, unsigned short usRandomLen);

/*
 * 功能：计算序列号SQN
 * 参数：	  @pucSQN[out 6]	    输出序列号SQN
 *            @usSQNLen[in 2]       输入序列号SQN长度
 * 返回值：	0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_gen_SQN(unsigned char *pucSQN, int baseTime, unsigned short usSQNLen);

/*
 * 功能：验证序列号SQN
 * 参数：	  @pucSQN[in 6]         输入序列号SQN
 * 返回值：	0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_verify_SQN(unsigned char *pucSQN);

/*
 * 功能：计算消息验证码MAC（f1）
 * 参数：    @pucRandom [in 32]	随机数
 *          @pucK [in 32]	    预置密钥
 *          @pucSQN [in 6]	    序列号信息
 *          @pucAMF [in 2]      AMF标识
 *          @pucMAC [out 16]	返回生成的消息验证码MAC
 * 返回值：	0   成功
 *          非0 错误码
 * 备注：根据预置密钥、序列号信息、随机数生成响应的MAC，调用内部接口f1。（XMAC同样调用此接口）
 */
int NSP_TDYTH_4G_gen_MAC(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucSQN, unsigned char *pucAMF, unsigned char *pucMAC);

/*
 * 功能：计算RES（f2）
 * 参数：	@pucK [in 32]	    预置密钥
 *          @pucRandom [in 32]	随机数
 *          @pucRES [out 16]    返回生成的RES
 * 返回值：	 0  成功
 *          非0 错误码
 * 备注：根据预置密钥、随机数生成响应的RES，调用内部接口f2。（XRES同样调用此接口）
 */
int NSP_TDYTH_4G_gen_RES(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucRES);

/*
 * 功能：计算CK（f3）
 * 参数：	@pucK [in 32]	    预置密钥
 *          @pucRandom [in 32]	随机数
 *          @pucCK [out 16]	    返回生成的CK
 * 返回值：	 0  成功
 *          非0 错误码
 * 备注：根据预置密钥、随机数生成响应的CK，调用内部接口f3。
 */
int NSP_TDYTH_4G_gen_CK(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucCK);

/*
 * 功能：计算IK（f4）
 * 参数：	@pucK [in 32]	    预置密钥
 *          @pucRandom [in 32]	随机数
 *          @pucIK [out 16]	    返回生成的IK
 * 返回值：	 0  成功
 *          非0 错误码
 * 备注：根据预置密钥、随机数生成响应的IK，调用内部接口f4。
 */
int NSP_TDYTH_4G_gen_IK(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucIK);

/*
 * 功能：计算AK（f5）
 * 参数：	@pucK [in 32]	    预置密钥
 *          @pucRandom [in 32]	随机数
 *          @pucAK [out 6]      返回生成的AK
 * 返回值：	 0  成功
 *          非0 错误码
 * 备注：根据预置密钥、随机数生成响应的AK，调用内部接口f5。
 */
int NSP_TDYTH_4G_gen_AK(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucAK);

/*
 * 功能：计算AUTN
 * 参数：	@pucSQN [in 6]      序列号
 *          @pucAK [in 6]	    序列号
 *          @pucAMF [in 2]      AMF标识
 *          @pucMAC [in 16]     MAC
 *          @pucAUTN [out 24]	AUTN
 * 返回值：	0   成功
 *          非0 错误码
 * 备注：根据预置密钥、序列号及MAC信息生成认证的AUTN。
 */
int NSP_TDYTH_4G_gen_AUTN(unsigned char *pucSQN, unsigned char *pucAK, unsigned char *pucAMF, unsigned char *pucMAC, unsigned char *pucAUTN);

/*
 * 功能：计算Kamse
 * 参数：	@pucSNID [in 49bits]	服务网络id
 *          @pucCK [in 16] 	        CK
 *          @pucAK [in 6] 	        AK
 *          @pucKamse [out 256]	    AUTN
 * 返回值：	0   成功
 *          非0 错误码
 * 备注：根据预置密钥、序列号及MAC信息生成认证的AUTN。
 */
int NSP_TDYTH_4G_gen_Kamse(unsigned char *pucSNID, unsigned char *pucCK, unsigned char *pucIK, unsigned char *pucSQN, unsigned char *pucAK ,unsigned char *pucKasme);


/*
 * 功能：计算Kenb1
 * 参数：  
 *          @pucNH1     [in 32]      NH
 *          @pucPCI     [in 2]       uiPCI
 *          @pucEARFCN  [in 2]       uiEARFCN
 *          @pucKenb1   [out 32]     Kenb1
 * 返回值：   0   成功
 *          非0 错误码
 * 备注：根据NH、uiPCI、EARFCN等生成的Kenb1。
 */
int NSP_TDYTH_4G_gen_Kenb(unsigned char *pucNH1, unsigned int *pucPCI, unsigned int *pucEARFCN, unsigned char *pucKenb1);

/*
 * 功能：计算Kenb3
 * 参数：  
 *          @pucKenb2   [in 32]      Kenb2
 *          @pucPCI     [in 2]       uiPCI
 *          @pucEARFCN  [in 2]       uiEARFCN
 *          @pucKenb3   [out 32]     Kenb3
 * 返回值：   0   成功
 *          非0 错误码
 * 备注：根据Kenb2、uiPCI、EARFCN等生成的Kenb3。
 */
int NSP_TDYTH_4G_gen_Kenb3(unsigned char *pucKenb2, unsigned int *pucPCI, unsigned int *pucEARFCN, unsigned char *pucKenb3);

/*
 * 功能：计算Kenb2
 * 参数：  
 *          @pucKasme   [in 32]      Kasme
 *          @pucKenb1   [in 32]      K*enb
 *          @pucKenb2   [out 32]     K*+enb
 * 返回值：   0   成功
 *          非0 错误码
 * 备注：根据Kasme、K*enb等生成的Kenb1。
 */
int NSP_TDYTH_4G_gen_Kenb2(unsigned char *pucKasme, unsigned char *pucKenb1, unsigned char *pucKenb2);
/*
 * 功能：计算NH*ncc
 * 参数：
 *          @pucKasme   [in 32]      Kasme
 *          @pucKenb2   [in 32]      K*+enb
 *          @pucNH2     [out 32]     NH*ncc
 * 返回值：   0   成功
 *          非0 错误码
 * 备注：根据Kasme、K*+enb等生成的NH*ncc。
 */
int NSP_TDYTH_4G_gen_NH1(unsigned char *pucKasme, unsigned char *pucKenb, unsigned char *pucNH1);
/*
 * 功能：计算NH*ncc
 * 参数：  
 *          @pucKasme   [in 32]      Kasme
 *          @pucKenb2   [in 32]      K*+enb
 *          @pucNH2     [out 32]     NH*ncc
 * 返回值：   0   成功
 *          非0 错误码
 * 备注：根据Kasme、K*+enb等生成的NH*ncc。
 */
int NSP_TDYTH_4G_gen_S1_NH2(unsigned char *pucKasme, unsigned char *pucKenb2, unsigned char *pucNH2);
/*
 * 功能：计算NH*ncc
 * 参数：
 *          @pucKasme   [in 32]      Kasme
 *          @pucpucNH1   [in 32]     NHNCC
 *          @pucNH2     [out 32]     NH*ncc
 * 返回值：   0   成功
 *          非0 错误码
 * 备注：根据Kasme、NHncc等生成的NH*ncc。
 */
int NSP_TDYTH_4G_gen_X2_NH2(unsigned char *pucKasme, unsigned char *pucNH1, unsigned char *pucNH2);
/*
 * 功能：计算Kupenc
 * 参数：  
 *          @pucKenb3       [in 32]      Kenb**
 *          @pucKupenc      [out 32]     Kupenc
 * 返回值： 0   成功
 *          非0 错误码
 * 备注：根据Kenb**生成的Kupenc。
 */

int NSP_TDYTH_4G_gen_Kupenc(unsigned char *pucKenb3, unsigned char *pucKupenc);

/*
 * 功能：计算Krrcint
 * 参数：  
 *          @pucKenb3        [in 32]      Kenb**
 *          @pucKrrcint      [out 32]     Krrcint
 * 返回值： 0   成功
 *          非0 错误码
 * 备注：根据Kenb**生成的Krrcint。
 */  
int NSP_TDYTH_4G_gen_Krrcint(unsigned char *pucKenb3, unsigned char *pucKrrcint);

/*
 * 功能：计算Krrcenc
 * 参数：  
 *          @pucKenb3        [in 32]      Kenb**
 *          @pucKrrcenc      [out 32]     Krrcenc
 * 返回值： 0   成功
 *          非0 错误码
 * 备注：根据Kenb**生成的Krrcenc。
 */  
int NSP_TDYTH_4G_gen_Krrcenc(unsigned char *pucKenb3, unsigned char *pucKrrcenc);

#ifdef __cplusplus
}
#endif

#endif //NSP_TDYTH_NAAAS_4G_GEN_H
