#ifndef SCORE_H
#define SCORE_H
#include"dataobject.h"
#include <QSqlQuery>
class Score:public DataObject
{
public:
    int stuId;
    int subId;
    double score;
    Score();

    bool insert() override;         //插入数据
    bool selectById(int id) override;//根据id查询数据
    bool deleteData() override;     //删除数据
    bool updateData() override;     //更新数据
    std::vector<DataObject*> selectAll() override;//查询所有数据
    bool isScoreExits(QWidget* parent);
};

#endif // SCORE_H
