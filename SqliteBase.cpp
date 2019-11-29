//
// Created by hoenking on 11/27/19.
//

#include "SqliteBase.h"
#include "basedef.h"
#include <iostream>
#include <sstream>

//打开数据库
bool SqliteBase::OpenDataBase(const std::string& strDBFileName)
{
    int res = sqlite3_open(strDBFileName.c_str(), &m_DBHandle);
    //如果打开失败返回否则继续
    if(res)
    {
        fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(m_DBHandle));
        return false;
    }
    else
    {
        fprintf(stderr,"open database succeddfully\n");
    }
    return true;
}
//关闭数据库
bool SqliteBase::CloseDataBase()
{
    sqlite3_close(m_DBHandle);
    return true;
}
//创建表
bool SqliteBase::CreateOneTable(const std::string& strTableName, const std::string& strFieldInfo)
{
    //创建company 数据表的 sql语句
    //const char* sql = "create table company(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE TEXT NOT NULL, ADDRESS CHAR(50))";


    sprintf(sql, "create table %s(%s);",  strTableName.c_str(), strFieldInfo.c_str());

    int res = sqlite3_exec(m_DBHandle, sql, callback, nullptr, &m_strErrMsg); // 执行上面sql中的命令

    //sql执行结果判断
    if(SQLITE_OK != res)
    {
        fprintf(stderr, "SQL error: %s\n", m_strErrMsg);
        return false;
    }
    else
    {
        fprintf(stdout, "create table successfully\n");
    }
    return true;
}
//插入一条记录
bool SqliteBase::InsertRecord(const std::string& strTableName, const std::string& strFields, const std::string& strValues)
{
    sprintf(sql, "insert into %s(%s) values(%s);",  strTableName.c_str(), strFields.c_str(),strValues.c_str());

    int res = sqlite3_exec(m_DBHandle, sql, callback, nullptr, &m_strErrMsg); // 执行上面sql中的命令

    //sql执行结果判断
    if(SQLITE_OK != res)
    {
        fprintf(stderr, "SQL error: %s\n", m_strErrMsg);
        return false;
    }
    else
    {
        fprintf(stdout, "insert record successfully\n");
    }
    return true;
}
//查询
bool SqliteBase::Search(const std::string& strTableName, const std::string& strWhere)
{

    sprintf(sql,"select * from %s where %s ;", strTableName.c_str(), strWhere.c_str());

    int res = sqlite3_exec(m_DBHandle, sql, callback, nullptr, &m_strErrMsg); // 执行上面sql中的命令

    //sql执行结果判断
    if(SQLITE_OK != res)
    {
        fprintf(stderr, "SQL error: %s\n", m_strErrMsg);
        return false;
    }
    else
    {
        fprintf(stdout, "search successfully\n");
    }
    return true;
}
//更新并查询
bool SqliteBase::Update(const std::string& strTableName, const std::string& strField, const std::string& strValue, const std::string& strWhere)
{
    sprintf(sql, "update %s set %s = %s where ;select * from %s;", strTableName.c_str(), strField.c_str(), strValue.c_str(), strWhere.c_str(), strTableName.c_str());

    int res = sqlite3_exec(m_DBHandle, sql, callback, nullptr, &m_strErrMsg); // 执行上面sql中的命令

    //sql执行结果判断
    if(SQLITE_OK != res)
    {
        fprintf(stderr, "SQL error: %s\n", m_strErrMsg);
        return false;
    }
    else
    {
        fprintf(stdout, "update table successfully\n");
    }
    return true;
}
//删除并查询
bool SqliteBase::Delete(const std::string& strTableName, const std::string& strWhere)
{
    sprintf(sql,"delete from %s where %s;select * from %s;", strTableName.c_str(), strWhere.c_str(),strTableName.c_str());

    int res = sqlite3_exec(m_DBHandle, sql, callback, nullptr, &m_strErrMsg); // 执行上面sql中的命令

    //sql执行结果判断
    if(SQLITE_OK != res)
    {
        fprintf(stderr, "SQL error: %s\n", m_strErrMsg);
        return false;
    }
    else
    {
        fprintf(stdout, "delete table successfully\n");
    }
    return true;
}