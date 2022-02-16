/*
 * 文件功能：4G接入认证调试打印函数
 * 作者：dk
 * 创建日期：2020/02/13
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include <stdio.h>
#include <string.h>
#include "NSP_TDYTH_NAAAS_4G_debug.h"


#ifdef Debug
void debug_print(char *info,unsigned char *data,int len)
{
    int i;

    printf("%s", info);
    for(i = 0; i < len; i++)
    {
        if(i%16 == 0)
            printf("\n");
        printf("%02x ", data[i]);

    }
    printf("\n");
}
#endif

#ifdef ErrInfo
void err_print(char *info, int line, int err)
{
    printf("%s,[line=%d],errno=%d:%s\n", info, line, err, strerror(err));
}
#endif

// 键盘输入检测程序
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        //ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
