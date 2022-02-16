/*
 * 文件功能：ENB3实体，恶意基站
 * 作者
 * 创建日期：2021/03/02
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_anet.h"
#include "NSP_TDYTH_NAAAS_4G_utils.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"
#include "NSP_TDYTH_NAAAS_4G_debug.h"
#include "NSP_TDYTH_NAAAS_4G_db.h"
#include "NSP_TDYTH_NAAAS_4G_gen.h"

static int gs_ix = 200;                                         //基站X坐标
static int gs_iy = 200;                                         //基站Y坐标
static unsigned int gs_uiENB_ue_id = 2000;                      //ENB分给UE的id
static int gs_iIsOpen = 0;                                      //基站是否开启。0为关闭，1为开启。

static int gs_iENB3_PCI = D_NSP_TDYTH_NAAAS_4G_ENB3_PCI;
static int gs_iENB3_EARFCN = D_NSP_TDYTH_NAAAS_4G_ENB3_EARFCN;
static int gs_iENB3_RSRP = D_NSP_TDYTH_NAAAS_4G_ENB3_RSRP;

/***************************************************************附着*************************************************************************************/
/*
 * 功能：拒绝附着请求
 * 参数：   @pcRecvBuf [in] 收到的数据
 *         @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_AucRefuse_eNB_Send(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_AucRefuseBody stRefuseBodyENBToUE;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB_LEN);
    St_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB *stAttBodyFirstUEToeNB = (St_NSP_TDYTH_NAAAS_4G_AttBodyFirstUEToeNB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stAttBodyFirstUEToeNB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucReqType = 0x12;
    stHeader.ucStep = 0x00;

    stRefuseBodyENBToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stRefuseBodyENBToUE.aucDstId, stAttBodyFirstUEToeNB->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    memcpy(stRefuseBodyENBToUE.aucSrcId, stAttBodyFirstUEToeNB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stRefuseBodyENBToUE.uiSrcPort = stAttBodyFirstUEToeNB->aucSrcPort;
    stRefuseBodyENBToUE.uiDstPort = stAttBodyFirstUEToeNB->aucDstPort;
    stRefuseBodyENBToUE.iPCI = 66;

    memcpy(stRefuseBodyENBToUE.aucTerminalId, stAttBodyFirstUEToeNB->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stRefuseBodyENBToUE, D_NSP_TDYTH_NAAAS_4G_AucRefuseBody_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf,D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,stAttBodyFirstUEToeNB->aucSrcId,
                                            stAttBodyFirstUEToeNB->aucSrcPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UDP ipv4 send err", __LINE__, errno, strerror(errno));
    } else {
        printf("拒绝认证消息发送完毕\n");
    }

    return 0;
}
/***************************************************************附着End*************************************************************************************/

/***************************************************************TAU*************************************************************************************/
/*
 * 功能：  TAU流程Reject
 * 参数：
 * 返回值：
 *            >=0   成功
 *            < 0   失败
 */
NSP_TDYTH_4G_TAU_Reject_ENBToUE(int iSockfd, char *pcRecvBuf, char *pcSendBuf)
{
    int iRet;
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE stTauRejectENBToUE;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, D_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB_LEN);
    St_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB *stTauBodyFirstUEToENB = (St_NSP_TDYTH_NAAAS_4G_TauBodyFirstUEToENB *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stTauBodyFirstUEToENB->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

    memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucReqType = 0x12;
    stHeader.ucStep = 0x1;

    stTauRejectENBToUE.TimeStamp = NSP_TDYTH_4G_get_time_now();
    memcpy(stTauRejectENBToUE.aucDstId, stTauBodyFirstUEToENB->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauRejectENBToUE.uiDstPort = stTauBodyFirstUEToENB->uiSrcPort;
    memcpy(stTauRejectENBToUE.aucSrcId, D_NSP_TDYTH_NAAAS_ENB3_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stTauRejectENBToUE.uiSrcPort = D_NSP_TDYTH_NAAAS_ENB3_PORT;
    stTauRejectENBToUE.uiDataLen = 0;

    stTauRejectENBToUE.iEMM_Cause = 0b00000111;

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stTauRejectENBToUE, D_NSP_TDYTH_NAAAS_4G_TauRejectENBToUE_LEN);

    if ((iRet = NSP_TDYTH_4G_ANET_ipv4_send(iSockfd, pcSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN,
                                            stTauRejectENBToUE.aucDstId, stTauRejectENBToUE.uiDstPort, 0)) < 0)
    {
        printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno, strerror(errno));
        return NULL;
    }

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}
/***************************************************************TAU End*************************************************************************************/
/*
 * 功能：eNB进入监听状态
 * 参数：  无
 * 返回值：  0   成功
 *         <0   失败
 */
