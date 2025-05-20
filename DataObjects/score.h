#ifndef SCORE_H
#define SCORE_H
#include"dataobject.h"
#include <QSqlQuery>
class Score:public DataObject
{
public:
    std::string stuId;
    int subId;
    double score;
    Score();
    bool createTable() override;    //创建表
    bool insert() override;         //插入数据
    bool selectById(int id) override;//根据id查询数据
    bool deleteData() override;     //删除数据
    bool updateData() override;     //更新数据
    std::vector<DataObject*> selectAll() override;//查询所有数据
};

#endif // SCORE_H
