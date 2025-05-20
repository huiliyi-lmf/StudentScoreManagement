#include "subject.h"

Subject::Subject():DataObject("Subject") {}
bool Subject::createTable() 
{
    QSqlQuery query;
   return query.exec("CREATE TABLE IF NOT EXISTS `subject`  ("
  "`id` int(11) NOT NULL,"
  "`subName` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "PRIMARY KEY (`id`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;");
}
bool Subject::insert() 
{
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 ( `subName`) VALUES (?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(subName));
    return query.exec();
}

bool Subject::selectById(int id)
{
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        this->subName = query.value("subName").toString().toStdString();
        return true; 
    }
    else {
        return false; 
    }
}
bool Subject::deleteData()
{
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE id=?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Subject::updateData()
{
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET subName =? WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(subName));
    query.addBindValue(id);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
std::vector<DataObject*> Subject::selectAll()
{
    std::vector<DataObject*> subjects;
    QSqlQuery query;
    query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName)));
    while (query.next()) {
        Subject* subject = new Subject();
        subject->id = query.value("id").toInt();
        subject->subName = query.value("subName").toString().toStdString();
        subjects.push_back(subject); 
    }
    return subjects;
}

    