void * NSP_TDYTH_4G_ListenAccept_eNB_Function(int* iSockfd)
{
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_ENB_BUFF_LEN];
    char acHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];          //对mme发来的数据包头进行解析

    pid_t pid;
    pthread_t tid;
    pid = getpid();
    tid = pthread_self();

    printf(" 当前线程信息为 :pid %u tid %u (0x%x)\n", (unsigned int) pid,
           (unsigned int) tid, (unsigned int) tid);
    // UE
    struct sockaddr_in UESockaddr;
    bzero(&UESockaddr, sizeof(UESockaddr));
    UESockaddr.sin_family = AF_INET;
    UESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);

    // ENB3
    struct sockaddr_in ENB3Sockaddr;
    bzero(&ENB3Sockaddr, sizeof(ENB3Sockaddr));
    ENB3Sockaddr.sin_family = AF_INET;
    ENB3Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB3_PORT);
    ENB3Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB3_IP);

    // MME1
    struct sockaddr_in MMESockaddr;
    bzero(&MMESockaddr, sizeof(MMESockaddr));
    MMESockaddr.sin_family = AF_INET;
    MMESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_MME1_PORT);
    MMESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_MME1_IP);

    int iSinLen = sizeof(ENB3Sockaddr);

    while(1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));
        iRet = recvfrom(*iSockfd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_UE_BUFF_LEN, MSG_DONTWAIT,
                        (struct sockaddr *) &ENB3Sockaddr, &iSinLen);
        if (iRet < 0)
        {
            return NULL;
        }
        if (iRet > 0)
        {
            memcpy(acHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *) acHeader;
            switch(pstHeader->ucReqType)
            {
                case 0x03:
                    printf("----------------------目前处于UE附着流程！---------------------\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x01:
                            printf("------------------------step1: ENB#Receiver <- UE:0x01------------------------\n");
                            NSP_TDYTH_4G_AucRefuse_eNB_Send(*iSockfd, cRecvBuf, cSendBuf);
                            break;
                        default:
                            printf("%s,[line=%d],errno=%d:%s\n", "step errno.", __LINE__, errno, strerror(errno));
                            break;
                    }
                    break;
                case 0x13:
                    printf("\n----------目前处于TAU流程----------\n\n");
                    switch (pstHeader->ucStep)
                    {
                        case 0x01:
                            printf("----------TAU: step1. ENB#Receiver -> UE：TAU Request----------\n");
                            if (NSP_TDYTH_4G_TAU_Reject_ENBToUE(*iSockfd, cRecvBuf, cSendBuf) < 0) {
                                printf("----------TAU: step2. SEND ERROR----------\n");
                                break;
                            } else {
                                printf("----------TAU: step2. ENB#Sender -> UE：TAU Reject----------\n");
                            }
                            break;
                        default:
                            printf("收到TAU包的步骤信息无法识别！[Step=%d]\n", pstHeader->ucStep);
                            break;
                    }
                default:
                    printf("%s ReqType=%d,[line=%d],errno=%d:%s\n", "ReqType errno.", pstHeader->ucReqType, __LINE__, errno, strerror(errno));
                    break;
            }
        }
    }
    return NULL;
}

/*
 * 功能：构造广播包
 * 参数： @xxx [pcSendBuf in] 发送缓冲区
 * 返回值：  0   成功
 *          < 0 失败
 */
