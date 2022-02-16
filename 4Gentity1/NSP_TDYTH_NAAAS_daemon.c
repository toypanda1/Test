/*
 * 文件功能：守护进程
 * 作者： 李高明 
 * 创建日期：2020/06/29
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_4G_anet.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"
#include "NSP_TDYTH_NAAAS_4G_debug.h"

static int gs_iSocketFd;											//通信套接字

int printf_uesend(const char *c_pcFormat, ...)
{
	va_list stArgs;
	char acBuff[D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN];
	int iBuffLen;

	va_start(stArgs, c_pcFormat);
	iBuffLen = vsprintf(acBuff, c_pcFormat, stArgs);
	va_end(stArgs);
	
	return F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, (unsigned char *)acBuff, iBuffLen, D_NSP_TDYTH_NAAAS_UEFRONT_IP, D_NSP_TDYTH_NAAAS_UEFRONT_PORT, 1);
}

int printf_mmesend(const char *c_pcFormat, ...)
{
	va_list stArgs;
	char acBuff[D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN];
	int iBuffLen;

	va_start(stArgs, c_pcFormat);
	iBuffLen = vsprintf(acBuff, c_pcFormat, stArgs);
	va_end(stArgs);
	
	return F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, (unsigned char *)acBuff, iBuffLen, D_NSP_TDYTH_NAAAS_MMEFRONT_IP, D_NSP_TDYTH_NAAAS_MMEFRONT_PORT, 1);
}

/*
 * 功能：开机
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int ue_poweron(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 开机要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：关机
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int ue_poweroff(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 关机要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：打电话功能
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int ue_phone(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 打电话要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：重启
 * 参数：   
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int ue_restart(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 重启要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;	
}

/*
 * 功能：转发给UE，进行注册功能
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int ue_register(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 注册要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：转发给UE，进行地理位置定位
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int ue_location(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 定位要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

int ue_attach(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 附着要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：转发给UE，进行读卡
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int ue_readsim(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 读卡要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：转发给MME，进行用户身份请求
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int ue_measurereport(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_UE_IP, D_NSP_TDYTH_NAAAS_UEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->UE 测量报告要求发送!\n");
        printf_uesend("UE后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}



/*
 * 功能：转发给MME，进行认证
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int mme_auth(unsigned char *pcRecvBuf)
{
	int iRet;

	//后续应该添加是否已经进行了IP分配与认证，再决定是否打电话

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MMEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->MME 认证要求发送!\n");
        printf_mmesend("MME后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}
/*
 * 功能：转发给MME，进行IP分配
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int mme_ipallot(unsigned char *pcRecvBuf)
{
	int iRet;

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MMEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->MME IP分配要求发送!\n");
        printf_mmesend("MME后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：转发给MME，进行中断连接
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int mme_linkbreak(unsigned char *pcRecvBuf)
{
	int iRet;

	//后续应该添加是否已经进行了IP分配与认证，再决定是否打电话

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MMEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->MME 中断连接要求发送!\n");
        printf_mmesend("MME后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：转发给MME，进行恢复连接
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int mme_linkrecover(unsigned char *pcRecvBuf)
{
	int iRet;

	//后续应该添加是否已经进行了IP分配与认证，再决定是否打电话

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MMEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->MME 恢复连接要求发送!\n");
        printf_mmesend("MME后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：转发给MME，在线用户信息展示
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int mme_onlineuserinfo(unsigned char *pcRecvBuf)
{
	int iRet;

	//后续应该添加是否已经进行了IP分配与认证，再决定是否打电话

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MMEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->MME 在线用户信息展示已发送!\n");
        printf_mmesend("MME后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}


/*
 * 功能：转发给MME，网络拓扑信息展示
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int mme_netinfo(unsigned char *pcRecvBuf)
{
	int iRet;

	//后续应该添加是否已经进行了IP分配与认证，再决定是否打电话

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MMEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->MME 网络拓扑信息展示已发送!\n");
        printf_mmesend("MME后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}

/*
 * 功能：转发给MME，注册用户信息展示
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int mme_reguserinfo(unsigned char *pcRecvBuf)
{
	int iRet;

	//后续应该添加是否已经进行了IP分配与认证，再决定是否打电话

	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, D_NSP_TDYTH_NAAAS_MME1_IP, D_NSP_TDYTH_NAAAS_MMEF_PORT, 1);
    if(iRet > 0)
    {
        printf("Daemon->MME 注册用户信息展示已发送!\n");
        printf_mmesend("MME后台启动！\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }

	return 0;
}
/*
 * 功能：将后台消息转发给前端
 * 参数：   
 *         @pcRecvBuf   [in]   收到前端的指令
 *         @pcIP        [in]   需要转发前端的IP
 *         @usPort      [in]   需要转发前端的端口
 * 返回值：  
 *          <  0        失败 
 *          >= 0        成功
 */
int back_transport(unsigned char *pcRecvBuf, char *pcIP, unsigned short usPort)
{
	int iRet;
	iRet = F_NSP_TDYTH_NAAAS_ComSendData(gs_iSocketFd, pcRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, pcIP, usPort, 1);
	if(iRet > 0)
    {
        printf("转发消息给前端success!\n");
    }else
    {
        printf("%s,[line=%d],errno=%d:%s\n", "Daemon:UDP ipv4 send err", __LINE__, errno, strerror(errno));
    }
    return 0;
}

