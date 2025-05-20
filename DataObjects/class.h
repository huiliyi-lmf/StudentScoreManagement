#ifndef CLASS_H
#define CLASS_H
#include <QSqlQuery>
#include"dataobject.h"
class Class:public DataObject
{
public:
    std::string className;
    int majorId;
    Class();
    bool createTable() override;
    bool insert() override;
    bool selectById(int id) override;
    bool deleteData() override;
    bool updateData() override;
    std::vector<DataObject*> selectAll() override;
};

#endif // CLASS_H
