/*
 * 文件功能：4G接入认证调试打印函数
 * 作者：dk
 * 创建日期：2020/02/13
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#ifndef NSP_TDYTH_NAAAS_4G_DEBUG_H
#define NSP_TDYTH_NAAAS_4G_DEBUG_H

#ifdef Debug
void debug_print(char *info,unsigned char *data,int len);
#else
#define debug_print(...)
#endif

#ifdef ErrInfo
void err_print(char *info,int line,int err);
#else
#define err_print(...)
#endif

#endif //NSP_TDYTH_NAAAS_4G_DEBUG_H

int kbhit();