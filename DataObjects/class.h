#ifndef CLASS_H
#define CLASS_H
#include <QSqlQuery>
#include"dataobject.h"
class Class:public DataObject
{
public:
    std::string className;
    int id;
    int majorId;
    Class();
    bool createTable() override;
    bool insert() override;
    bool selectById(int id) override;
    bool deleteData() override;
    bool updateData() override;
    std::vector<DataObject*> selectAll() override;
    std::vector<DataObject*> selectByQuery(QSqlQuery sql) override;
};

#endif // CLASS_H
