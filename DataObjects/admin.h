#ifndef ADMIN_H
#define ADMIN_H

#include "dataobject.h"
#include <QSqlQuery>

class Admin : public DataObject
{
public:
    std::string userName;
    std::string userPwd;
    Admin();

    bool insert() override;
    bool selectById(int id) override;
    bool deleteData() override;
    bool updateData() override;
    std::vector<DataObject*> selectAll() override;
};

#endif // ADMIN_H
