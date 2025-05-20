#include "subject.h"

Subject::Subject():DataObject("Subject") {}
bool Subject::createTable() 
{
    QSqlQuery query;
   return query.exec("CREATE TABLE IF NOT EXISTS `subject`  ("
  "`subID` int(11) NOT NULL,"
  "`subName` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "PRIMARY KEY (`subID`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;");
}
bool Subject::insert() 
{
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (`subID`, `subName`) VALUES (?, ?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(subID);
    query.addBindValue(STDTOQSTR(subName));
    return query.exec();
}

bool Subject::selectById(int id)
{
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE subID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(subID);
    if (query.exec() && query.next()) {
        this->subID = query.value("subID").toInt();
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
    query.prepare(QString("DELETE FROM %1 WHERE subID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(subID);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Subject::updateData()
{
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET subName =? WHERE subID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(subName));
    query.addBindValue(subID);
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
        subject->subID = query.value("subID").toInt();
        subject->subName = query.value("subName").toString().toStdString();
        subjects.push_back(subject); 
    }
    return subjects;
}
std::vector<DataObject*> Subject::selectByQuery(QSqlQuery sql)
{
    std::vector<DataObject*> subjects;
}
    
