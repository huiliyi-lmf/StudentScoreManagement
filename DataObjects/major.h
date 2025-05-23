#ifndef MAJOR_H
#define MAJOR_H
#include"dataobject.h"
#include<QMessageBox>
#include<QSqlError>
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
    bool isNameExists(QWidget* parent);
    // 新增的方法：检查指定名称是否已被其他专业（不同ID）使用
    bool isNameTakenByOther(const std::string& nameToCheck, int currentMajorId, QWidget* parent);
    std::vector<DataObject*> selectAll() override;
};

#endif // MAJOR_H
