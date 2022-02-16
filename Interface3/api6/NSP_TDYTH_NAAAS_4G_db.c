/*
 * 文件功能：4G接入认证协议数据库操作相关文件
 * 作者：董珂 丁潇  寇飞翔 李泽宇 李紫妍
 * 创建日期：2020/02/12
 * 修改日期：
 * 版本：V1.0
 * 备注：
 */

#include "NSP_TDYTH_NAAAS_4G_db.h"

/*
 * 功能：连接mysql数据库
 * 参数：
 * 返回值：	MYSQL对象conn_ptr
 */
MYSQL *NSP_TDYTH_4G_DB_connect() {
    // Initialize a MYSQL object
    MYSQL *conn_ptr = mysql_init(NULL);
    if (conn_ptr == NULL) {
        printf("Failed to initialise database: insufficient memory\n");
        return NULL;
    }
    // Connection to a MySQL database

    conn_ptr = mysql_real_connect(
            conn_ptr,
            D_NSP_TDYTH_NAAAS_4G_DB_IP,
            D_NSP_TDYTH_NAAAS_4G_DB_USER,
            D_NSP_TDYTH_NAAAS_4G_DB_PASSWD,
            D_NSP_TDYTH_NAAAS_4G_DB_DB,
            D_NSP_TDYTH_NAAAS_4G_DB_PORT,
            NULL,
            // 告诉服务器 client 可以在单个 string 中发送多个 statements(由;个字符分隔)
            CLIENT_MULTI_STATEMENTS
    );
    if (conn_ptr == NULL) {
        printf("Failed to connect to database: Error: %s\n", mysql_error(conn_ptr));
        return NULL;
        //return NSP_TDYTH_4G_DB_CONNECT_ERR;
    }
    return conn_ptr;
}


/*
 * 功能：查询单个数据
 * 参数：	  @conn_ptr     mysql句柄
 *            @
 * 返回值：	0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_DB_select_onlyInfo_by_limit(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_DBQueryInfo *pstDbQueryInfo, unsigned char *pcSendbuf)
{
    int iRet;
    char sql[500];
    char *header = "select";
    char *FROM = "FROM";
    char *WHERE = "WHERE";

    sprintf(sql, "%s %s %s %s %s %s='%s';", header, pstDbQueryInfo->fieldKey, FROM, pstDbQueryInfo->tableName, WHERE, pstDbQueryInfo->limitName, pstDbQueryInfo->limitValue);
    printf("querySQLQuery: %s\n", sql);
    MYSQL_RES *res;

    unsigned long *lengths;

    MYSQL_ROW row;
    iRet = mysql_real_query(conn_ptr, sql, strlen(sql));
    if (iRet)
    {
        printf("Failed to query: %s\n", mysql_error(conn_ptr));
        return -1;
    }
    res = mysql_store_result(conn_ptr);
    row = mysql_fetch_row(res);
    lengths = mysql_fetch_lengths(res);

    if(row == NULL)
    {
        printf("no result\n");
    }
  //  printf("%s\n", row[0]);
    memcpy(pcSendbuf, row[0],lengths[0] + 1);


    mysql_free_result(res);
    return iRet;

}

/*
 * 功能：插入数据
 * 参数：	  @conn_ptr     mysql句柄
 *            @pstDbInsertInfo  表名、字段名、字段值
 * 返回值：	0   成功
 *          非0 错误码
 */
//int NSP_TDYTH_4G_DB_Oldinsert(MYSQL *conn_ptr, struct DBInsertInfo *pstDbInsertInfo)
//{
//    int iRet;
//    char insertSQLQuery[500];
//    char *insertSQLHead = "INSERT INTO ";
//    char *insertSQLLeft = "(";
//    char *insertSQLRight = ")";
//    char *insertSQLValues = "VALUES";
//    sprintf(insertSQLQuery, "%s%s %s%s%s %s %s%s%s;\n",
//            insertSQLHead, pstDbInsertInfo->tableName, insertSQLLeft, pstDbInsertInfo->fieldKey,
//            insertSQLRight, insertSQLValues, insertSQLLeft, pstDbInsertInfo->fieldStringValue, insertSQLRight);
//    printf("insertSQLQuery: %s\n", insertSQLQuery);
//    iRet = mysql_real_query(conn_ptr, insertSQLQuery, strlen(insertSQLQuery));
//    if (iRet) {
//        printf("Failed to insert: %s\n", mysql_error(conn_ptr));
//        return -1;
//        //return NSP_TDYTH_4G_DB_INSERT_ERR;
//    }
//    printf("Insert successfully");
//    return iRet;
//}

