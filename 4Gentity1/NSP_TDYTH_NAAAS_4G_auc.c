//
// Created by lgm on 2020/6/9.
//

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_anet.h"
#include "NSP_TDYTH_NAAAS_4G_gen.h"
#include "NSP_TDYTH_NAAAS_4G_utils.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"
#include "NSP_TDYTH_NAAAS_4G_debug.h"


/*
 * 功能：AUC端密钥生成
 * 参数：   
 *         @pucK      [in]      主密钥K
 *         @pucRandom [in]     随机数
 *         @pucIK     [out]    IK完整性密钥
 *         @pucCK     [out]    CK加密性密钥
 *         @pucAK     [out]    AK
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int NSP_TDYTH_4G_Key_Generate(unsigned char *pucK, unsigned char *pucRandom, unsigned char *pucIK, unsigned char *pucCK, unsigned char *pucAK)
{
    NSP_TDYTH_4G_gen_CK(pucK, pucRandom, pucCK);
    NSP_TDYTH_4G_gen_IK(pucK, pucRandom, pucIK);
    NSP_TDYTH_4G_gen_AK(pucK, pucRandom, pucAK);

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：AUC端密钥管理
 * 参数：   
 *         @pucIK     [in]    IK完整性密钥
 *         @pucCK     [in]    CK加密性密钥
 *         @pucAK     [in]    AK
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int NSP_TDYTH_4G_Key_Management(unsigned char *pucIK, unsigned char *pucCK, unsigned char *pucAK)
{
    if(pucIK == NULL || pucCK == NULL || pucAK == NULL)
    {
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能：AUC端数据处理流程
 * 参数：	@pcRecvBuf [in]	 接收缓冲区
 *          @puSendBuf [out] 发送缓冲区
 * 返回值：
 */
