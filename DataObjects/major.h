#ifndef MAJOR_H
#define MAJOR_H
#include"dataobject.h"

class Major:public DataObject
{
public:
    std::string majorName;
    Major();
    bool createTable() override;
    bool insert() override;
    bool selectById(int id) override;
    bool deleteData() override;
    bool updateData() override;
    std::vector<DataObject*> selectAll() override;
};

#endif // MAJOR_H