/*
 * 功能：插入数据
 * 参数：    @conn_ptr     mysql句柄
 *            @pstDbInsertInfo  表名、字段名、字段值
 * 返回值： 0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_DB_Newinsert(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_DBInsertInfo *pstDbInsertInfo) {
    int iRet;
    char insertSQLQuery[500];
    char *insertSQLHead = "INSERT INTO ";
    char *insertSQLLeft = "(";
    char *insertSQLRight = ")";
    char *insertSQLValues = "VALUES";
    char *insertSQLComma = ",";
    sprintf(insertSQLQuery, "%s%s%s%s%s %s %s%s%s%d%s;\n",
            insertSQLHead, pstDbInsertInfo->tableName, insertSQLLeft, pstDbInsertInfo->fieldKey,
            insertSQLRight, insertSQLValues, insertSQLLeft, pstDbInsertInfo->fieldStringValue,
            insertSQLComma, pstDbInsertInfo->port, insertSQLRight);
    printf("insertSQLQuery: %s\n", insertSQLQuery);
    iRet = mysql_real_query(conn_ptr, insertSQLQuery, strlen(insertSQLQuery));
    if (iRet) {
        printf("Failed to insert: %s\n", mysql_error(conn_ptr));
        return -1;
        //return NSP_TDYTH_4G_DB_INSERT_ERR;
    }
    printf("Insert successfully");
    return iRet;
}

///*
// * 功能：查找IMSI是否在HSS表中
// * 参数：	  @conn_ptr     mysql句柄
// *            @pucIMSI      4G用户身份标识符
// * 返回值：	0   成功
// *          非0 错误码
// */
//int NSP_TDYTH_4G_DB_select(MYSQL *conn_ptr, struct DBQueryInfo *pstDbQueryInfo) {
//    int iRet;
////    char *query = "show databases";
//    char querySQLQuery[500];
//    char *querySQLHead = "SELECT";
//    char *querySQLLeft = "(";
//    char *querySQLRight = ")";
//    char *querySQLFrom = "From";
////    char *querySQLLimit = "LIMIT";
////    sprintf(querySQLQuery, "%s %s%s%s %s %s%s%s %s %u;\n",
////            querySQLHead, querySQLLeft, pstDbQueryInfo->keyName, querySQLRight, querySQLFrom,
////            querySQLLeft, pstDbQueryInfo->tableName, querySQLRight, querySQLLimit, pstDbQueryInfo->limitNum);
//    sprintf(querySQLQuery, "%s %s%s%s %s %s%s%s;\n",
//            querySQLHead, querySQLLeft, pstDbQueryInfo->keyName, querySQLRight, querySQLFrom,
//            querySQLLeft, pstDbQueryInfo->tableName, querySQLRight);
//    printf("querySQLQuery: %s\n", querySQLQuery);
//    MYSQL_RES *res;
//    MYSQL_ROW row;
//    iRet = mysql_real_query(conn_ptr, querySQLQuery, strlen(querySQLQuery));
//    if (iRet) {
//        printf("Failed to query: %s\n", mysql_error(conn_ptr));
//        return -1;
//    }
//    res = mysql_store_result(conn_ptr);
//    printf("\n");
//    row = mysql_fetch_row(res);
//    while (row) {
//        int i;
//        for (i = 0; i < mysql_num_fields(res); i++) {
//            printf("%s\t", row[i]);
//        }
//        printf("\n");
//        row = mysql_fetch_row(res);
//    }
//    mysql_free_result(res);
//    return iRet;
//}

