/*
 * 文件功能：4G接入认证协议UDP相关文件
 * 作者：董珂
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_anet.h"

#define SOCK_RECV_TIMEOUT	2	//为测试超时，暂时改为2秒

/******ipv4******/

/*
 * 功能：UDP-ipv4接收初始化
 * 参数：@usPort[in]，端口号
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv4_recv_init(unsigned short usPort, struct sockaddr_in *pstSin4)
{
    int iSockfd;
	//时间
	
    struct timeval stTimeOut;

    iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(iSockfd == -1) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 socket", __LINE__, errno, strerror(errno));
        //err_print();
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_SOCKET;
    }

    stTimeOut.tv_sec = SOCK_RECV_TIMEOUT;
    stTimeOut.tv_usec = 0;
    if (setsockopt(iSockfd, SOL_SOCKET, SO_RCVTIMEO, &stTimeOut, sizeof(struct timeval)) < 0) {
        close(iSockfd);
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_SETSOCKETOPT;
    }

    bzero(pstSin4, sizeof(struct sockaddr_in));
    pstSin4->sin_family = AF_INET;
    pstSin4->sin_port = htons(usPort);
    pstSin4->sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(iSockfd, (struct sockaddr *)pstSin4, sizeof(struct sockaddr)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        //err_print();
        close(iSockfd);
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
    }
    return iSockfd;
}

/*
 * 功能：UDP-ipv4发送初始化
 * 参数：
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv4_send_init()
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

/*
 * 功能：UDP-ipv4发送初始化
 * 参数：@usPort[in]，端口号
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv4_send_init_ex(unsigned short usPort)
{
    int iSockfd;
    struct sockaddr_in stSin4;

    iSockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(iSockfd == -1) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        //err_print();
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_SOCKET;
    }

    bzero(&stSin4, sizeof(stSin4));
    stSin4.sin_family = AF_INET;
    stSin4.sin_port = htons(usPort);
    stSin4.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(iSockfd, (struct sockaddr *)&stSin4, sizeof(struct sockaddr)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        //err_print();
        close(iSockfd);
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
    }
    return iSockfd;
}

/*
 * 功能：UDP-ipv4发送数据
 * 参数：@iSockfd[in]	套接字
 *		@pucBuf[in]		发送数据缓冲区
 *		@iLen[in]		发送数据长度
 *		@pcIP[in]		目的IP地址
 *		@usPort[in]		目的端口号
 *		@iBlock[in]		是否阻塞，1表示阻塞，0表示非阻塞
 * 返回值：成功返回发送数据长度，失败返回错误码
 * 备注：dstAddr可以为NULL，dstLen可以为0
 */
int NSP_TDYTH_4G_ANET_ipv4_send(int iSockfd, unsigned char *pucBuf, int iLen, char *pcIP, unsigned short usPort, int iBlock)
{
    if ((pucBuf == NULL) || (iLen <= 0)) {
        printf("%s,[line=%d],errno=%d:%s\n","pucBuf ipv4 null", __LINE__, errno, strerror(errno));
        //err_print();
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }

    struct sockaddr_in stSin4;

    bzero(&stSin4, sizeof(stSin4));
    stSin4.sin_family = AF_INET;
    stSin4.sin_port = htons(usPort);
    stSin4.sin_addr.s_addr = inet_addr(pcIP);

    if (iBlock) {
        return sendto(iSockfd, pucBuf, iLen, 0, (struct sockaddr *)&stSin4, sizeof(struct sockaddr));
    } else {
        return sendto(iSockfd, pucBuf, iLen, MSG_DONTWAIT, (struct sockaddr *)&stSin4, sizeof(struct sockaddr));
    }
}

/*
 * 功能：UDP-ipv4发送数据
 * 参数：@iSockfd[in]	套接字
 *		@pucBuf[in]		发送数据缓冲区
 *		@iLen[in]		发送数据长度
 *		@pcIP[in]		目的IP地址
 *		@usPort[in]		目的端口号
 *		@iBlock[in]		是否阻塞，1表示阻塞，0表示非阻塞
 * 返回值：成功返回发送数据长度，失败返回错误码
 * 备注：dstAddr可以为NULL，dstLen可以为0
 */
