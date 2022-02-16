/*
 * 文件功能:NET端
 * 作者：李高明 张才玲
 * 创建日期：
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_anet.h"
#include "NSP_TDYTH_NAAAS_4G_gen.h"
#include "NSP_TDYTH_NAAAS_4G_utils.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"
#include "NSP_TDYTH_NAAAS_4G_debug.h"
#include "NSP_TDYTH_NAAAS_4G_db.h"

int HandleipRequest(char *pcRecvBuf,char *pcResult)
{
    int readLen = 0;
    int writeRet;
    int sockfd;
    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet));
    St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet *stIpBodyFourthPGWToNet = (St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet *)aucbody;

    //创建TCP套接字
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("socket create failed");
        return -1;
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(stIpBodyFourthPGWToNet->aucDstPort);
    servaddr.sin_addr.s_addr = inet_addr(stIpBodyFourthPGWToNet->aucDstIp);

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error!\n");
        return -2;
    }
    writeRet = write(sockfd, stIpBodyFourthPGWToNet->aucRequest, strlen(stIpBodyFourthPGWToNet->aucRequest));
    if (writeRet < 0) 
    {
        printf("write error:%d,%s\n", errno, strerror(errno));
        return -3;
    }
    printf(">>>>>>> success with %d byte(s) <<<<<<<\n", writeRet);

    struct timeval tv;
    int selectRet = 0;
    fd_set t_set1;

    tv.tv_sec= 2;
    tv.tv_usec= 0;

    FD_ZERO(&t_set1);
    FD_SET(sockfd, &t_set1);

    selectRet = select(sockfd + 1, &t_set1, NULL, NULL, &tv);

    if (selectRet < 0) 
    {
        close(sockfd);
        printf("select failed!\n");
        return -1;
    }

    if (selectRet > 0)
    {
        readLen = read(sockfd, pcResult, D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN-1); // read once only!
        printf("readLen:%d\n", readLen);
        printf("\n\n%s\n\n", pcResult);
    }

    // unsigned char aucshell[100];
    // memset(aucshell,0,100);
    // sprintf(aucshell,"http --verify=no %s -o /tmp/web.html",stIpBodyFourthPGWToNet->aucDstIp);
    // printf("%s\n",aucshell);
    // system(aucshell);
    close(sockfd);
    return 0;
}
/*
 * 功能：上网请求第五步
 * 参数：   @pcRecvBuf [in] 收到的数据
 *          @pcSendBuf [out] 发送的数据
 * 返回值：    0   成功
 */