// int NSP_TDYTH_4G_DB_select_key_by_limit(MYSQL *conn_ptr, struct DBQueryInfo *pstDbQueryInfo)
// {
//     int iRet;
//     char querySQLQuery[500];
//     char *querySQLHead = "SELECT";
//     char *querySQLLeft = "(";
//     char *querySQLRight = ")";
//     char *querySQLFrom = "From";
//     char *querySQLWhere = "WHERE";
//     sprintf(querySQLQuery, "%s %s%s%s %s %s%s%s %s %s = %s;\n", querySQLHead, querySQLLeft, pstDbQueryInfo->keyName, querySQLRight, querySQLFrom, querySQLLeft, pstDbQueryInfo->tableName, querySQLRight, querySQLWhere, pstDbQueryInfo->limitName, pstDbQueryInfo->limitValue);
//     printf("querySQLQuery: %s\n", querySQLQuery);
//     MYSQL_RES *res;
//     MYSQL_ROW row;
//     iRet = mysql_real_query(conn_ptr, querySQLQuery, strlen(querySQLQuery));
//     if (iRet) {
//         printf("Failed to query: %s\n", mysql_error(conn_ptr));
//         return -1;
//     }
//     res = mysql_store_result(conn_ptr);
//     printf("\n");
//     row = mysql_fetch_row(res);

//     strcpy(pstDbQueryInfo->keyValue,row[0]);
//     while (row) {
//         int i;
//         for (i = 0; i < mysql_num_fields(res); i++) {
//             printf("%s\t", row[i]);
//         }
//         printf("\n");
//         row = mysql_fetch_row(res);
//     }
// //	strcpy(pstDbQueryInfo->keyValue, row);
//     mysql_free_result(res);
//     return iRet;
// }


///*
// * 功能：   查找某元素A在某表T中是否存在
// * 参数：   @conn_ptr【in】     	  mysql句柄
// *          @pstDbQueryInfo【in&out】     数据库查询信息结构体
// * 返回值：	0      成功，即存在该元素
// *          非0    不存在该元素
// * 作者：   寇飞翔
// */
//int NSP_TDYTH_4G_DB_find(MYSQL *conn_ptr, struct DBQueryInfo *pstDbQueryInfo)
//{
//    int iRet;
//    char querySQLQuery[500];
//    char *querySQLHead = "SELECT";
//    char *querySQLLeft = "(";
//    char *querySQLRight = ")";
//    char *querySQLFrom = "From";
//    char *querySQLEnd = "LIMIT 1";
//    sprintf(querySQLQuery, "%s %s%s%s %s %s%s%s %s;\n",querySQLHead, querySQLLeft, pstDbQueryInfo->keyName, querySQLRight, querySQLFrom, querySQLLeft, pstDbQueryInfo->tableName, querySQLRight, querySQLEnd);
//    printf("querySQLQuery: %s\n", querySQLQuery);
//	MYSQL_RES *res;
//    MYSQL_ROW row;
//    iRet = mysql_real_query(conn_ptr, querySQLQuery, strlen(querySQLQuery));
//    if (iRet) {
//        printf("Failed to query: %s\n", mysql_error(conn_ptr));
//        return -1;
//    }
//	res = mysql_store_result(conn_ptr);
//    printf("\n");
//    row = mysql_fetch_row(res);
//
//    pstDbQueryInfo->keyValue = malloc(200);
//    strcpy(pstDbQueryInfo->keyValue,row[0]);
//    while (row) {
//        int i;
//        for (i = 0; i < mysql_num_fields(res); i++) {
//            printf("%s\t", row[i]);
////            fprintf((FILE *) pstDbQueryInfo->keyValue, "%s\n", row[i]);
//        }
//        printf("\n");
//        row = mysql_fetch_row(res);
//    }
////    memset(pstDbQueryInfo,0, sizeof(struct DBQueryInfo));
////    strcpy(pstDbQueryInfo->keyValue,row[0]);
////	memcpy(pstDbQueryInfo->keyValue, row, sizeof(row)+1);
//    mysql_free_result(res);
//
//    return iRet;
//}

