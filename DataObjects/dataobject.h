#ifndef DATAOBJECT_H
#define DATAOBJECT_H

#include <string>
#include <vector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

#define DATABASE_NAME "student.db"

class DataObject
{
public:
    DataObject();
protected:
    std::string tableName;
    DataObject(std::string name);
    static QSqlDatabase* db;
    bool isDataBaseOpen;
public:
    int id;

    static bool openDataBase();
    static bool closeDataBase();

    virtual bool createTable()=0;               // 创建数据库表
    virtual bool insert()=0;                    // 将当前类对象的数据插入到数据库表里
    virtual bool selectById(int id)=0;          // 根据ID将数据库表的数据加载到当前类对象中
    virtual bool deleteData()=0;                // 将当前类对象的ID指定的数据删除
    virtual bool updateData()=0;                // 将当前类对象的ID指定的数据更新
    virtual std::vector<DataObject*> selectAll()=0; // 获取所有数据
    virtual std::vector<DataObject*> selectByQuery(QSqlQuery sql)=0;    // 根据sql条件查询
};

QSqlDatabase* DataObject::db = nullptr;

#endif // DATAOBJECT_H
