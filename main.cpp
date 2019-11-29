#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include <ctime>
#include "sqlite3.h"

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

    for (auto l:factory.GetModelList()) {
        l->startThread();
    }
    system("pause");
    return 0;
}
