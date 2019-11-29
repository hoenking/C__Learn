#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <ctime>

#include "basedef.h"
#include "SqliteBase.h"

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
        int sec,i=0;
        std::cin >> sec;
        clock_t delay = sec * CLOCKS_PER_SEC;
        clock_t start = clock();

        while (clock()-start < delay) {
            std::cout << "tick : " << i << std::endl;
            i++;
        }
        std::cout << "Hello Thread" << std::endl;
    }
};

class CrossLine:public TestModel
{
    void func() override
    {
        std::cout << "Cross Line" << std::endl;
        sqlite3* dbhandle;
        sqlite3_open("test.db", &dbhandle);
        sqlite3_close(dbhandle);
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
        const char* strName = "test.db";  //数据库
        const char* strTable = "company";  //表
        SqliteBase db{};
        db.OpenDataBase(strName);
        db.CreateOneTable(strTable, "ID INT PRIMARY KEY NOT NULL, NAME TEXT NOT NULL, AGE TEXT NOT NULL, ADDRESS CHAR(50)");

        //插入数据
        const std::string strFields = "ID, NAME, AGE, ADDRESS";
        std::vector<std::string> strValueList;
        strValueList.emplace_back("1, 'Paul', 25, 'USA'");
        strValueList.emplace_back("2, 'James', 28, 'JAP'");
        strValueList.emplace_back("3, 'Yao', 30, 'CHA'");
        strValueList.emplace_back("4, 'kobe', 38, 'USA'");

        for(const auto& val:strValueList)
            db.InsertRecord(strTable,strFields,val);

        //查询
        db.Search(strTable, "ID=2");

        //更新
        db.Update(strTable, "ADDRESS", "CHA", "ID=4");

        //删除
        db.Delete(strTable, "ADDRESS = 'JAP'");

        //关闭数据库
        //db.CloseDataBase();

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
        l->startThread();
    }
    system("pause");
    return 0;
}