/*
 * 功能：更改数据
 * 参数：    @conn_ptr     mysql句柄
 *            @pstDbInsertInfo  表名、字段名、字段值
 * 返回值： 0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_DB_update(MYSQL *conn_ptr, St_NSP_TDYTH_NAAAS_4G_DBUpdateInfo *pstDbUpdateInfo)
{
    int iRet;
    char updateSQLQuery[500];
    char *updateSQLHead = "UPDATE";
    char *updateSQLLeft = "(";
    char *updateSQLRight = ")";
    char *updateSQLSET = "set";
    char *updateSQLWhere = "where";
    sprintf(updateSQLQuery, "%s %s%s%s %s %s='%s' %s %s='%s';\n",
            updateSQLHead,updateSQLLeft, pstDbUpdateInfo->tableName, updateSQLRight,
            updateSQLSET, pstDbUpdateInfo->fieldKey, pstDbUpdateInfo->fieldValue, updateSQLWhere, pstDbUpdateInfo->limitName, pstDbUpdateInfo->limitValue);
    printf("updateSQLQuery: %s\n", updateSQLQuery);
    iRet = mysql_real_query(conn_ptr, updateSQLQuery, strlen(updateSQLQuery));
    if (iRet)
    {
        printf("Failed to update: %s\n", mysql_error(conn_ptr));
        return -1;
        //return NSP_TDYTH_4G_DB_INSERT_ERR;
    }
    printf("Update successfully");
    return iRet;
}

///*
// * 功能：通过某元素B在某表T中查找某元素A
// * 参数： @conn_ptr【in】                mysql句柄
// *        @pstDbQueryInfo【in&out】     数据库查询信息结构体
// * 返回值： 0   成功
// *          非0 错误码
// */
//int NSP_TDYTH_4G_DB_select_userInfo_by_limit(MYSQL *conn_ptr, struct DBQueryInfo *pstDbQueryInfo)
//{
//    int iRet;
//    char sql[500];
//    char *header = "select";
//    char *FROM = "FROM";
//    char *WHERE = "WHERE";
//
//    sprintf(sql, "%s %s,%s,%s,%s %s %s %s %s='%s';", header, pstDbQueryInfo->keyName, pstDbQueryInfo->keyName2, pstDbQueryInfo->keyName3, pstDbQueryInfo->keyName4, FROM, pstDbQueryInfo->tableName, WHERE, pstDbQueryInfo->limitName, pstDbQueryInfo->limitValue);
//    printf("querySQLQuery: %s\n", sql);
//    MYSQL_RES *res;
//
//    MYSQL_ROW row;
//    iRet = mysql_real_query(conn_ptr, sql, strlen(sql));
//    if (iRet)
//    {
//        printf("Failed to query: %s\n", mysql_error(conn_ptr));
//        return -1;
//    }
//    res = mysql_store_result(conn_ptr);
//    row = mysql_fetch_row(res);
//
//    while (row)
//    {
//        int i;
//        for (i = 0; i < mysql_num_fields(res); i++)
//        {
//            printf("%s\t", row[i]);
//        }
//        printf("\n");
//        row = mysql_fetch_row(res);
//    }
//    mysql_free_result(res);
//    return iRet;
//}

/*
 * 功能： 删除数据
 * 参数：    @conn_ptr     mysql句柄
 *            @pstDbDeleteInfo  表名、字段名、字段值
 * 返回值： 0   成功
 *          非0 错误码
 */
int NSP_TDYTH_4G_DB_delete(MYSQL *conn_ptr, struct DBDeleteInfo *pstDbDeleteInfo) 
{
    int iRet;
    char deleteSQLQuery[500];
    char *deleteSQLHead = "DELETE FROM ";
    char *deleteSQLWhere = "WHERE";
    char *deleteSQLEquel = "=";

    sprintf(deleteSQLQuery, "%s%s %s %s %s %s;\n",
            deleteSQLHead, pstDbDeleteInfo->tableName,  deleteSQLWhere, pstDbDeleteInfo->limitName,
            deleteSQLEquel, pstDbDeleteInfo->limitValue);
    printf("deleteSQLQuery: %s\n", deleteSQLQuery);
    iRet = mysql_real_query(conn_ptr, deleteSQLQuery, strlen(deleteSQLQuery));
    if (iRet) {
        printf("Failed to delete: %s\n", mysql_error(conn_ptr));
        return -1;
        //return NSP_TDYTH_4G_DB_INSERT_ERR;
    }
    printf("Delete successfully");
    return iRet;
}

/*
 * 功能：断开mysql数据库
 * 参数：	  @conn_ptr mysql句柄
 * 返回值：
 */
