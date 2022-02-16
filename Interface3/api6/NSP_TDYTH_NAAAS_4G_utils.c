/*
 * 文件功能：4G接入认证协议工具类文件
 * 作者：董珂 李高明 李紫妍 李泽宇
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_utils.h"
#include "NSP_TDYTH_NAAAS_4G_proto.h"

/*
 * 功能：SIP打印函数
 * 参数：    @pucAddr[in]		待打印数据指针
 * 			@uiLen[in]		待打印数据长度
 * 返回值：  无
 */
void NSP_TDYTH_4G_printSIP(St_NSP_TDYTH_NAAAS_4G_SIPHeader *pucAddr,unsigned int uiLen){

    printf("   From: %s\n",pucAddr->aucFrom);
    printf("     T0: %s\n",pucAddr->aucTo);
    printf("Call_ID: %s\n",pucAddr->aucCall_ID);
    printf("    Seq: %s\n",pucAddr->aucCSeq);
    printf("    Via: %s\n",pucAddr->aucVia);
    printf("    TTL: %d\n",pucAddr->uiMax_Forwards);

}


/*
 * 功能：16进制打印函数
 * 参数：    @pucAddr[in]		待打印数据指针
 * 			@uiLen[in]		待打印数据长度
 * 返回值：  无
 */
void NSP_TDYTH_4G_print(char *pcInfo, unsigned char *pucAddr, unsigned int uiLen)
{
    int i;

    printf("%s:\n", pcInfo);
    for (i = 0; i < uiLen; i++)
    {
        if(i && i % 16 == 0)
            puts("");
        printf("%02x ", pucAddr[i]);
    }
    puts("");
}

/*
 * 功能：获取当前时间戳
 * 参数：无
 * 返回值：当前时间戳
 */
time_t NSP_TDYTH_4G_get_time_now()
{
    return time((time_t *)NULL);
}
/*
 * 功能：验证时间戳
 * 参数：
 *        @timeStamp      时间戳
 * 返回值：
 *         0 合法
 *        -1 非法
 */

 int NSP_TDYTH_4G_verification_timestamp(time_t timeStamp) 
 {
    time_t tmpTime = time(NULL);
    unsigned char * tmp=(unsigned char*)&tmpTime;
    if (fabs(difftime(timeStamp, tmpTime)) <= D_NSP_TDYTH_NAAAS_4G_LEGAL_TIME_INTERVAL)
    {
        return 0;
    }
    return -1;
}

//int verification_timestampForullT4(unsigned char* timeStamp)
//{
//     // 将标准格式转为time_t来验证时间戳
//     struct tm* tmp_time = (struct tm*)malloc(sizeof(struct tm));
//     strptime(timeStamp, "%a %b  %d %H:%M:%S %Y", tmp_time);
//     time_t t = mktime(tmp_time);
//
//     time_t tmpTime = time(NULL);
//     unsigned char * tmp=(unsigned char*)&tmpTime;
//     if (fabs(difftime(t, tmpTime)) <= D_NSP_TDYTH_NAAAS_4G_LEGAL_TIME_INTERVAL) {
//         return -1;
//     }
//     return 0;
//}

/*
 * 功能：获取时间戳（数组形式）
 * 参数：
 *        @buffer      时间数组32位
 * 返回值：
 */
void get_timestamp(char *buffer) {
    time_t t;
    struct tm *p;
    struct timeval tv;
    int len;
    int millsec;

    t = time(NULL);
    p = localtime(&t);

    gettimeofday(&tv, NULL);
    millsec = (int) (tv.tv_usec / 1000);

    len = snprintf(buffer, 32, "[%04d-%02d-%02d %02d:%02d:%02d:%03d]",
                   p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min,
                   p->tm_sec, millsec);
    buffer[len] = '\0';
    printf("%s",buffer);
}

/*
 * 功能：异或函数
 * 参数：    @pucP1[in]		待异或数据1指针
 * 			@pucP2[in]		待异或数据2指针
 * 			@iLen[in]       待异或数据长度
 * 			@pucRet[out]    待异或数据1和待异或数据2异或之后结果
 * 返回值：   0         成功
 *          非0        错误码
 */
int NSP_TDYTH_4G_XOR(unsigned char *pucP1, unsigned char *pucP2, int iLen, unsigned char *pucRet)
{
    int i;

    if(pucP1 == NULL || pucP2 == NULL || pucRet == NULL)
        return D_NSP_TDYTH_NAAAS_4G_ERR_BUFF_NULL;
    
    for(i = 0; i < iLen; i++)
    {
        pucRet[i] = pucP1[i] ^ pucP2[i];
    }
    return D_NSP_TDYTH_NAAAS_4G_SUCCESS;
}

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
int NSP_TDYTH_4G_getlinetxt(FILE *fp, int iline,unsigned char * pcstr)
{
    fseek(fp,0,0);
    for (int i = 0; i < iline; ++i) 
    {
        if(fgets(pcstr, D_NSP_TDYTH_NAAAS_4G_BUFF_LEN, fp)==NULL)
        {
            printf("行数不存在！\n");
            return -1;
        }
    }
    return strlen(pcstr);
}
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
int NSP_TDYTH_4G_getfiletxt(char *cfilename, int iline,unsigned char * pcstr)
{
    FILE * fp;
    if((fp = fopen(cfilename, "r")) == NULL)
    {
        return -1;
    }
    return NSP_TDYTH_4G_getlinetxt(fp, iline, pcstr);
    fclose(fp);
    return 0;
}

/*
 * 功能：将信息记录在日志中(当前目录)
 * 参数：    
 *            变参函数
 * 返回值：   
 *            >= 0        成功
 *            <  0        错误
 * 备注：
 */
int NSP_TDYTH_4G_WriteLog(const char *format, ...)
{
    FILE* pFile = fopen("../log.txt","a+");
    if(!pFile)
    {
        printf("文件打开失败或创建失败(请确定路径)!\n");
        return -1;
    }
    va_list arg;
    int iNum;
    va_start (arg, format);
    time_t time_log = time(NULL);
    struct tm* tm_log = localtime(&time_log);
    //打印时间
    fprintf(pFile, "%04d-%02d-%02d %02d:%02d:%02d\n", tm_log->tm_year + 1900, tm_log->tm_mon + 1, tm_log->tm_mday, tm_log->tm_hour, tm_log->tm_min, tm_log->tm_sec);

    iNum = vfprintf(pFile, format, arg);
    va_end (arg);

    fflush(pFile);
    fclose(pFile);
    return iNum;
}

