/*
 * 文件功能：4G接入认证协议数据库操作相关文件
 * 作者：董珂 丁潇 寇飞翔
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#ifndef TDYTH_NSP_NAAAS_TDYTH_4G_DB_H
#define TDYTH_NSP_NAAAS_TDYTH_4G_DB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <mysql/mysql.h>

#include "NSP_TDYTH_NAAAS_4G_common.h"



/*
 * 结构体描述：数据库信息插入结构体
 * 参数：@tableName 	所要插入的数据库名称
 *		 @fieldKey  	所要插入的字段名称
 *       @fieldValue    所要插入的字段值
 * 修改内容：
 * 修改时间：
 * 修改人：
 */
//struct OldDBInsertInfo {
//    char *tableName;
//    char *fieldKey;
//    char *fieldValue;
//};

//struct DBInsertInfo {
//    char *tableName;
//    char *fieldKey;
//    int port;
//    char *fieldStringValue;
//};

typedef struct {
    char *tableName;
    char *fieldKey;
    int port;
    char *fieldStringValue;
}St_NSP_TDYTH_NAAAS_4G_DBInsertInfo;

//struct DBQueryInfo {
//    char *tableName;
//    char *keyName;
//    char *keyValue;
//    char *keyName2;
//    char *keyName3;
//    char *keyName4;
//    char *limitName;
//    char *limitValue;
//};

//typedef struct DBQueryInfo {
//    char *tableName;
//    char *fieldKey;
//    char *limitName;
//    char *limitValue;
//}St_NSP_TDYTH_NAAAS_4G_DBQueryInfo;

typedef struct {
    char *tableName;
    char *fieldKey;
    char *limitName;
    char *limitValue;
}St_NSP_TDYTH_NAAAS_4G_DBQueryInfo;

//struct DBQueryInfo2 {
//    char *tableName;
//    char *fieldKey;
//    char *limitName;
//    char *limitValue;
//};

//struct DBQueryInfo2 {
//    char *tableName1;
//    char *tableName2;
//    char *limitName1;
//    char *limitName2;
//    char *limitValue;
//};

typedef struct {
    char *tableName1;
    char *tableName2;
    char *limitName1;
    char *limitName2;
    char *limitValue;
}St_NSP_TDYTH_NAAAS_4G_DBJoinQueryInfo;

//struct DBQuerInfo {
//    char *tableName;
//    char *fieldKey;
//    char *limitName;
//    char *limitValue;
//};

//struct HSSDBInsertInfo {
//    char *tableName;
//    char *fieldKey;
//    char *fieldValue;
//};

typedef struct {
    char *tableName;
    char *fieldKey;
    char *fieldValue;
}St_NSP_TDYTH_NAAAS_4G_HSSDBInsertInfo;

//struct DBUpdateInfo {
//    char *tableName;
//    char *fieldKey;
//    char *fieldValue;
//    char *limitName;
//    char *limitValue;
//};

typedef struct {
    char *tableName;
    char *fieldKey;
    char *fieldValue;
    char *limitName;
    char *limitValue;
}St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo;

/*
 * 结构体描述：数据库删除信息结构体
 * 参数：@tableName 	    所要删除的数据库名称
 *		 @limitName	    所要删除的限定字段名称
 *		 @limitValue	所要删除的限定字段值
 * 作者：李泽宇
 */
struct DBDeleteInfo{
    char *tableName;
    char *limitName;
    char *limitValue;
};

/*
 * 功能：连接mysql数据库
 * 参数：
 * 返回值：	MYSQL对象conn_ptr
 */
MYSQL *NSP_TDYTH_4G_DB_connect();

/*
 * 功能：查询单个数据
 * 参数：	  @conn_ptr     mysql句柄
 *            @
 * 返回值：	0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *pstDbQueryInfo, unsigned char *pcSendbuf);

/*
 * 功能：断开mysql数据库
 * 参数：	  @conn_ptr mysql句柄
 * 返回值：
 */
