#ifndef CLASS_H
#define CLASS_H
#include <QSqlQuery>
#include"dataobject.h"
#include<QMessageBox>
#include<QSqlError>
class Class:public DataObject
{
public:
    std::string className;
    int majorId;
    Class();

    bool insert() override;
    bool selectById(int id) override;
    bool deleteData() override;
    bool updateData() override;
    std::vector<DataObject*> selectAll() override;
    bool isClassNameTakenInMajor(const std::string& classNameToCheck, int targetMajorId, int currentClassId = -1);
};

#endif // CLASS_H