int NSP_TDYTH_4G_IpNet_Fifth_NetSendToPGW(char *pcRecvBuf, char *pcSendBuf)
{
    unsigned char aucResult[D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN];
    memset(aucResult, 0, D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN);
    if(HandleipRequest(pcRecvBuf,aucResult) < 0)
    {
        printf("上网请求失败\n");
        strcpy(aucResult,"failed");
    }
    int iRet;

    unsigned char aucbody[D_NSP_TDYTH_NAAAS_4G_BUFF_LEN];
    memcpy(aucbody, pcRecvBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet));

    St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet *stIpBodyFourthPGWToNet = (St_NSP_TDYTH_NAAAS_4G_IpBodyFourthPGWToNet *)aucbody;

    if(NSP_TDYTH_4G_verification_timestamp(stIpBodyFourthPGWToNet->TimeStamp) < 0)
    {
        printf("time error!\n");
        return -1;
    }

	St_NSP_TDYTH_NAAAS_4G_Header stHeader;
	St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW stIpBodyFifthNetToPGW;
	memcpy(&stHeader, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    stHeader.ucStep = 0x05;

    stIpBodyFifthNetToPGW.TimeStamp = NSP_TDYTH_4G_get_time_now();
    stIpBodyFifthNetToPGW.aucIdNumber = 0x04;
    memcpy(stIpBodyFifthNetToPGW.aucDstId, stIpBodyFourthPGWToNet->aucSrcId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);//源、目的互换
    memcpy(stIpBodyFifthNetToPGW.aucSrcId, stIpBodyFourthPGWToNet->aucDstId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    stIpBodyFifthNetToPGW.aucSrcPort = stIpBodyFourthPGWToNet->aucDstPort;
    stIpBodyFifthNetToPGW.aucDstPort = stIpBodyFourthPGWToNet->aucSrcPort;
    stIpBodyFifthNetToPGW.iDataLen = D_NSP_TDYTH_NAAAS_4G_HEADER_LEN + sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW);

    memcpy(stIpBodyFifthNetToPGW.aucDstIp, stIpBodyFourthPGWToNet->aucSrcIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyFifthNetToPGW.aucSrcIp, stIpBodyFourthPGWToNet->aucDstIp, D_NSP_TDYTH_NAAAS_4G_IP_LEN);
    memcpy(stIpBodyFifthNetToPGW.aucGUTI, stIpBodyFourthPGWToNet->aucGUTI, D_NSP_TDYTH_NAAAS_4G_GUTI_LEN);
    memcpy(stIpBodyFifthNetToPGW.aucResult, aucResult, D_NSP_TDYTH_NAAAS_4G_IP_Result_LEN); //"......"这里需要填写返回信息

    memcpy(stIpBodyFifthNetToPGW.aucTerminalId, stIpBodyFourthPGWToNet->aucTerminalId, D_NSP_TDYTH_NAAAS_4G_ID_LEN);
    printf("上网响应数据构造完毕.\n");
    memcpy(pcSendBuf, &stHeader, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
    memcpy(pcSendBuf + D_NSP_TDYTH_NAAAS_4G_HEADER_LEN, &stIpBodyFifthNetToPGW, sizeof(St_NSP_TDYTH_NAAAS_4G_IpBodyFifthNetToPGW));

    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}



/**********************************************************
 * 函数名：	 NSP_TDYTH_4G_NET_ListenAccept_Function
 * 功能：    NET端--接受监听函数
 * 参数：	 void
 * 返回值：  0   成功
 *          <0   失败
 *********************************************************/
int NSP_TDYTH_4G_NET_ListenAccept_Function()
{
    int iRet;

    char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cSendBuf[D_NSP_TDYTH_NAAAS_4G_HSS_BUFF_LEN];
    char cHeader[D_NSP_TDYTH_NAAAS_4G_HEADER_LEN];

    struct sockaddr_in PGWSockaddr;
    bzero(&PGWSockaddr, sizeof(PGWSockaddr));
    PGWSockaddr.sin_family = AF_INET;
    PGWSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_PGW_PORT);
    PGWSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_PGW_IP);

    int iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in NETSockaddr;
    bzero(&NETSockaddr, sizeof(NETSockaddr));
    NETSockaddr.sin_family = AF_INET;
    NETSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_NET_PORT);
    NETSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_NET_IP);
    int iAddrLen = sizeof(NETSockaddr);

    // bind，成功返回0，出错返回-1
    if (bind(iSockfd, (struct sockaddr *) &NETSockaddr, sizeof(NETSockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }

    while (1)
    {
        memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(cSendBuf, 0, sizeof(cSendBuf));

        iRet = recvfrom(iSockfd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, 0,
                        (struct sockaddr *) &NETSockaddr, &iAddrLen);

        if (iRet > 0)
        {
            memcpy(cHeader, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_HEADER_LEN);
            St_NSP_TDYTH_NAAAS_4G_Header *pstHeader = (St_NSP_TDYTH_NAAAS_4G_Header *)cHeader;             // 解析从mme发来的包头

            switch (pstHeader->ucReqType)  
            {

                case 0x10: 
                    printf("-------------目前处于上网流程！-------------\n");
                    switch (pstHeader->ucStep)  
                    {
                        case 0x04:      
                            printf("-------------step4.NET#Receiver <-PGW:0x04-------------\n");
         
                            if(NSP_TDYTH_4G_IpNet_Fifth_NetSendToPGW(cRecvBuf, cSendBuf) < 0 ) break;
                            if ((iRet = sendto(iSockfd, cSendBuf, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, MSG_DONTWAIT,
                                               (struct sockaddr *) &PGWSockaddr, sizeof(PGWSockaddr))) < 0)
                            {
                                printf("%s,[line=%d],errno=%d:%s\n", "UE:UDP ipv4 socket err", __LINE__, errno,
                                       strerror(errno));
                                return -1;
                            } 
                            else 
                            {
                                printf("-------------step5: NET#Sender -> PGW：0x5-------------\n");
                            }
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

    return iRet;
}

int main()
{
    int NET_function;
    printf("NET开启监听状态\n");
    NSP_TDYTH_4G_NET_ListenAccept_Function();
    return 0;
}