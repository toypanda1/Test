/*
 * 文件功能：4G认证协议中相关算法函数
 * 作者：董珂 李泽宇 寇飞翔
 * 创建日期：2020/02/10
 * 修改日期：2020/08/20
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_fn.h"


static uint8 INOUT[160];
static const uint8	ALGONAME[] = "TUAK1.0";

void TUAK_ComputeTOPC(uint8*, uint8*);

uint8	TOP[32];
uint8	KEY_sz				= 16;
uint8	RES_sz				= 16;
uint8	CK_sz				= 16;
uint8	IK_sz				= 16;
uint8	AK_sz				= 6;
uint8	MAC_sz				= 16;
uint8	Iterations = sizeof(INOUT)%32 ? sizeof(INOUT)/32 +1 : sizeof(INOUT)/32;

void PUSH_DATA(const uint8 * data, uint8 n, uint8 location)
{
    while(n--)

        INOUT[location++] = data[n];

}

void PULL_DATA(uint8 * data, uint8 n, uint8 location)
{
    while(n--)
        data[n] = INOUT[location++];

}

void TUAK_Main	(uint8 instance, uint8 *rand, uint8 *amf, uint8 *sqn, uint8 *key)
{
    uint8 i, TOPC[32];
    TUAK_ComputeTOPC(key, TOPC);
    memset((uint8*)INOUT	, 0, 200);

    PUSH_DATA(TOPC		, 32,  0);
    PUSH_DATA(&instance  , 1 , 32);
    PUSH_DATA(ALGONAME   , 7 , 33);
    PUSH_DATA(rand       , 16, 40);
    if(amf) PUSH_DATA(amf, 2 , 56);
    if(sqn) PUSH_DATA(sqn, 6 , 58);

    PUSH_DATA(key, (instance & 1)?32:16, 64);

    TUAK_ADD_PADDING();

    int j = 0;
    for(i=0,j=0; i<Iterations; ++i,j+=32)
        SM3_256(&INOUT[i*10],120, &INOUT[j]);

}


void TUAK_ComputeTOPC(uint8 *key, uint8 *TOPC)
{
    uint8 i, inst = KEY_sz>>5;
    memset(INOUT, 0, 160);
    PUSH_DATA(TOP      , 32, 0 );
    PUSH_DATA(&inst    , 1 , 32);
    PUSH_DATA(ALGONAME , 7 , 33);
    PUSH_DATA(key  , KEY_sz, 64);
    TUAK_ADD_PADDING();

    int j = 0;
    for(i=0,j=0; i<Iterations; ++i,j+=32)
        SM3_256(&INOUT[i*10],120, &INOUT[j]);

    PULL_DATA(TOPC, 32, 0);
}


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
int NSP_TDYTH_naaa_sm3_hmac(unsigned char *pucK, unsigned char *pucMessage, unsigned short usMessageLen, unsigned char *pucSM3HMAC, unsigned short usSM3HMACLen)
{
    return NSP_TDYTH_SUCCESS;
}

/*
功能：	  随机数生成算法
参数：	  @pucRandom[out]		  输出随机数RAND
        @usRandomLen[in 2]      输入随机数RAND长度
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f0(unsigned char *pucRandom, unsigned short usRandomLen)
{
	return NSP_TDYTH_SUCCESS;
}

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
int NSP_TDYTH_naaa_f1(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucSQN, unsigned char *pucAMF, unsigned char *pucMACA)
{
    TUAK_Main( (KEY_sz>>5) | MAC_sz, pucRandom, pucAMF, pucSQN, pucK);
    PULL_DATA(pucMACA, MAC_sz, 0);
}

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
int NSP_TDYTH_naaa_f1x(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucSQN, unsigned char *pucAMF, unsigned char *pucMACA)
{
    TUAK_Main( (KEY_sz>>5) | MAC_sz | 0x80, pucRandom, pucAMF, pucSQN, pucK);
    PULL_DATA(pucMACA, MAC_sz, 0);
}

/*
功能：	  消息认证码算法, 计算RES
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucRES[out 16]			输出RES
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f2(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucRES)
{
    TUAK_Main( (KEY_sz>>5) | ((IK_sz>>4)&0x02) | ((CK_sz>>3)&0x04)
               | (RES_sz&0x38) | 0x40, pucRandom, 0, 0, pucK);
    PULL_DATA(pucRES, RES_sz, MAC_sz );
}

/*
功能：	  加密密钥生成算法, 计算CK
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucCK[out 16]			输出CK
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f3(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucCK)
{
    TUAK_Main( (KEY_sz>>5) | ((IK_sz>>4)&0x02) | ((CK_sz>>3)&0x04)
               | (RES_sz&0x38) | 0x40, pucRandom, 0, 0, pucK);
    PULL_DATA(pucCK , CK_sz , MAC_sz+RES_sz);
}

/*
功能：	  完整性密钥生成算法, 计算IK
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucIK[out 16]			输出IK
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f4(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucIK)
{
    TUAK_Main( (KEY_sz>>5) | ((IK_sz>>4)&0x02) | ((CK_sz>>3)&0x04)
               | (RES_sz&0x38) | 0x40, pucRandom, 0, 0, pucK);
    PULL_DATA(pucIK , IK_sz , MAC_sz+RES_sz+CK_sz);
}

/*
功能：	  掩盖密钥生成算法, 计算AK
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucAK[out 6]			输出AK
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f5(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucAK)
{
    TUAK_Main( (KEY_sz>>5) | ((IK_sz>>4)&0x02) | ((CK_sz>>3)&0x04)
               | (RES_sz&0x38) | 0x40, pucRandom, 0, 0, pucK);
    PULL_DATA(pucAK , AK_sz     , MAC_sz+RES_sz+CK_sz+IK_sz);
}

/*
功能：	重同步消息的掩盖密钥生成算法, 计算AK【文档中暂无协议格式说明，暂时以f5函数为准】
参数：	  @pucRandom[in 16]	      输入随机数RAND
        @pucK[in 16]			输入预置密钥K
		@pucAK[out 6]			输出AK
返回值： 0	成功
		非0	错误码
*/
int NSP_TDYTH_naaa_f5x(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucAK)
{
    TUAK_Main( (KEY_sz>>5) | 0xc0, pucRandom, 0, 0, pucK);
    PULL_DATA(pucAK, AK_sz, MAC_sz+RES_sz+CK_sz+IK_sz);
}