int NSP_TDYTH_4G_Broadcast_Enb_Send(char *pcSendBuf)
{
    // 构造广播包内容（以下数据仅针对ENB1，之后可以修改为动态数据）
    //printf("开始构造广播包内容\n");
    St_NSP_TDYTH_NAAAS_4G_Header stHeader;
    St_NSP_TDYTH_NAAAS_4G_BroadcastBody stBroadcastBody;
    memset(pcSendBuf, 0, sizeof(pcSendBuf));

    stHeader.ucVersion = 0x01;
    stHeader.usModuleCode = 0x0001;
    stHeader.ucReqType = 0x11;              // 类型为广播包；其他不重要，置为1
    stHeader.ucSubType = 0x01;
    stHeader.ucUserCategory = 0x01;
    stHeader.ucStep = 0x01;

    stBroadcastBody.uiDataLen = D_NSP_TDYTH_NAAAS_4G_BroadcastBody_LEN;
    stBroadcastBody.uiENBID = D_NSP_TDYTH_NAAAS_4G_ENB3_ENBID;
    stBroadcastBody.uiPCI = gs_iENB3_PCI;
    stBroadcastBody.uiEARFCN = gs_iENB3_EARFCN;
    stBroadcastBody.iRSRP = gs_iENB3_RSRP;
    stBroadcastBody.iCellRP = D_NSP_TDYTH_NAAAS_4G_ENB3_CellRP;
    stBroadcastBody.iQrxlevmin = D_NSP_TDYTH_NAAAS_4G_ENB3_Qrxlevmin;
    memcpy(stBroadcastBody.aucTAI, D_NSP_TDYTH_NAAAS_4G_ENB3_TAI, D_NSP_TDYTH_NAAAS_4G_TAI_LEN);

    memcpy(stBroadcastBody.aucENBIp, D_NSP_TDYTH_NAAAS_ENB3_IP, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stBroadcastBody.uiENBPort = D_NSP_TDYTH_NAAAS_ENB3_PORT;

    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stBroadcastBody, D_NSP_TDYTH_NAAAS_4G_BroadcastBody_LEN);
    //printf("广播包内容构造完毕\n");

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;

}


/*
 * 功能：向所有UE发送广播包，其中包含此基站的相关信息
 * 参数：  无
 * 返回值：  0   成功
 *         <0   失败
 */
int NSP_TDYTH_4G_Broadcast_eNB_Function()
{
    int iRet;
    int ENBSockfd;

    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];

    ENBSockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // ENB
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB3_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB3_IP);

    // UE1
    struct sockaddr_in UE1Sockaddr;
    bzero(&UE1Sockaddr, sizeof(UE1Sockaddr));
    UE1Sockaddr.sin_family = AF_INET;
    UE1Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE_PORT);
    UE1Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE_IP);
    // UE2
    struct sockaddr_in UE2Sockaddr;
    bzero(&UE2Sockaddr, sizeof(UE2Sockaddr));
    UE2Sockaddr.sin_family = AF_INET;
    UE2Sockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_UE2_PORT);
    UE2Sockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_UE2_IP);

    /*设置socket属性，端口可以重用*/
    int opt = SO_REUSEADDR;
    setsockopt(ENBSockfd,SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
    setnonblocking(ENBSockfd);

    //持续发送广播包
    while(1)
    {
        NSP_TDYTH_4G_Broadcast_Enb_Send(cSendBuf);      // 构造广播包
        if ((iRet = sendto(ENBSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                           (struct sockaddr *) &UE1Sockaddr, sizeof(UE1Sockaddr))) < 0) {
            printf("%s,[line=%d],errno=%d:%s\n", "broadcast err", __LINE__, errno, strerror(errno));
            return -1;
        } else {
            //printf("ENB->UE1 广播包发送成功!\n");
        }
        sleep(D_NSP_TDYTH_NAAAS_4G_EnbBroadcast_Interval);
    }

    close(ENBSockfd);

    return 0;
}

/*
 * 功能： 修改当前ENB的参数
 * 参数：  无
 * 返回值：  0   成功
 *         <0   失败
 */
