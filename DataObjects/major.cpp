#include "major.h"

Major::Major(): DataObject("Major") {}

bool Major::createTable() {
    QSqlQuery query(*this->db) ;
    return query.exec("CREATE TABLE IF NOT EXISTS `major`  ("
  "`id` int(11) NOT NULL,"
  "`majorName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "PRIMARY KEY (`id`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;");
}
bool Major::insert() {
    this->createTable();
    QSqlQuery query(*this->db);
    return query.exec(QString("INSERT INTO %1 (majorName) values ('%2')").arg(STDTOQSTR(this->tableName)).arg(STDTOQSTR(this->majorName)));

}

bool Major::selectById(int id) {
    this->createTable();
    QSqlQuery query(*this->db);
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
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
        return false; 
    }
    return query.numRowsAffected() > 0;
}
bool Major::updateData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("UPDATE %1 SET majorName =? WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(majorName));
    query.addBindValue(id);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0; 
}
std::vector<DataObject*> Major::selectAll() {
    this->createTable();
    std::vector<DataObject*> majors;
    QSqlQuery query(*this->db);
    query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName))); 
    while (query.next()) {
        Major* major = new Major();
        major->id = query.value("id").toInt();
       major->majorName = query.value("majorName").toString().toStdString();
       majors.push_back(major);
    }
    return majors;
}
