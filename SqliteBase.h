//
// Created by hoenking on 11/27/19.
//

#ifndef C__LEARN_SQLITEBASE_H
#define C__LEARN_SQLITEBASE_H
#include <sqlite3.h>
#include <string>

class SqliteBase {
    sqlite3* m_DBHandle;
    char* m_strErrMsg;  //错误信息
    char sql[1024];
    static int callback(void *NotUsed, int argc, char **argv, char **azColName)
    {
        int i = 0;
        for(i = 0; i < argc; i++)
        {
            printf("%s = %s\n",azColName[i], argv[i]?argv[i]:"NULL");
        }

        printf("\n");
        return 0;
    }

public:
    //打开数据库
    bool OpenDataBase(const std::string& strDBFileName);
    //关闭数据库
    bool CloseDataBase();
    //创建表
    bool CreateOneTable(const std::string& strTableName, const std::string& strFieldInfo);
    //插入一条记录
    bool InsertRecord(const std::string& strTableName, const std::string& strFields, const std::string& strValues);
    //查询
    bool Search(const std::string& strTableName, const std::string& strWhere);
    //更新并查询
    bool Update(const std::string& strTableName, const std::string& strField, const std::string& strValue, const std::string& strWhere);
    //删除并查询
    bool Delete(const std::string& strTableName, const std::string& strWhere);

};


#endif //C__LEARN_SQLITEBASE_H
