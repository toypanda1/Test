/*
 * 文件功能：4G接入认证协议计算相关文件
 * 作者：董珂
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_gen.h"
#include "NSP_TDYTH_NAAAS_sha256.h"
#include "NSP_TDYTH_NAAAS_fn.h"
#include "NSP_TDYTH_NAAAS_4G_utils.h"



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
int NSP_TDYTH_NAAAS_KDF(const EVP_MD *md, int outLen, u8 *out, int inKeyLen, u8 *inKey, int inputNum, ...)
{
//    unsigned char buf[32];
//    unsigned char key[] = "this is a tmp key.";
//    NSP_TDYTH_NAAAS_KDF(EVP_sha256(), 32, buf, sizeof(key), key, 3, 5, "P0...", 5, "P1...", 5, "p2...");
//    HEXPrint("out", buf, 32);
//    return 0;
    unsigned char buf[1024] = {0};
    unsigned char *p = buf;
    int inputLen = 0;
    //填入FC
    buf[0] = 0x6B;
    p += 1;
    inputLen += 1;

    //依次填入P0||L0||P1||L1...
    unsigned short Li;
    va_list var_arg;
    int count;
    va_start(var_arg, inputNum);
    for(count = 0; count < inputNum; ++count){
        Li = (unsigned short)va_arg(var_arg, int);
        memcpy(p, (unsigned char *)&Li, sizeof(unsigned short));
        p += 2;
        memcpy(p, va_arg(var_arg, unsigned char *), Li);
        p += Li;
        inputLen += Li + sizeof(unsigned short);
    }
    va_end(var_arg);

    //调用HMAC-SHA256
    unsigned int len = 1024;
    HMAC_CTX *ctx = HMAC_CTX_new();
    HMAC_Init_ex(ctx, inKey, inKeyLen, EVP_sha256(), NULL);
    HMAC_Update(ctx, buf, inputLen);
    HMAC_Final(ctx, out, &len);
    HMAC_CTX_free(ctx);
    return (len != outLen) ? -1 : 0;
}
#define NSP_TDYTH_NAAAS_KDF_SM3_256(out, inKeyLen, inKey, inputNum, ...) \
    NSP_TDYTH_NAAAS_KDF(EVP_sm3(), 32, out, inKeyLen, inKey, inputNum, ## __VA_ARGS__);




/*
 * 功能：计算随机数RAND（f0）
 * 参数：	  @pucRandom[out 32]        输出随机数RAND
 *            @usRandomLen[in 2]        输入随机数RAND长度
 * 返回值：	0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_gen_RAND(unsigned char *pucRandom, unsigned short usRandomLen)
{
    if(pucRandom == NULL || usRandomLen <= 0)
    {
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }

    return NSP_TDYTH_naaa_f0(pucRandom, usRandomLen);
    /*struct timeval tpstart;
    gettimeofday(&tpstart, NULL);
    srand(tpstart.tv_usec);
    int i;
    for ( i = 0;i < usRandomLen; i++)
    {
        pucRandom[i] = rand()%16;
        pucRandom[i]<10 ? (pucRandom[i] = pucRandom[i]+ '0'):(pucRandom[i] = pucRandom[i] + 0x37);
    }
    return 0;*/
}