int NSP_TDYTH_4G_ANET_ipv4_send_back(int iSockfd, unsigned char *pucBuf, int iLen, struct sockaddr_in *pstDstAddr, socklen_t dstLen, int iBlock)
{
    if ((pucBuf == NULL) || (iLen <= 0) || (pstDstAddr == NULL) || (dstLen <= 0))
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;

    if (iBlock) {
        return sendto(iSockfd, pucBuf, iLen, 0, (struct sockaddr *)pstDstAddr, dstLen);
    } else {
        return sendto(iSockfd, pucBuf, iLen, MSG_DONTWAIT, (struct sockaddr *)pstDstAddr, dstLen);
    }
}

/*
 * 功能：UDP-ipv4接收数据
 * 参数：@iSockfd[in]	套接字
 *		@pucBuf[in]		接收数据缓冲区
 *		@iLen[in]		接收数据缓冲区长度
 *		@pstSrcAddr[in]	客户端socket
 *		@srcLen[in]		客户端socket长度
 *		@iBlock[in]		是否阻塞，1表示阻塞，0表示非阻塞
 * 返回值：成功返回接受数据长度，失败返回错误码
 * 备注：dstAddr可以为NULL，dstLen可以为0
 */
int NSP_TDYTH_4G_ANET_ipv4_recv(int iSockfd, unsigned char *pucBuf, int iLen, struct sockaddr_in *pstSrcAddr, socklen_t srcLen, int iBlock)
{
    int iRet;

    if ((pucBuf == NULL) || (iLen < 0))
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;

    if (iBlock) {
        iRet = recvfrom(iSockfd, pucBuf, iLen, 0, (struct sockaddr *)pstSrcAddr, &srcLen);
    } else {
        iRet = recvfrom(iSockfd, pucBuf, iLen, MSG_DONTWAIT, (struct sockaddr *)pstSrcAddr, &srcLen);
    }

    if (iRet == -1) {
        iLen = 0;
        if(errno == EAGAIN)
            return D_NSP_TDYTH_NAAAS_4G_ERR_NET_RECV_TIMEOUT;

        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_RECV_ERROR;
    }

    iLen = iRet;
    return iRet;
}

/******ipv6******/

/*
 * 功能：UDP-ipv6接收初始化
 * 参数：@usPort[in]，端口号
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv6_recv_init(unsigned short usPort, struct sockaddr_in6 *pstSin6)
{
    int iSockfd;
    struct timeval stTimeOut;

    iSockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(iSockfd == -1) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv6 socket", __LINE__, errno, strerror(errno));
        //err_print();
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_SOCKET;
    }

    stTimeOut.tv_sec = SOCK_RECV_TIMEOUT;
    stTimeOut.tv_usec = 0;
    if (setsockopt(iSockfd, SOL_SOCKET, SO_RCVTIMEO, &stTimeOut, sizeof(struct timeval)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv6 socket", __LINE__, errno, strerror(errno));
        //err_print();
        close(iSockfd);
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_SETSOCKETOPT;
    }

    bzero(pstSin6, sizeof(struct sockaddr_in6));
    pstSin6->sin6_family = AF_INET6;
    pstSin6->sin6_port = htons(usPort);
    pstSin6->sin6_addr = in6addr_any;

    if(bind(iSockfd, (struct sockaddr *)pstSin6, sizeof(struct sockaddr)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv6 bind", __LINE__, errno, strerror(errno));
        //err_print();
        close(iSockfd);
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
    }

    return iSockfd;
}

/*
 * 功能：UDP-ipv6发送初始化
 * 参数：
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv6_send_init()
{
    return socket(AF_INET6, SOCK_DGRAM, 0);
}

/*
 * 功能：UDP-ipv6发送初始化
 * 参数：@usPort[in]，端口号
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv6_send_init_ex(unsigned short usPort)
{
    int iSockfd;
    struct sockaddr_in6 stSin6;

    iSockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if(iSockfd == -1) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv6 socket", __LINE__, errno, strerror(errno));
        //err_print();
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_SOCKET;
    }

    bzero(&stSin6, sizeof(stSin6));
    stSin6.sin6_family = AF_INET6;
    stSin6.sin6_port = htons(usPort);
    stSin6.sin6_addr = in6addr_any;

    if(bind(iSockfd, (struct sockaddr *)&stSin6, sizeof(struct sockaddr)) < 0) {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv6 bind", __LINE__, errno, strerror(errno));
        //err_print();
        close(iSockfd);
        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
    }

    return iSockfd;
}

/*
 * 功能：UDP-ipv6发送数据
 * 参数：@iSockfd[in]	套接字
 *		@pucBuf[in]		发送数据缓冲区
 *		@iLen[in]		发送数据长度
 *		@pcIP[in]		目的IP地址
 *		@usPort[in]		目的端口号
 *		@iBlock[in]		是否阻塞，1表示阻塞，0表示非阻塞
 * 返回值：成功返回发送数据长度，失败返回错误码
 * 备注：dstAddr可以为NULL，dstLen可以为0
 */
