/*
 * 文件功能：4G接入认证协议UDP相关文件
 * 作者：董珂
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#ifndef NSP_TDYTH_NAAAS_4G_ANET_H
#define NSP_TDYTH_NAAAS_4G_ANET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include "NSP_TDYTH_NAAAS_4G_common.h"

/******ipv4******/

/*
 * 功能：UDP-ipv4接收初始化
 * 参数：@usPort[in]，端口号
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv4_recv_init(unsigned short usPort, struct sockaddr_in *pstSin4);

/*
 * 功能：UDP-ipv4发送初始化
 * 参数：
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv4_send_init();

/*
 * 功能：UDP-ipv4发送初始化
 * 参数：@usPort[in]，端口号
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv4_send_init_ex(unsigned short usPort);

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
int NSP_TDYTH_4G_ANET_ipv4_send(int iSockfd, unsigned char *pucBuf, int iLen, char *pcIP, unsigned short usPort, int iBlock);

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
int NSP_TDYTH_4G_ANET_ipv4_send_back(int iSockfd, unsigned char *pucBuf, int iLen, struct sockaddr_in *pstDstAddr, socklen_t dstLen, int iBlock);

/*
 * 功能：UDP-ipv4接收数据
 * 参数：@iSockfd[in]	套接字
 *		@pucBuf[in]		发送数据缓冲区
 *		@iLen[in]		发送数据长度
 *		@pstSrcAddr[in]	客户端socket
 *		@srcLen[in]		客户端socket长度
 *		@iBlock[in]		是否阻塞，1表示阻塞，0表示非阻塞
 * 返回值：成功返回接受数据长度，失败返回错误码
 * 备注：dstAddr可以为NULL，dstLen可以为0
 */
int NSP_TDYTH_4G_ANET_ipv4_recv(int iSockfd, unsigned char *pucBuf, int iLen, struct sockaddr_in *pstSrcAddr, socklen_t srcLen, int iBlock);

/******ipv6******/

/*
 * 功能：UDP-ipv6接收初始化
 * 参数：@usPort[in]，端口号
 * 返回值：成功返回套接字fd，失败返回错误码
 */
int NSP_TDYTH_4G_ANET_ipv6_recv_init(unsigned short usPort, struct sockaddr_in6 *pstSin4);

/*
 * 功能：UDP-ipv6发送初始化
 * 参数：
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv6_send_init();

/*
 * 功能：UDP-ipv6发送初始化
 * 参数：@usPort[in]，端口号
 * 返回值：成功 套接字fd
 *        失败 错误码
 */
int NSP_TDYTH_4G_ANET_ipv6_send_init_ex(unsigned short usPort);

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
int NSP_TDYTH_4G_ANET_ipv6_send(int iSockfd, unsigned char *pucBuf, int iLen, char *pcIP, unsigned short usPort, int iBlock);

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
int NSP_TDYTH_4G_ANET_ipv6_send_back(int iSockfd, unsigned char *pucBuf, int iLen, struct sockaddr_in6 *pstDstAddr, socklen_t dstLen, int iBlock);

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
int NSP_TDYTH_4G_ANET_ipv6_recv(int iSockfd, unsigned char *pucBuf, int iLen, struct sockaddr_in6 *pstSrcAddr, socklen_t srcLen, int iBlock);

/*
 * 函数功能：UDP发送数据
 * 参数：  @iSockfd[in]：   套接字
 *          @pucBuf[in]：        发送数据缓冲区
 *          @iLen[in]：      发送数据长度
 *          @pcIP[in]    目的地IP地址
 *          @usPort[in]     目的地端口
 *          @iBlock[in]：    是否阻塞，1表示阻塞，0表示不阻塞
 * 返回值： 成功返回发送数据长度，失败返回错误码
 * 备注：  dstAddr可以为NULL，dstLen可以为0
 */
int F_NSP_TDYTH_NAAAS_ComSendData(int iSockfd, unsigned char *pucBuf, int iLen, char *pcIP, unsigned short usPort, int iBlock);


/*
 * 功能：setnonblocking – 设置句柄为非阻塞方式
 * 参数：  sock[in]     套接字
 * 返回值：成功返回1,失败返回-1
 * 备注：
 */
int setnonblocking(int sock);

/*
 * 功能：将信息传递给守护进程转发给UE前端的端口
 * 参数：
 * 返回值：
 *          <  0        失败
 *          >= 0        成功
 */
int printf_dae_uesend(const char *c_pcFormat, ...);

/*
 * 功能：将信息传递给守护进程转发给MME前端的端口
 * 参数：
 * 返回值：
 *          <  0        失败
 *          >= 0        成功
 */
int printf_dae_mmesend(const char *c_pcFormat, ...);

/*
 * 功能：DNS解析相应域名到IP
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int NSP_TDYTH_NAAAS_4G_ANET_dns(char *pcWeburl, char *pcDnsIp);

#ifdef __cplusplus
}
#endif

#endif //NSP_TDYTH_NAAAS_4G_ANET_H
