#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

#define ADD_CLASS_FIELD(type, name, getter, setter) \
private:\
    type m_##name;\
public:\
    type& getter(){return m_##name;}\
    type const & getter() const {return m_##name;}\
    void setter(type name){m_##name=name;}

class TestModel
{
    ADD_CLASS_FIELD(std::string, strModel, getModelName, setModelName);
    pthread_t TID;
public:
    virtual void func()=0;
//    {
//        //子线程执行代码
//        std::cout << "Hello, World!" << std::endl;
//    }
    bool startThread();

};

static void* callback( void* arg )
{//回调函数
    ((TestModel*)arg)->func(); //调用成员函数
    return nullptr;
}

bool TestModel::startThread()
{//启动子线程
    int ret = pthread_create( &TID , nullptr , callback , this );
    return ret == 0;
}

class Hello:public TestModel
{
    void func() override
    {
        std::cout << "Hello Thread" << std::endl;
    }
};

class CrossLine:public TestModel
{
    void func() override
    {
        std::cout << "Cross Line" << std::endl;
    }
};

class SqliteDB:public TestModel
{
    // 暂时先不管 返回的数据
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

    void func() override
    {
        sqlite3* db;
        std::string strName = "test.db";  //数据库句柄
        char* zErrMsg;  //错误信息

        //新创建或打开数据库
        int res = sqlite3_open( strName.c_str(), &db);

        //如果打开失败返回否则继续
        if(res)
        {
            fprintf(stderr, "Can't open database:%s\n", sqlite3_errmsg(db));
            return;
        }
        else
        {
            fprintf(stderr,"open database succeddfully\n");
        }

        //创建company 数据表的 sql语句
        const char* sql = "create table company(ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE TEXT NOT NULL, ADDRESS CHAR(50))";

        res = sqlite3_exec(db, sql, callback, nullptr, &zErrMsg); // 执行上面sql中的命令

        //sql执行结果判断
        if(SQLITE_OK != res)
        {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
        }
        else
        {
            fprintf(stdout, "create table successfully\n");
        }

        //关闭数据库
        sqlite3_close(db);
    }
};

class Factory
{
    std::vector<TestModel*> ModelList;
public:
    void AddModel(TestModel* model)
    {
        ModelList.push_back(model);
    }
    std::vector<TestModel*> GetModelList() const {
        return ModelList;
    }
};


int main()
{
//    Hello a;
//    a.startThread();

    Factory factory;
    TestModel* model = nullptr;

    model = new Hello;
    model->setModelName("hello");
    factory.AddModel(model);

    model = new CrossLine;
    model->setModelName("CrossLine");
    factory.AddModel(model);

    model = new SqliteDB;
    model->setModelName("SqliteDB");
    factory.AddModel(model);

    for (auto l:factory.GetModelList()) {
        l->func();
    }

    return 0;
}