int NSP_TDYTH_4G_ANET_ipv6_send(int iSockfd, unsigned char *pucBuf, int iLen, char *pcIP, unsigned short usPort, int iBlock)
{
    if ((pucBuf == NULL) || (iLen <= 0)) {
        printf("%s,[line=%d],errno=%d:%s\n","pucBuf ipv6 null", __LINE__, errno, strerror(errno));
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }

    struct sockaddr_in6 stSin6;

    bzero(&stSin6, sizeof(stSin6));
    stSin6.sin6_family = AF_INET6;
    stSin6.sin6_port = htons(usPort);
    stSin6.sin6_addr = in6addr_any;

    if (iBlock) {
        return sendto(iSockfd, pucBuf, iLen, 0, (struct sockaddr *)&stSin6, sizeof(struct sockaddr));
    } else {
        return sendto(iSockfd, pucBuf, iLen, MSG_DONTWAIT, (struct sockaddr *)&stSin6, sizeof(struct sockaddr));
    }
}

/*
 * 功能：UDP-ipv6发送数据
 * 参数：@iSockfd[in]	套接字
 *		@pucBuf[in]		发送数据缓冲区
 *		@iLen[in]		发送数据长度
 *		@pcIP[in]		目的IP地址
 *		@usPort[in]		目的端口号
 *		@iBlock[in]		是否阻塞，1表示阻塞，0表示非阻塞
 * 返回值：成功返回发送数据长度，失败返回错误码
 * 备注：dstAddr可以为NULL，dstLen可以为0
 */
int NSP_TDYTH_4G_ANET_ipv6_send_back(int iSockfd, unsigned char *pucBuf, int iLen, struct sockaddr_in6 *pstDstAddr, socklen_t dstLen, int iBlock)
{
    if ((pucBuf == NULL) || (iLen <= 0) || (pstDstAddr == NULL) || (dstLen <= 0))
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;

    if (iBlock) {
        return sendto(iSockfd, pucBuf, iLen, 0, (struct sockaddr *)pstDstAddr, dstLen);
    } else {
        return sendto(iSockfd, pucBuf, iLen, MSG_DONTWAIT, (struct sockaddr *)pstDstAddr, dstLen);
    }
}

/*
 * 功能：UDP-ipv6接收数据
 * 参数：@iSockfd[in]	套接字
 *		@pucBuf[in]		发送数据缓冲区
 *		@iLen[in]		发送数据长度
 *		@pstSrcAddr[in]	客户端socket
 *		@srcLen[in]		客户端socket长度
 *		@iBlock[in]		是否阻塞，1表示阻塞，0表示非阻塞
 * 返回值：成功返回接受数据长度，失败返回错误码
 * 备注：dstAddr可以为NULL，dstLen可以为0
 */
int NSP_TDYTH_4G_ANET_ipv6_recv(int iSockfd, unsigned char *pucBuf, int iLen, struct sockaddr_in6 *pstSrcAddr, socklen_t srcLen, int iBlock)
{
    int iRet;

    if ((pucBuf == NULL) || (iLen < 0))
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;

    if (iBlock) {
        iRet = recvfrom(iSockfd, pucBuf, iLen, 0, (struct sockaddr *)pstSrcAddr, &srcLen);
    } else {
        iRet = recvfrom(iSockfd, pucBuf, iLen, MSG_DONTWAIT, (struct sockaddr *)pstSrcAddr, &srcLen);
    }

    if (iRet == -1) {
        iLen = 0;
        if(errno == EAGAIN)
            return D_NSP_TDYTH_NAAAS_4G_ERR_NET_RECV_TIMEOUT;

        return D_NSP_TDYTH_NAAAS_4G_ERR_NET_RECV_ERROR;
    }

    iLen = iRet;
    return iRet;
}
/************************************************************************新UDP封装*******************************************************************/
/*
 * 函数功能：UDP发送数据
 * 参数：  @iSockfd[in]：   套接字
 *          @pucBuf[in]：        发送数据缓冲区
 *          @iLen[in]：      发送数据长度
 *          @pcIP[in]    目的地IP地址
 *          @usPort[in]     目的地端口
 *          @iBlock[in]：    是否阻塞，1表示阻塞，0表示不阻塞
 * 返回值： 成功返回发送数据长度，失败返回错误码
 * 备注： 
 */