int main()
{

	unsigned char aucBuff[D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN];
	unsigned char cRecvBuf[D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN];
    unsigned char cSendBuf[D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN];
	int iRet;
	int iErr;

    gs_iSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    // Daemon接收前端
    struct sockaddr_in stDAESockaddr;
    bzero(&stDAESockaddr, sizeof(stDAESockaddr));
    stDAESockaddr.sin_family = AF_INET;
    stDAESockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_DAEMON_PORT);
    stDAESockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_DAEMON_IP);

    int iSocketUE = socket(AF_INET, SOCK_DGRAM, 0);
    // Daemon接收UE后台
    struct sockaddr_in stUEBackSockaddr;
    bzero(&stUEBackSockaddr, sizeof(stUEBackSockaddr));
    stUEBackSockaddr.sin_family = AF_INET;
    stUEBackSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_DAEMONUE_PORT);
    stUEBackSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_DAEMON_IP);

    int iSocketMME = socket(AF_INET, SOCK_DGRAM, 0);
    // Daemon接收MME后台
    struct sockaddr_in stMMEBackSockaddr;
    bzero(&stMMEBackSockaddr, sizeof(stMMEBackSockaddr));
    stMMEBackSockaddr.sin_family = AF_INET;
    stMMEBackSockaddr.sin_port = htons(D_NSP_TDYTH_NAAAS_DAEMONMME_PORT);
    stMMEBackSockaddr.sin_addr.s_addr = inet_addr(D_NSP_TDYTH_NAAAS_DAEMON_IP);

    if (bind(gs_iSocketFd, (struct sockaddr *) &stDAESockaddr, sizeof(stDAESockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        return -D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
    }

     if (bind(iSocketUE, (struct sockaddr *) &stUEBackSockaddr, sizeof(stUEBackSockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        return -D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
    }

    if (bind(iSocketMME, (struct sockaddr *) &stMMEBackSockaddr, sizeof(stMMEBackSockaddr)) == -1)
    {
        printf("%s,[line=%d],errno=%d:%s\n","UDP ipv4 bind", __LINE__, errno, strerror(errno));
        return -D_NSP_TDYTH_NAAAS_4G_ERR_NET_BIND;
    }

    int iSinLen = sizeof(stDAESockaddr);
    int iSinBackLen = sizeof(stUEBackSockaddr);

	while(1)
	{
		memset(cRecvBuf, 0, sizeof(cRecvBuf));
        memset(aucBuff, 0, sizeof(aucBuff));
        iRet = recvfrom(gs_iSocketFd, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &stDAESockaddr, &iSinLen);
		if(iRet > 0)
		{
			if(strlen(cRecvBuf) > 10)
			{
				printf("命令码长度错误！\n");
				continue;
			}
			memcpy(aucBuff, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_DaemonControl_LEN);

			St_NSP_TDYTH_NAAAS_4G_DaemonControl *pstControl = (St_NSP_TDYTH_NAAAS_4G_DaemonControl *) aucBuff;
			switch(pstControl->uiFunction)
			{
				case D_NSP_TDYTH_NAAAS_4G_UE_POWERON:
					ue_poweron(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_UE_REGISTER:
					ue_register(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_UE_PHONE:
					ue_phone(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_UE_LOCATION:
					ue_location(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_UE_POWEROFF:
					ue_poweroff(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_UE_RESTART:
					ue_restart(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_UE_ATTACH:
					ue_attach(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_UE_SWITCH:
					ue_measurereport(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_UE_SIMINFO:
					ue_readsim(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_MME_REGUSERINFO:
					mme_reguserinfo(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_MME_AUTHBEGIN:
					mme_auth(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_MME_IPALLOT:
					mme_ipallot(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_MME_ONLINEUSERINFO:
					mme_onlineuserinfo(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_MME_NETTOPOLOGY:
					mme_netinfo(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_MME_LINKBREAK:
					mme_linkbreak(cRecvBuf);
					break;
				case D_NSP_TDYTH_NAAAS_4G_MME_LINKRECOVER:
					mme_linkrecover(cRecvBuf);
					break;
				default:
					printf("命令码格式错误！\n");
					break;
			}
		}
		memset(cRecvBuf, 0, sizeof(cRecvBuf));
		iRet = recvfrom(iSocketUE, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &stUEBackSockaddr, &iSinBackLen);
		if(iRet > 0)
		{
			back_transport(cRecvBuf, D_NSP_TDYTH_NAAAS_UEFRONT_IP, D_NSP_TDYTH_NAAAS_UEFRONT_PORT);
		}
		memset(cRecvBuf, 0, sizeof(cRecvBuf));
		iRet = recvfrom(iSocketMME, cRecvBuf, D_NSP_TDYTH_NAAAS_4G_DAEMON_BUFF_LEN, MSG_DONTWAIT,(struct sockaddr *) &stMMEBackSockaddr, &iSinBackLen);
		if(iRet > 0)
		{
			back_transport(cRecvBuf, D_NSP_TDYTH_NAAAS_MMEFRONT_IP, D_NSP_TDYTH_NAAAS_MMEFRONT_PORT);
		}
	}

	return 0;
}