 /*
 * 文件功能：PCRF网元（涉及计费等）
 * 作者：张才玲 刘梦如
 * 创建日期：2020/11/10
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



/*
 * 功能：专有承载建立第一步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_BearerSetUp_First_PCRFToPGW(char *pcRecvBuf, char *pcSendBuf)
{
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW stBearerSetUpFirstPCRFToPGW;

    stHeader.ucVersion = 0x00;
    stHeader.usModuleCode = 0x0000;
    stHeader.ucReqType = 0x0b;//暂时认为0x0b是专有承载的建立
    stHeader.ucSubType = 0x00;
    stHeader.ucUserCategory = 0x00;
    stHeader.ucStep = 0x01;

    stBearerSetUpFirstPCRFToPGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stBearerSetUpFirstPCRFToPGW.aucIdNumber = 0x0;
    memcpy(stBearerSetUpFirstPCRFToPGW.aucDstId,D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stBearerSetUpFirstPCRFToPGW.aucSrcId,D_NSP_TDYTH_NAAAS_PCRF_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBearerSetUpFirstPCRFToPGW.aucSrcPort = D_NSP_TDYTH_NAAAS_PCRF_PORT;
    stBearerSetUpFirstPCRFToPGW.aucDstPort = D_NSP_TDYTH_NAAAS_UE_PORT;
    stBearerSetUpFirstPCRFToPGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW);

    memcpy(stBearerSetUpFirstPCRFToPGW.aucTerminalId, "0000000000000000", D_NSP_TDYTH_NAAAS_4G_ID_LEN);//还不确定里面写什么
    printf("专有承载建立数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBearerSetUpFirstPCRFToPGW, sizeof(St_NSP_TDYTH_NAAAS_4G_BearerSetUpFirstPCRFToPGW));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

/*
 * 功能： PCRF发起专有承载建立
 * 参数：
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_BearerSetUp_Function()
{
    int iRet;

    /* ------------------------------------*/
    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_MME_BUFF_LEN];

    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // PGW
    struct sockaddr_in PGWSockaddr;
    bzero(&PGWSockaddr, sizeof(PGWSockaddr));
    PGWSockaddr.sin_family = AF_INET;
    PGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PGW_PORT);
    PGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PGW_IP);
    int sin_len = sizeof(PGWSockaddr);

    /****************************************附着流程，属于以前放在while1循环中的功能******************************************/
    memset(cRecvBuf, 0, sizeof(cRecvBuf));
    memset(cSendBuf, 0, sizeof(cSendBuf));

    NSP_TDYTH_4G_BearerSetUp_First_PCRFToPGW(cRecvBuf,cSendBuf);
    iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &PGWSockaddr, sin_len);
    if(iRet > 0)
    {
        printf("-------------step1. PCRF#Sender -> PGW：0x01-------------\n");
        printf("PCRF->PGW 专有承载触发，通知PGW!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "PCRF:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

    close(iSockfd);
    return 0;
}
/**********************************************************
 * 函数名：	 NSP_TDYTH_4G_ListenAccept_Function
 * 功能：    接受监听函数
 * 参数：	 void
 * 返回值：  0   成功
 *          <0   失败
 *********************************************************/
int NSP_TDYTH_4G_ListenAccept_Function()
{
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];

    // PGW
    struct sockaddr_in PGWSockaddr;
    bzero(&PGWSockaddr, sizeof(PGWSockaddr));
    PGWSockaddr.sin_family = AF_INET;
    PGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PGW_PORT);
    PGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PGW_IP);

    // SGW
    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in PCRFSockaddr;
    bzero(&PCRFSockaddr, sizeof(PCRFSockaddr));
    PCRFSockaddr.sin_family = AF_INET;
    PCRFSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PCRF_PORT);
    PCRFSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PCRF_IP);
    int iAddrLen = sizeof(PCRFSockaddr);

    // bind，成功返回0，出错返回-1
    if (bind(iSockfd, (struct sockaddr *) &PCRFSockaddr, sizeof(PCRFSockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));

        iRet = recvfrom(iSockfd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_SGW_BUFF_LEN, 0,
                        (struct sockaddr *) &PCRFSockaddr, &iAddrLen);

        if (iRet > 0)
        {
            memcpy(cHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *)cHeader;             // 解析从mme发来的包头

            switch (pstHeader->ucReqType)   // 判断请求类别
            {
                case 0x0b:          
                    printf("-------------目前处于专有承载建立流程！-------------\n");
                    switch (pstHeader->ucStep)     // 判断步骤码
                    {
                        case 0x0c:      
                            printf("-------------step12. PCRF#Receiver <- PGW：0x0c-------------\n");

                            break;

                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
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

    close(iSockfd);

    return  iRet;
}


int main()
{
    int PCRF_function;

    pthread_t pth_epoll;

    printf("------------开启子线程进行监听------------\n");

    if(pthread_create(&pth_epoll, NULL, (void *)NSP_TDYTH_4G_ListenAccept_Function, NULL))
    {
       perror("pthread_creat error!\n");
       return -1;
    }
    else
    {
        printf("---------------监听子线程开启成功---------------\n");
    }
    
    while(1)
    {
        puts("请选择S-GW需要执行的功能：");
        puts("1.专有承载建立");
        puts("99.退出");
        puts("......后续添加.......");
        scanf("%d", &PCRF_function);
        switch(PCRF_function)
        {
            case 1:
                NSP_TDYTH_4G_BearerSetUp_Function();
                break;
            case 99:
                return 0;
            default:
                printf("输入错误\n");
                break;
        }
    }
    return 0;
}