int F_NSP_TDYTH_NAAAS_ComSendData(int iSockfd, unsigned char *pucBuf, int iLen, char *pcIP, unsigned short usPort, int iBlock)
{
    if ((pucBuf == NULL) || (iLen <= 0)){
//    if((NULL == pucBuf) ||(0 >= iLen)){
        return -D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    }

    struct sockaddr_in stAddr;
    stAddr.sin_family = AF_INET;
    stAddr.sin_port = htons(usPort);
    stAddr.sin_addr.s_addr = inet_addr(pcIP);

    if (iBlock){
        return sendto(iSockfd, pucBuf, iLen, 0, (struct sockaddr *) &stAddr, sizeof(struct sockaddr));
    }
    else{
        return sendto(iSockfd, pucBuf, iLen, MSG_DONTWAIT, (struct sockaddr *) &stAddr, sizeof(struct sockaddr));
    }
}

/*
 * 功能：setnonblocking – 设置句柄为非阻塞方式
 * 参数：  sock[in]     套接字
 * 返回值：成功返回1,失败返回-1
 * 备注：
 */
int setnonblocking(int sock)
{
    int opts;
    opts=fcntl(sock,F_GETFL);

    if(opts<0)

    {
        perror("fcntl(sock,GETFL)");
        return -1;
    }

    opts = opts|O_NONBLOCK;
    if(fcntl(sock,F_SETFL,opts)<0)
    {
        perror("fcntl(sock,SETFL,opts)");
        return -1;
    } 
    return 1;   
}

/************************************************************************前端展示相应功能*******************************************************************/

/*
 * 功能：将信息传递给守护进程转发给UE前端的端口
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int printf_dae_uesend(const char *c_pcFormat, ...)
{
    int iSocketfd = socket(AF_INET, SOCK_DGRAM, 0);
    int iRet;
    va_list stArgs;
    char acBuff[D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN];
    int iBuffLen;

    va_start(stArgs, c_pcFormat);
    iBuffLen = vsprintf(acBuff, c_pcFormat, stArgs);
    va_end(stArgs);

    iRet = F_NSP_TDYTH_NAAAS_ComSendData(iSocketfd, (unsigned char *)acBuff, iBuffLen, D_NSP_TDYTH_NAAAS_DAEMON_IP, D_NSP_TDYTH_NAAAS_DAEMONUE_PORT, 1);
    close(iSocketfd);
    return iRet;
}

/*
 * 功能：将信息传递给守护进程转发给MME前端的端口
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int printf_dae_mmesend(const char *c_pcFormat, ...)
{
    int iSocketfd = socket(AF_INET, SOCK_DGRAM, 0);
    int iRet;
    va_list stArgs;
    char acBuff[D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN];
    int iBuffLen;

    va_start(stArgs, c_pcFormat);
    iBuffLen = vsprintf(acBuff, c_pcFormat, stArgs);
    va_end(stArgs);

    iRet = F_NSP_TDYTH_NAAAS_ComSendData(iSocketfd, (unsigned char *)acBuff, iBuffLen, D_NSP_TDYTH_NAAAS_DAEMON_IP, D_NSP_TDYTH_NAAAS_DAEMONMME_PORT, 1);

    close(iSocketfd);
    return iRet;
}
/************************************************************************业务封装*******************************************************************/
/*
 * 功能：DNS解析相应域名到IP
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int NSP_TDYTH_NAAAS_4G_ANET_dns(char *pcWeburl, char *pcDnsIp)
{
    struct hostent *h_pcWeburl;
    int i;

    h_pcWeburl = gethostbyname(pcWeburl);
    if(h_pcWeburl == NULL){        
        return -1;
    }

    for (i = 0; h_pcWeburl->h_addr_list[i]; i++) {
        printf("%s 域名DNS解析: %s \n", pcWeburl, inet_ntoa(*((struct in_addr*)h_pcWeburl->h_addr_list[i])));
    }

   sprintf(pcDnsIp,"%s",inet_ntoa(*((struct in_addr*)h_pcWeburl->h_addr_list[0])));
   printf("选取此IP:%s\n", pcDnsIp);

    return 0;
}