void NSP_TDYTH_4G_DB_close(MYSQL *conn_ptr) {
    mysql_close(conn_ptr);
}

#define BUFSIZE 1024
/*
 * 功能：数据库测试函数
 * 参数：	
 * 返回值：0   成功
 *             非0 错误码
 */

/* insert -> NSP_TDYTH_UE_AUTHEN and query Demo */
//int NSP_TDYTH_4G_DB_test() {
//    int iRet;
//    struct NSP_TDYTH_UE_AUTHENFieldValue {
//        char IMSI[15];
//        char GUTI[20];
//        char HSS_ID[8];
//        char SNID[5];
//        char SQN[15];
//        char AMF[16];
//        char userKEY[32];
//        char RAND[32];
//        char KASME[32];
//        char KSIASAME[3];
//        char cardID[40];
//    };
//    char *tableName;
//    char *fieldKey;
//    char *fieldValue;
//    fieldValue = malloc(200);
//    // Connect MySQL database
//    MYSQL *conn_ptr = NSP_TDYTH_4G_DB_connect();
//    // Insert data
//    printf("-------------------Insert Demo-------------------\n");
//    struct NSP_TDYTH_UE_AUTHENFieldValue fieldValueSt = {
//            .IMSI="012345678901234",
//            .GUTI="01234567890123456789",
//            .HSS_ID="010",
//            .SNID="01234",
//            .SQN="012345678901234",
//            .AMF="0000000011111",
//            .userKEY="01234567890123456789012345678901",
//            .RAND="01234567890123456789012345678901",
//            .KASME="01234567890123456789012345678901",
//            .KSIASAME="1",
//            .cardID="0123456789012345678901234567890123456789"
//    };
//    tableName = "NSP_TDYTH_UE_AUTHEN";
//    printf("tableName: %s\n", tableName);
//    fieldKey = "IMSI, GUTI, HSS_ID, SNID, SQN, AMF, userKEY, RAND, KASME, KSIASAME, cardID";
//    printf("fieldKey: %s\n", fieldKey);
//    sprintf(fieldValue, "%.*s,%.*s,%.*s,%.*s,%.*s,%.*s,%.*s,%.*s,%.*s,%.*s,%.*s",
//            15, fieldValueSt.IMSI, 20, fieldValueSt.GUTI, 8, fieldValueSt.HSS_ID,
//            5, fieldValueSt.SNID, 15, fieldValueSt.SQN, 16, fieldValueSt.AMF, 32, fieldValueSt.userKEY,
//            32, fieldValueSt.RAND, 32, fieldValueSt.KASME, 3, fieldValueSt.KSIASAME, 40, fieldValueSt.cardID);
//    printf("fieldValue: %s\n", fieldValue);
//    struct DBInsertInfo *insertInfo;
//    insertInfo = (struct DBInsertInfo *) malloc(sizeof(struct DBInsertInfo));
//    insertInfo->tableName = tableName;
//    insertInfo->fieldKey = fieldKey;
//    insertInfo->fieldStringValue = fieldValue;
//    printf("All insert info: %s---%s---%s\n", insertInfo->tableName, insertInfo->fieldKey, insertInfo->fieldStringValue);
//    iRet = NSP_TDYTH_4G_DB_Oldinsert(conn_ptr, insertInfo);
//    if (iRet < 0) {
//        printf("Failed to insert data\n");
//    }
//    // Query data
//    printf("-------------------Query Demo-------------------\n");
//    struct DBQueryInfo *queryInfo;
//    queryInfo = (struct DBQueryInfo *) malloc(sizeof(struct DBQueryInfo));
//    queryInfo->tableName = "NSP_TDYTH_UE_AUTHEN";
//    queryInfo->keyName = "IMSI";
////    queryInfo->limitNum = 2;
////    printf("All query info: %s---%s---%u\n", queryInfo->tableName, queryInfo->keyName, queryInfo->limitNum);
//    iRet = NSP_TDYTH_4G_DB_select(conn_ptr, queryInfo);
//    if (iRet < 0) {
//        printf("Failed to query data\n");
//    }
//    // Close connection
//    NSP_TDYTH_4G_DB_close(conn_ptr);
//    free(insertInfo);
//    return 0;
//}