void NSP_TDYTH_4G_DB_close(MYSQL *conn_ptr);

/*
 * 功能：更改数据
 * 参数：    @conn_ptr     mysql句柄
 *            @pstDbInsertInfo  表名、字段名、字段值
 * 返回值： 0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_DB_update(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *pstDbUpdateInfo);

/*
 * 功能：查找IMSI是否在HSS表中
 * 参数：	  @conn_ptr     mysql句柄
 *            @pucIMSI      4G用户身份标识符
 * 返回值：	0   成功
 *          非0 错误码
 */
//int NSP_TDYTH_4G_DB_find_by_IMSI(MYSQL *conn_ptr, char *pucIMSI);


/*
 * 功能：查找SN id是否在HSS表中
 * 参数：	  @conn_ptr     mysql句柄
 *            @pucSNID      服务网络ID
 * 返回值：	0   成功
 *          非0 错误码
 */
//int NSP_TDYTH_4G_DB_find_by_SNID(MYSQL *conn_ptr, unsigned char *pucSNID);


/*
 * 功能：查询数据
 * 参数：	  @conn_ptr     mysql句柄
 *            @
 * 返回值：	0   成功
 *          非0 错误码
 */
//int NSP_TDYTH_4G_DB_select(MYSQL *conn_ptr, struct DBQueryInfo *pstDbQueryInfo);


/*
 * 功能：   查找某元素A在某表T中是否存在
 * 参数：   @conn_ptr【in】     	  mysql句柄
 *          @pstDbQueryInfo【in&out】     数据库查询信息结构体
 * 返回值：	0      成功，即存在该元素
 *          非0    不存在该元素
 * 作者：   寇飞翔
 */
//int NSP_TDYTH_4G_DB_find(MYSQL *conn_ptr, struct DBQueryInfo *pstDbQueryInfo);

/*
 * 功能：插入数据
 * 参数：	  @conn_ptr     mysql句柄
 *            @pstDbInsertInfo  表名、字段名、字段值
 * 返回值：	0   成功
 *          非0 错误码
 */
//int NSP_TDYTH_4G_DB_Oldinsert(MYSQL *conn_ptr, struct DBInsertInfo *pstDbInsertInfo);

/*
 * 功能：插入数据
 * 参数：    @conn_ptr     mysql句柄
 *            @pstDbInsertInfo  表名、字段名、字段值
 * 返回值： 0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_DB_Newinsert(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_DBInsertInfo *pstDbInsertInfo);

///*
// * 功能：通过某元素B在某表T中查找某元素A
// * 参数： @conn_ptr【in】                mysql句柄
// *        @pstDbQueryInfo【in&out】     数据库查询信息结构体
// * 返回值： 0   成功
// *          非0 错误码
// */
//int NSP_TDYTH_4G_DB_select_userInfo_by_limit(MYSQL *conn_ptr, struct DBQueryInfo *pstDbQueryInfo);

/*
 * 功能： 删除数据
 * 参数：    @conn_ptr     mysql句柄
 *            @pstDbDeleteInfo  表名、字段名、字段值
 * 返回值： 0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_DB_delete(MYSQL *conn_ptr, struct DBDeleteInfo *pstDbDeleteInfo);
/*
 * 功能：数据库测试函数
 * 参数：	
 * 返回值：0   成功
 *        非0 错误码
 */
//int NSP_TDYTH_4G_DB_test();


/*
 * 功能：更新数据
 * 参数：	  @conn_ptr     mysql句柄
 *            @
 * 返回值：	0   成功
 *          非0 错误码
 */



/*
 * 功能：删除数据
 * 参数：	  @conn_ptr     mysql句柄
 *            @
 * 返回值：	0   成功
 *          非0 错误码
 */
//int NSP_TDYTH_4G_DB_delete(MYSQL *conn_ptr, );



























#ifdef __cplusplus
}
#endif

#endif //TDYTH_NSP_NAAAS_TDYTH_4G_DB_H
