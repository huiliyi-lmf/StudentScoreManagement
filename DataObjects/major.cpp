#include "major.h"

Major::Major(): DataObject("Major") {}

bool Major::createTable() {
    QSqlQuery query ;
    return query.exec("CREATE TABLE IF NOT EXISTS `major`  ("
  "`id` int(11) NOT NULL,"
  "`majorName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "PRIMARY KEY (`id`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;");
}
bool Major::insert() {
    QSqlQuery query;
    return query.exec(QString("INSERT INTO %1 (id, majorName) values ('%2', '%3')").arg(STDTOQSTR(this->tableName)).arg(id).arg(STDTOQSTR(this->majorName)));

}

bool Major::selectById(int id) {
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id); 
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        this->majorName = query.value("majorName").toString().toStdString();
        return true; 
    }
    else {
        return false; 
    }
}
bool Major::deleteData() {
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
        return false; 
    }
    return query.numRowsAffected() > 0;
}
bool Major::updateData() {
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET majorName =? WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(majorName));
    query.addBindValue(id);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0; 
}
std::vector<DataObject*> Major::selectAll() {
    std::vector<DataObject*> majors;
    QSqlQuery query;
    query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName))); 
    while (query.next()) {
        Major* major = new Major();
        major->id = query.value("id").toInt();
       major->majorName = query.value("majorName").toString().toStdString();
       majors.push_back(major);
    }
    return majors;
}
std::vector<DataObject*> Major::selectByQuery(QSqlQuery sql) {
    std::vector<DataObject*> majors;
}
