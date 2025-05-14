#ifndef RESULT_H
#define RESULT_H
#include"dataobject.h"
#include <QSqlQuery>
class Result:public DataObject
{
public:
    int ID;
    std::string stuId;
    int kindId;
    int subId;
    double result;
    Result();
    bool createTable() override;    //创建表
    bool insert() override;         //插入数据
    bool selectById(int id) override;//根据id查询数据
    bool deleteData() override;     //删除数据
    bool updateData() override;     //更新数据
    std::vector<DataObject*> selectAll() override;//查询所有数据
    std::vector<DataObject*> selectByQuery(QSqlQuery sql) override;//根据查询语句查询数据
};

#endif // RESULT_H