int NSP_TDYTH_4G_accessAuth_AUCProcess(char *pcRecvBuf, char *pcSendBuf)
{
    int iSockfd;
    int iRet;

    printf("进入AUC处理流程!\n");


    unsigned char aucIMSI[D_NSP_TDYTH_NAAAS_4G_IMSI_LEN];
    unsigned char aucK[D_NSP_TDYTH_NAAAS_4G_KEY_LEN];
    unsigned char aucSQN[D_NSP_TDYTH_NAAAS_4G_SQN_LEN];
    unsigned char aucRAND[D_NSP_TDYTH_NAAAS_4G_RAND_LEN];
    unsigned char aucAMF[D_NSP_TDYTH_NAAAS_4G_AMF_LEN];
    unsigned char aucMAC[D_NSP_TDYTH_NAAAS_4G_MAC_LEN];
    unsigned char aucXRES[D_NSP_TDYTH_NAAAS_4G_RES_LEN];
    unsigned char aucCK[D_NSP_TDYTH_NAAAS_4G_CK_LEN];
    unsigned char aucIK[D_NSP_TDYTH_NAAAS_4G_IK_LEN];
    unsigned char aucAK[D_NSP_TDYTH_NAAAS_4G_AK_LEN];
    unsigned char aucAUTN[D_NSP_TDYTH_NAAAS_4G_AUTN_LEN];
    unsigned char aucSNID[D_NSP_TDYTH_NAAAS_4G_SNID_LEN];

    unsigned char aucAubody_auc[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    unsigned char aucSQNAK[D_NSP_TDYTH_NAAAS_4G_AK_LEN];

    memset(aucAMF,0,D_NSP_TDYTH_NAAAS_4G_AMF_LEN);   
    memset(aucK,0,D_NSP_TDYTH_NAAAS_4G_KEY_LEN);

    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS stAuBodySeventhAUCToHSS;
    memcpy(aucAubody_auc, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AuBodySixthHSSToAUC_LEN);
    St_NSP_TDYTH_NAAAS_4G_AuBodySixthHSSToAUC *stAuBodySixthHSSToAUC = (St_NSP_TDYTH_NAAAS_4G_AuBodySixthHSSToAUC *)aucAubody_auc;

    if(NSP_TDYTH_4G_verification_timestamp(stAuBodySixthHSSToAUC->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }
    unsigned char *pucAV = (unsigned char*)malloc(sizeof(unsigned char) * D_NSP_TDYTH_NAAAS_4G_AV_LEN);
    if(pucAV == NULL)
    {
        printf("%s,[line=%d],errno=%d:%s\n","HSS:malloc err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 从接收到的buf获取数据，并对数据进行处理

    memcpy(aucIMSI, stAuBodySixthHSSToAUC->aucIMSI, sizeof(aucIMSI));
    memcpy(aucSNID, stAuBodySixthHSSToAUC->aucSNID, sizeof(aucSNID));
    printf("用户身份请求数据解析完成.\n");

/***************************************************************************************************************
    // 2.查找IMSI，找对应的K，调用数据库api进行查找和获取
    MYSQL *conn_ptr = NSP_TDYTH_4G_DB_connect();

    struct DBQueryInfo *pstDbQueryInfo;
    pstDbQueryInfo = (struct DBQueryInfo *) malloc(sizeof(struct DBQueryInfo));


    pstDbQueryInfo->tableName = "NSP_TDYTH_HSS_AUTHEN";
    pstDbQueryInfo->keyName = "IMSI";
    // 查找IMSI是否存在
    if ((NSP_TDYTH_4G_DB_find(conn_ptr, pstDbQueryInfo)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_IMSI err", __LINE__, errno, strerror(errno));
        return -1;
    }
    // 将查询到的结果存到aucIMSI中
    memcpy(aucIMSI, pstDbQueryInfo->keyValue, sizeof(aucIMSI));

    // 根据IMSI查找对应的K
//    memset(pstDbQueryInfo, 0, sizeof(struct DBQueryInfo));
//    fprintf((FILE *) pstDbQueryInfo->keyName, "%s", "KEY");
    pstDbQueryInfo->keyName = "userKEY";
    pstDbQueryInfo->limitName = "IMSI";
    pstDbQueryInfo->limitValue = aucIMSI;
    if ((iRet = NSP_TDYTH_4G_DB_select_key_by_limit(conn_ptr, pstDbQueryInfo)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_SQN err", __LINE__, errno, strerror(errno));
        return -1;
    }
    //将查询到的结果存到aucK中
//	strcpy(aucK, pstDbQueryInfo->keyValue);
    memcpy(aucK, pstDbQueryInfo->keyValue, sizeof(aucK));


    // 3.验证SN id 合法性，从数据库中查找是否存在等操作来判断合法性
    // 查找SNID是否存在
    pstDbQueryInfo->keyName = "SNID";
    if ((NSP_TDYTH_4G_DB_find(conn_ptr, pstDbQueryInfo)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_SNID err", __LINE__, errno, strerror(errno));
        return -1;
    }
    //将查询到的结果存到aucSNID中
//	strcpy(aucSNID, pstDbQueryInfo->keyValue);
    memcpy(aucSNID, pstDbQueryInfo->keyValue, sizeof(aucSNID));
***************************************************************************************************************/

    // 生成随机数SQN和RAND
    printf("开始计算认证向量.\n");
    if ((iRet = NSP_TDYTH_4G_gen_SQN(aucSQN, 0, D_NSP_TDYTH_NAAAS_4G_SQN_LEN)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_SQN err", __LINE__, errno, strerror(errno));
        return -1;
    }
    if ((iRet = NSP_TDYTH_4G_gen_RAND(aucRAND, D_NSP_TDYTH_NAAAS_4G_RAND_LEN)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_RAND err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算消息认证码 MAC=f1(K,SQN,RAND,AMF)
    if ((iRet = NSP_TDYTH_4G_gen_MAC(aucRAND, aucK, aucSQN, aucAMF, aucMAC)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_MAC err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算认证数据 XRES=f2(K,RAND)
    if ((iRet = NSP_TDYTH_4G_gen_RES(aucK, aucRAND, aucXRES)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_RES err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算密文加密密钥 CK=f3(K,RAND)
    if ((iRet = NSP_TDYTH_4G_gen_CK(aucK, aucRAND, aucCK)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_CK err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算完整性保护密钥 IK=f4(K,RAND)
    if ((iRet = NSP_TDYTH_4G_gen_IK(aucK, aucRAND, aucIK)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_CK err", __LINE__, errno, strerror(errno));
        return -1;
    }

    // 计算序列号保护密钥 AK=f5(K,RAND)
    if ((iRet = NSP_TDYTH_4G_gen_AK(aucK, aucRAND, aucAK)) != 0) {
        printf("%s,[line=%d],errno=%d:%s\n", "gen_AK err", __LINE__, errno, strerror(errno));
        return -1;
    }

    if ((iRet = NSP_TDYTH_4G_XOR(aucSQN, aucAK, D_NSP_TDYTH_NAAAS_4G_AK_LEN, aucSQNAK)) != 0) //计算SQN^AK
    {
        printf("%s,[line=%d],errno=%d:%s\n", "xor err", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_GEN;
    }
    
    // 生成认证令牌 AUTN∶=SQN ⊕ AK∥AMF∥MAC
     /*if ((iRet = NSP_TDYTH_4G_gen_AUTN(aucSQN, aucAK, aucAMF, aucMAC, aucAUTN)) != 0) {
         printf("%s,[line=%d],errno=%d:%s\n", "gen_AUTN err", __LINE__, errno, strerror(errno));
        return -1;
     }*/


    memcpy(aucAUTN,aucSQNAK,D_NSP_TDYTH_NAAAS_4G_AK_LEN);
    memcpy(aucAUTN+D_NSP_TDYTH_NAAAS_4G_AK_LEN,aucAMF,D_NSP_TDYTH_NAAAS_4G_AMF_LEN);
    memcpy(aucAUTN+D_NSP_TDYTH_NAAAS_4G_AK_LEN+D_NSP_TDYTH_NAAAS_4G_AMF_LEN,aucMAC,D_NSP_TDYTH_NAAAS_4G_MAC_LEN);

    printf("认证向量计算完毕.\n");

    // 构造发送数据包头部
    printf("开始构造认证数据.\n");
    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x07;

    // 构造发送数据包内容
    stAuBodySeventhAUCToHSS.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stAuBodySeventhAUCToHSS.aucIdNumber = 0x0;
    memcpy(stAuBodySeventhAUCToHSS.aucDstId, stAuBodySixthHSSToAUC->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stAuBodySeventhAUCToHSS.aucSrcId, stAuBodySixthHSSToAUC->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stAuBodySeventhAUCToHSS.aucDstPort = stAuBodySixthHSSToAUC->aucDstPort;
    stAuBodySeventhAUCToHSS.aucSrcPort = stAuBodySixthHSSToAUC->aucSrcPort;
    stAuBodySeventhAUCToHSS.iDataLen = 0;

    memcpy(stAuBodySeventhAUCToHSS.aucSQN, aucSQN, D_NSP_TDYTH_NAAAS_4G_SQN_LEN);
    memcpy(stAuBodySeventhAUCToHSS.aucCK, aucCK, D_NSP_TDYTH_NAAAS_4G_CK_LEN);
    memcpy(stAuBodySeventhAUCToHSS.aucIK, aucIK, D_NSP_TDYTH_NAAAS_4G_IK_LEN);
    memcpy(stAuBodySeventhAUCToHSS.aucAK, aucAK, D_NSP_TDYTH_NAAAS_4G_AK_LEN);
    memcpy(stAuBodySeventhAUCToHSS.aucRAND, aucRAND, D_NSP_TDYTH_NAAAS_4G_RAND_LEN);
    memcpy(stAuBodySeventhAUCToHSS.aucAUTN, aucAUTN, D_NSP_TDYTH_NAAAS_4G_AUTN_LEN);
    memcpy(stAuBodySeventhAUCToHSS.aucXRES, aucXRES, D_NSP_TDYTH_NAAAS_4G_RES_LEN);

    memcpy(stAuBodySeventhAUCToHSS.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    // 构造发送数据缓冲区
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stAuBodySeventhAUCToHSS, D_NSP_TDYTH_NAAAS_4G_AuBodySeventhAUCToHSS_LEN);
    return 0;
}

/*
 * 功能：AUC端接收监听
 * 参数：  
 * 返回值：   0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_ListenAccept_AUC_Function()
{
    //一直接收数据
    int iSockfd;
    int iRet;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_AUC_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_AUC_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];

    // AUC
    int iSockfdAUC = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in AUCSockaddr;
    bzero(&AUCSockaddr, sizeof(AUCSockaddr));
    AUCSockaddr.sin_family = AF_INET;
    AUCSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_AUC_PORT);
    AUCSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_AUC_IP);
    int iAUCAddrLen = sizeof(AUCSockaddr);

    // bind 成功返回0，出错返回-1
    if (bind(iSockfdAUC, (struct sockaddr *) &AUCSockaddr, sizeof(AUCSockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));

        iRet = recvfrom(iSockfdAUC, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_AUC_BUFF_LEN, MSG_DONTWAIT, (struct sockaddr *) &AUCSockaddr, &iAUCAddrLen);


        if (iRet > 0)
        {
            memcpy(cHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *cHeader_hss = (St_NSP_TDYTH_NAAAS_4G_Header *)cHeader;
            printf("-------------step6. AUC#Receiver <- HSS：0x06-------------\n");
            if(cHeader_hss->ucReqType != 0x01)
            {
                printf("%s,[line=%d],errno=%d:%s\n", "ReqType errno.", __LINE__, errno, strerror(errno));
                break;
            }
            if(cHeader_hss->ucStep == 0x06)
            {
                if(NSP_TDYTH_4G_accessAuth_AUCProcess(cRecvBuf, cSendBuf) < 0) break;

                if ((iRet = sendto(iSockfdAUC, cSendBuf, D_NSP_TDYTH_NAAAS_4G_AUC_BUFF_LEN, 0,
                        (struct sockaddr *) &AUCSockaddr, sizeof(AUCSockaddr))) < 0)
                {
                    printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
                    return -1;
                }
                else
                {
                    printf("认证数据响应发送完毕,step: 7 完成.\n");
                }
                printf("-------------step7: AUC#Sender -> HSS：0x07-------------\n");
            } else{
                printf("%s,[line=%d],errno=%d:%s\n", "Step errno.", __LINE__, errno, strerror(errno));
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

    close(iSockfdAUC);

    return 0;
}

int main()
{
    puts("PGW开启监听状态");
    NSP_TDYTH_4G_ListenAccept_AUC_Function();
    return 0;
}
