#include "subject.h"

Subject::Subject():DataObject("Subject") {}
bool Subject::createTable() 
{
    QSqlQuery query(*this->db);
   return query.exec("CREATE TABLE IF NOT EXISTS `subject`  ("
  "`id` int(11) NOT NULL,"
  "`majorId` int(11) NOT NULL,"
  "`subName` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "PRIMARY KEY (`id`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;");
}
bool Subject::insert() 
{
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("INSERT INTO %1 (`majorId`, `subName`) VALUES (?,?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(majorId);
    query.addBindValue(STDTOQSTR(subName));
    return query.exec();
}

bool Subject::selectById(int id)
{
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("SELECT * FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        this->majorId=query.value("majorId").toInt();
        this->subName = query.value("subName").toString().toStdString();
        return true; 
    }
    else {
        return false; 
    }
}
bool Subject::deleteData()
{
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id=?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Subject::updateData()
{
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("UPDATE %1 SET majorId=?,subName =? WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(majorId);
    query.addBindValue(STDTOQSTR(subName));
    query.addBindValue(id);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
std::vector<DataObject*> Subject::selectAll()
{
    this->createTable();
    std::vector<DataObject*> subjects;
    QSqlQuery query(*this->db);
    query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName)));
    while (query.next()) {
        Subject* subject = new Subject();
        subject->id = query.value("id").toInt();
        subject->majorId=query.value("majorId").toInt();
        subject->subName = query.value("subName").toString().toStdString();
        subjects.push_back(subject); 
    }
    return subjects;
}

    