int NSP_TDYTH_4G_EditParameter_eNB_Function()
{
    int opt, value;
    while(1)
    {
        printf("请选择需要修改的参数:\n");
        printf("1: uiPCI\n");
        printf("2: uiEARFCN\n");
        printf("3: iRSRP\n");
        scanf("%d",&opt);
        getchar();
        switch(opt)
        {
            case 1:
                while(1)
                {
                    printf("当前PCI为  %d\n", gs_iENB3_PCI);
                    printf("请输入需要修改的值:\n");
                    scanf("%d",&value);
                    getchar();
                    if(value >= 0 && value <= 503)
                    {
                        gs_iENB3_PCI = value;
                        printf("修改成功！\n");
                        break;
                    }else{
                        printf("输入值不符合规范！\n\n");
                    }
                }
                break;
            case 2:
                while(1)
                {
                    printf("当前EARFCN为  %d\n", gs_iENB3_EARFCN);
                    printf("请输入需要修改的值:(38400/38544)\n");
                    scanf("%d",&value);
                    getchar();
                    if(value == 38400 || value == 38544)
                    {
                        gs_iENB3_EARFCN = value;
                        printf("修改成功！\n");
                        break;
                    }else{
                        printf("输入值不符合规范！\n\n");
                    }
                }
                break;
            case 3:
                while(1)
                {
                    printf("当前RSRP为  %d\n", gs_iENB3_RSRP);
                    printf("请输入需要修改的值:\n");
                    scanf("%d",&value);
                    getchar();
                    if(value >= -200 && value <= -30)
                    {
                        gs_iENB3_RSRP = value;
                        printf("修改成功！\n");
                        break;
                    }else{
                        printf("输入值不符合规范！\n\n");
                    }
                }
                break;
            default:
                printf("输入错误！\n\n");
                break;
        }
    }
    return 0;
}

/*
 * 功能：采用epoll模型，创建线程处理消息
 * 参数：  无
 * 返回值：  0   成功
 *         <0   失败
 */
int NSP_TDYTH_4G_Epoll_eNB_Function()
{
    int iFD,kdpfd,nfds,n,curfds;

    struct epoll_event ev;
    struct epoll_event events[D_NSP_TDYTH_NAAAS_4G_MAXEPOLLSIZE];
    struct rlimit rt;

    pthread_t thread, thread2, thread3;
    pthread_attr_t attr;

    /* 设置每个进程允许打开的最大文件数 */
    rt.rlim_max = rt.rlim_cur = 100;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
    {
        perror("setrlimit");
        exit(1);
    }

    /* 开启 socket 监听 */
    if ((iFD = socket(PF_INET, SOCK_DGRAM, 0)) == -1)
    {
        perror("socket create failed");
        exit(1);
    }

    /*设置socket属性，端口可以重用*/
    int opt=SO_REUSEADDR;
    setsockopt(iFD,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
    setnonblocking(iFD);

    //ENB Sockaddr
    struct sockaddr_in ENBSockaddr;
    bzero(&ENBSockaddr, sizeof(ENBSockaddr));
    ENBSockaddr.sin_family = AF_INET;
    ENBSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_ENB3_PORT);
    ENBSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_ENB3_IP);

    if (bind(iFD, (struct sockaddr *) &ENBSockaddr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    /* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
    kdpfd = epoll_create(D_NSP_TDYTH_NAAAS_4G_MAXEPOLLSIZE);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = iFD;

    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, iFD, &ev) < 0)
    {
        fprintf(stderr, "epoll set insertion error:fd=%d",iFD);
        return -1;
    }

    // 建立一个线程，用来持续发送发广播包
    if(pthread_create(&thread2, NULL, (void*)NSP_TDYTH_4G_Broadcast_eNB_Function, NULL)){
        perror("pthread2_creat error!\n");
        exit(-1);
    }else{
        printf("播放广播包子线程开启成功\n");
    }

    // 建立一个线程，用来进行参数修改
    if(pthread_create(&thread3, NULL, (void*)NSP_TDYTH_4G_EditParameter_eNB_Function, NULL)){
        perror("pthread3_creat error!\n");
        exit(-1);
    }else{
        printf("参数修改子线程开启成功\n");
    }

    printf("ENB开启监听状态\n");
    for( ; ; )
    {
        nfds = epoll_wait(kdpfd, events, 10000, 2000);

        /* 处理所有事件 */
        for (n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == iFD)
            {
                /*初始化属性值，均设为默认值*/
                pthread_attr_init(&attr);
                pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

                /*  设置线程为分离属性*/
                pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
                if(pthread_create(&thread,&attr,(void*)NSP_TDYTH_4G_ListenAccept_eNB_Function,(void*)&(events[n].data.fd)))
                {
                    perror("pthread_creat error!\n");
                    exit(-1);
                }
            }
        }
    }
    close(kdpfd);
    close(iFD);
    return 0;
}

int main()
{
    while(1)
    {
        printf("是否开启ENB ( 1->开启 )\n");
        scanf("%d", &gs_iIsOpen);
        getchar();
        if(gs_iIsOpen == 1) NSP_TDYTH_4G_Epoll_eNB_Function();
        return 0;
    }
}



