#ifndef GRADE_H
#define GRADE_H
#include"dataobject.h"

class Grade:public DataObject
{
public:
    std::string gradeName;
    int gradeId;
    Grade();
    bool createTable() override;
    bool insert() override;
    bool selectById(int id) override;
    bool deleteData() override;
    bool updateData() override;
    std::vector<DataObject*> selectAll() override;
    std::vector<DataObject*> selectByQuery(QSqlQuery sql) override;

};

#endif // GRADE_H