/*
 * 功能：计算序列号SQN
 * 参数：	  @pucSQN[out 6]	    输出序列号SQN
 *            @usSQNLen[in 2]       输入序列号SQN长度
 *            @baseTime
 * 返回值：	0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_gen_SQN(unsigned char *pucSQN, int baseTime, unsigned short usSQNLen)
{

    if(pucSQN == NULL || usSQNLen <= 0)
    {
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    unsigned char seq[6];
    unsigned char ind;
    time_t timer;

    //memset(pucSQN, 0xff, usSQNLen);
    seq[5] = pucSQN[5];
    seq[4] = pucSQN[4];
    seq[3] = pucSQN[3];
    seq[2] = pucSQN[2];
    seq[1] = pucSQN[1];
    seq[0] = pucSQN[0] & 0xe0;

    ind = pucSQN[0] & 0x1f;
    *(long long *) (&seq) = *(long long *) (&seq) >> 5;

    switch(baseTime)
    {
        case 0:
            *(long long *) (&seq) += STEP;
            ind = ind > 15 ? ((ind + 1) % 16) + 16 : (ind + 1) % 16;
            *(long long *) (&seq) = *(long long *) (&seq) << 5;
            pucSQN[5] = seq[5];
            pucSQN[4] = seq[4];
            pucSQN[3] = seq[3];
            pucSQN[2] = seq[2];
            pucSQN[1] = seq[1];
            pucSQN[0] = seq[0] | ind;
            break;
        case 1:
            time(&timer);
            *(long long *) (&seq)+=timer;
            ind = ind > 15 ? ((ind + 1) % 16) + 16 : (ind + 1) % 16;
            *(long long *) (&seq) = *(long long *) (&seq) << 5;
            pucSQN[5] = seq[5];
            pucSQN[4] = seq[4];
            pucSQN[3] = seq[3];
            pucSQN[2] = seq[2];
            pucSQN[1] = seq[1];
            pucSQN[0] = seq[0] + ind;
            break;
    }

    //按照标准SQN范围方式进行计算
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：验证序列号SQN
 * 参数：	  @pucSQN[in 6]         输入序列号SQN
 * 返回值：	0   成功
 *          非0 错误码
 */
