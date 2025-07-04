#ifndef SUBJECT_H
#define SUBJECT_H
#include"dataobject.h"
#include <QSqlQuery>
#include<QMessageBox>
#include<QSqlError>
class Subject:public DataObject
{
public:
    int majorId;
    std::string subName;
    Subject();

    bool insert() override;         //插入数据
    bool selectById(int id) override;//根据id查询数据
    bool deleteData() override;     //删除数据
    bool updateData() override;     //更新数据
    std::vector<DataObject*> selectAll() override;//查询所有数据
    bool isSubjectNameTakenInMajor(const std::string& subjectNameToCheck, int targetMajorId, int currentSubjectId = -1);
};

#endif // SUBJECT_H
