/*
 * 文件功能：4G接入认证协议工具类文件
 * 作者：董珂 李高明
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#ifndef NSP_TDYTH_NAAAS_4G_UTILS_H
#define NSP_TDYTH_NAAAS_4G_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "NSP_TDYTH_NAAAS_4G_common.h"
#include "NSP_TDYTH_NAAAS_cJSON.h"
    #include "NSP_TDYTH_NAAAS_4G_proto.h"


/*
 * 功能：SIP打印函数
 * 参数：    @pucAddr[in]		待打印数据指针
 * 			@uiLen[in]		待打印数据长度
 * 返回值：  无
 */
void NSP_TDYTH_4G_printSIP(St_NSP_TDYTH_NAAAS_4G_SIPHeader *pucAddr,unsigned int uiLen);
/*
 * 功能：16进制打印函数
 * 参数：    @pucAddr[in]		待打印数据指针
 * 			@uiLen[in]		待打印数据长度
 * 返回值：  无
 */
void NSP_TDYTH_4G_print(char *pcInfo, unsigned char *pucAddr, unsigned int uiLen);

/*
 * 功能：获取当前时间戳
 * 参数：无
 * 返回值：当前时间戳
 */
time_t NSP_TDYTH_4G_get_time_now();

/*
 * 功能：验证时间戳
 * 参数：
 *        @timeStamp      时间戳
 * 返回值：
 *         0 合法
 *        -1 非法
 */
int NSP_TDYTH_4G_verification_timestamp(time_t timeStamp);
/*
 * 功能：获取当前时间YYMM-MM-DD
 * 参数：无
 * 返回值：当前时间
 */
void NSP_TDYTH_4G_get_time();

/*
 * 功能：异或函数
 * 参数：    @pucP1[in]		待异或数据1指针
 * 			@pucP2[in]		待异或数据2指针
 * 			@iLen[in]       待异或数据长度
 * 			@pucRet[out]    待异或数据1和待异或数据2异或之后结果
 * 返回值：   0         成功
 *          非0        错误码
 */
int NSP_TDYTH_4G_XOR(unsigned char *pucP1, unsigned char *pucP2, int iLen, unsigned char *pucRet);

/*
 * 功能：读取文件某一行内容
 * 参数：
 *          @fp   [in]      文件指针
 *          @iline[in]      文件行数
 *          @pcstr[in]      读取内容
 * 返回值：   > 0        成功(返回读取长度)
 *           - 1        错误(没有此行)
 * 备注：应该在每次循环开始时清空一下pcstr
 */
int NSP_TDYTH_4G_getlinetxt(FILE *fp, int iline,unsigned char * pcstr);

/*
 * 功能：读取文件某一行内容(包含打开文件)
 * 参数：
 *          @cfilename   [in]      文件路径
 *          @iline       [in]      文件行数
 *          @pcstr       [in]      读取内容
 * 返回值：
 *            >= 0        成功
 *            <  1        错误
 * 备注：
 */
int NSP_TDYTH_4G_getfiletxt(char *cfilename, int iline,unsigned char * pcstr);

/*
 * 功能：将信息记录在日志中(当前目录)
 * 参数：    
 *            变参函数
 * 返回值：   
 *            >= 0        成功
 *            <  0        错误
 * 备注：
 */
int NSP_TDYTH_4G_WriteLog(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif //NSP_TDYTH_NAAAS_4G_UTILS_H
