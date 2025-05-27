#ifndef STUDENT_H
#define STUDENT_H
#include"dataobject.h"
#include <QSqlQuery>
class Student:public DataObject
{
public:
    std::string stuName;
    int classID;
    int majorID;
    std::string sex;
    int age;
    std::string address;
    std::string phoneNum;
    Student();

    bool insert() override;         //插入数据
    bool selectById(int id) override;//根据id查询数据
    bool deleteData() override;     //删除数据
    bool updateData() override;     //更新数据
    std::vector<DataObject*> selectAll() override;//查询所有数据
    
    // 获取指定专业的学生人数
    int getStudentCountByMajorId(int majorId);
};

#endif // STUDENT_H