//todo
int NSP_TDYTH_4G_verify_SQN(unsigned char *pucSQN)
{
   /* if(pucSQN == NULL)
    {
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    //按照标准校验SQN范围方式进行验证
    int iRet;
    unsigned char seq[6];
    unsigned char ind;
    unsigned char tmp[16][6] = {0};
    unsigned char sqn_tmp[6];

    ind = (pucSQN[0] & 0x1f) % 16;
    long long *p1=(long long*)sqn_tmp;
    *p1=*(long long *) (pucSQN);
    long long *p=(long long*)tmp[1];
    //*p=0x01aaffffaaff;
    seq[5] = sqn_tmp[5];
    seq[4] = sqn_tmp[4];
    seq[3] = sqn_tmp[3];
    seq[2] = sqn_tmp[2];
    seq[1] = sqn_tmp[1];
    seq[0] = sqn_tmp[0] & 0xe0;
    *(long long *) (seq) = *(long long *) (seq) >> 5;
    if(*(long long *) (seq)>*(long long*)tmp[1])
    {
        printf("sqn_success\n");
        *p=*(long long *) (&seq);
        printf("tmp[ind]更新为%llx\n",*(long long*)tmp[ind]);
        iRet =  0;
    }
    else{
        printf("sqn_error");
        iRet =  1;
    }
    //如果SQN验证失败，返回错误码
    if(iRet)
    {
        return D_NSP_TDYTH_NAAAS_4G_PROTO_SQN_ERR;
    }*/
    //暂时动不了，得等三组的数据库统一之后再进行通过ind去查数据库，然后得到一个数组再比较；
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

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
int NSP_TDYTH_4G_gen_MAC(unsigned char *pucRandom, unsigned char *pucK, unsigned char *pucSQN, unsigned char *pucAMF, unsigned char *pucMAC)
{
    if(pucRandom == NULL || pucK == NULL || pucSQN == NULL || pucAMF == NULL || pucMAC == NULL)
    {
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    return NSP_TDYTH_naaa_f1(pucRandom, pucK, pucSQN, pucAMF, pucMAC);
}

/*
 * 功能：计算RES（f2）
 * 参数：	@pucK [in 32]	    预置密钥
 *          @pucRandom [in 32]	随机数
 *          @pucRES [out 16]    返回生成的RES
 * 返回值：	 0  成功
 *          非0 错误码
 * 备注：根据预置密钥、随机数生成响应的RES，调用内部接口f2。（XRES同样调用此接口）
 */
int NSP_TDYTH_4G_gen_RES(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucRES)
{
    if(pucK == NULL || pucRandom == NULL || pucRES == NULL)
    {
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    return NSP_TDYTH_naaa_f2(pucK, pucRandom,  pucRES);
}
/*
 * 功能：计算CK（f3）
 * 参数：	@pucK [in 32]	    预置密钥
 *          @pucRandom [in 32]	随机数
 *          @pucCK [out 16]	    返回生成的CK
 * 返回值：	 0  成功
 *          非0 错误码
 * 备注：根据预置密钥、随机数生成响应的CK，调用内部接口f3。
 */
int NSP_TDYTH_4G_gen_CK(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucCK)
{
    if(pucK == NULL || pucRandom == NULL || pucCK == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    return NSP_TDYTH_naaa_f3(pucK, pucRandom, pucCK);
}

/*
 * 功能：计算IK（f4）
 * 参数：	@pucK [in 32]	    预置密钥
 *          @pucRandom [in 32]	随机数
 *          @pucIK [out 16]	    返回生成的IK
 * 返回值：	 0  成功
 *          非0 错误码
 * 备注：根据预置密钥、随机数生成响应的IK，调用内部接口f4。
 */
int NSP_TDYTH_4G_gen_IK(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucIK)
{
    if(pucK == NULL || pucRandom == NULL || pucIK == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    return NSP_TDYTH_naaa_f4(pucK, pucRandom, pucIK);
}

/*
 * 功能：计算AK（f5）
 * 参数：	@pucK [in 32]	    预置密钥
 *          @pucRandom [in 32]	随机数
 *          @pucAK [out 6]      返回生成的AK
 * 返回值：	 0  成功
 *          非0 错误码
 * 备注：根据预置密钥、随机数生成响应的AK，调用内部接口f5。
 */
int NSP_TDYTH_4G_gen_AK(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucAK)
{
    if(pucK == NULL || pucRandom == NULL || pucAK == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    return NSP_TDYTH_naaa_f5(pucK, pucRandom, pucAK);
}

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
int NSP_TDYTH_4G_gen_AUTN(unsigned char *pucSQN, unsigned char *pucAK, unsigned char *pucAMF, unsigned char *pucMAC, unsigned char *pucAUTN)
{
    if(pucSQN == NULL || pucAK == NULL || pucAMF == NULL || pucMAC == NULL || pucAUTN == NULL)
    {
        return  D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    //进行AUTN生成
    unsigned char pucSqnXorAki[D_NSP_TDYTH_NAAAS_4G_SQN_LEN];
    NSP_TDYTH_4G_XOR(pucSQN, pucAK, D_NSP_TDYTH_NAAAS_4G_SQN_LEN, pucSqnXorAki);
    memcpy(pucAUTN, pucSqnXorAki, D_NSP_TDYTH_NAAAS_4G_SQN_LEN);
    pucAUTN += D_NSP_TDYTH_NAAAS_4G_SQN_LEN;
    memcpy(pucAUTN, pucAMF, D_NSP_TDYTH_NAAAS_4G_AMF_LEN);
    pucAUTN += D_NSP_TDYTH_NAAAS_4G_AMF_LEN;
    memcpy(pucAUTN, pucMAC, D_NSP_TDYTH_NAAAS_4G_MAC_LEN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

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
int NSP_TDYTH_4G_gen_Kamse(unsigned char *pucSNID, unsigned char *pucCK, unsigned char *pucIK, unsigned char *pucSQN, unsigned char *pucAK ,unsigned char *pucKasme)
{
    if(pucSNID == NULL || pucCK == NULL || pucAK == NULL || pucKasme == NULL)
    {
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    //使用标准解释，Kamse密钥生成算法计算

    char FC=0x10;
    int i,Slen;
    int LenOut=32;
    unsigned short L0;
    unsigned short L1;

    unsigned char SQN[D_NSP_TDYTH_NAAAS_4G_SQN_LEN];
    unsigned char AK[D_NSP_TDYTH_NAAAS_4G_AK_LEN];
    unsigned char Key[D_NSP_TDYTH_NAAAS_4G_KASME_LEN];
    unsigned char S[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    unsigned char SNID[3];

    memset(Key, 0, sizeof(Key));
    memset(S, 0, sizeof(S));
    memset(SQN, 0, sizeof(SQN));
    memset(AK, 0, sizeof(AK));
    memcpy(SQN,pucSQN,6);
    memcpy(AK,pucAK,6);
    memcpy(SNID,pucSNID,3);


    //Key=IK || CK
    memcpy(Key,pucIK,D_NSP_TDYTH_NAAAS_4G_IK_LEN);
    memcpy(Key+D_NSP_TDYTH_NAAAS_4G_IK_LEN,pucCK,D_NSP_TDYTH_NAAAS_4G_CK_LEN);

    //S=FC∥P0∥L0∥P1∥L1
    memcpy(S,&FC,sizeof(FC));

    L0=sizeof(SNID);  //L0为P0的长度，规定为两字节，此处其实为0x00,0x03

    memcpy(S+sizeof(FC),SNID,L0);
    L0=BLEndianUshort(L0);
    memcpy(S+sizeof(FC)+sizeof(SNID),&L0,sizeof(L0));

    for(i=0;i<NSP_TDYTH_4G_Kasme_S_LEN;i++)
    {
        SQN[i]^=AK[i];
    }
    L1=sizeof(SQN);//L0为P0的长度，规定为两字节，此处其实为0x00,0x06

    memcpy(S+sizeof(FC)+sizeof(L0)+sizeof(SNID),SQN,L1);
    L1=BLEndianUshort(L1);
    memcpy(S+sizeof(FC)+sizeof(L0)+sizeof(SNID)+sizeof(SQN),&L1,sizeof(L1));

    hmac_sha256(Key, D_NSP_TDYTH_NAAAS_4G_KASME_LEN, S, NSP_TDYTH_4G_Kasme_S_LEN, pucKasme, &LenOut);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}




/***************************************************************eNB S1切换*******************************************************************************/
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

int NSP_TDYTH_4G_gen_Kenb(unsigned char *pucNH1, unsigned int *pucPCI, unsigned int *pucEARFCN, unsigned char *pucKenb1)
{
    if(pucNH1 == NULL || pucPCI == NULL || pucEARFCN == NULL || pucKenb1 == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    NSP_TDYTH_NAAAS_KDF_SM3_256(pucKenb1,
                                D_NSP_TDYTH_NAAAS_4G_NH_LEN,pucNH1, 2,\
                                sizeof(int), &pucPCI,\
                                sizeof(int), &pucKenb1);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
int NSP_TDYTH_4G_S1_gen_Kenb(unsigned char *pucKenb, unsigned int *pucPCI, unsigned int *pucEARFCN, unsigned char *pucKenb1)
{
    if(pucKenb == NULL || pucPCI == NULL || pucEARFCN == NULL )
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    NSP_TDYTH_NAAAS_KDF_SM3_256(pucKenb1,
                                D_NSP_TDYTH_NAAAS_4G_KENB_LEN,pucKenb, 2,\
                                sizeof(int), &pucPCI,\
                                sizeof(int), &pucKenb1);


    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
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
int NSP_TDYTH_4G_gen_Kenb3(unsigned char *pucKenb2, unsigned int *pucPCI, unsigned int *pucEARFCN, unsigned char *pucKenb3)
{
    if(pucKenb2 == NULL || pucPCI == NULL || pucEARFCN == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    NSP_TDYTH_NAAAS_KDF_SM3_256(pucKenb3,D_NSP_TDYTH_NAAAS_4G_KENB_LEN,pucKenb2, 2,\
                                sizeof(int), pucPCI,\
                                sizeof(int), pucEARFCN);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}


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
int NSP_TDYTH_4G_gen_Kenb2(unsigned char *pucKasme, unsigned char *pucKenb1, unsigned char *pucKenb2)
{
    if(pucKasme == NULL || pucKenb1 == NULL || pucKenb2 == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    NSP_TDYTH_NAAAS_KDF_SM3_256(pucKenb2,D_NSP_TDYTH_NAAAS_4G_KASME_LEN,pucKasme, 1,\
                                D_NSP_TDYTH_NAAAS_4G_KENB_LEN, pucKenb1);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
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
int NSP_TDYTH_4G_gen_NH1(unsigned char *pucKasme, unsigned char *pucKenb, unsigned char *pucNH1)
{
    if(pucKasme == NULL || pucKenb == NULL || pucNH1 == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    NSP_TDYTH_NAAAS_KDF_SM3_256(pucNH1,D_NSP_TDYTH_NAAAS_4G_KASME_LEN,pucKasme, 1,\
                                D_NSP_TDYTH_NAAAS_4G_KENB_LEN, pucKenb);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
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
int NSP_TDYTH_4G_gen_S1_NH2(unsigned char *pucKasme, unsigned char *pucKenb2, unsigned char *pucNH2)
{
    if(pucKasme == NULL || pucKenb2 == NULL || pucNH2 == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    NSP_TDYTH_NAAAS_KDF_SM3_256(pucNH2,D_NSP_TDYTH_NAAAS_4G_KASME_LEN,pucKasme, 1,\
                                D_NSP_TDYTH_NAAAS_4G_KENB_LEN, pucKenb2);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

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
int NSP_TDYTH_4G_gen_X2_NH2(unsigned char *pucKasme, unsigned char *pucNH1, unsigned char *pucNH2)
{
    if(pucKasme == NULL || pucNH1== NULL || pucNH2 == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    NSP_TDYTH_NAAAS_KDF_SM3_256(pucNH2,D_NSP_TDYTH_NAAAS_4G_KASME_LEN,pucKasme, 1,\
                                D_NSP_TDYTH_NAAAS_4G_NH_LEN,pucNH1);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/*
 * 功能：计算Kupenc
 * 参数：  
 *          @pucKenb3       [in 32]      Kenb**
 *          @pucKupenc      [out 32]     Kupenc
 * 返回值： 0   成功
 *          非0 错误码
 * 备注：根据Kenb**生成的Kupenc。
 */  
int NSP_TDYTH_4G_gen_Kupenc(unsigned char *pucKenb3, unsigned char *pucKupenc)
{
    if(pucKenb3 == NULL || pucKupenc == NULL )
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    //进行Kupenc生成

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}    

/*
 * 功能：计算Krrcint
 * 参数：  
 *          @pucKenb3        [in 32]      Kenb**
 *          @pucKrrcint      [out 32]     Krrcint
 * 返回值： 0   成功
 *          非0 错误码
 * 备注：根据Kenb**生成的Krrcint。
 */  
int NSP_TDYTH_4G_gen_Krrcint(unsigned char *pucKenb3, unsigned char *pucKrrcint)
{
    if(pucKenb3 == NULL || pucKrrcint == NULL )
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    //进行Krrcint生成

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}    
     
/*
 * 功能：计算Krrcenc
 * 参数：  
 *          @pucKenb3        [in 32]      Kenb**
 *          @pucKrrcenc      [out 32]     Krrcenc
 * 返回值： 0   成功
 *          非0 错误码
 * 备注：根据Kenb**生成的Krrcenc。
 */  
int NSP_TDYTH_4G_gen_Krrcenc(unsigned char *pucKenb3, unsigned char *pucKrrcenc)
{
    if(pucKenb3 == NULL || pucKrrcenc == NULL )
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    //进行Krrcenc生成

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}