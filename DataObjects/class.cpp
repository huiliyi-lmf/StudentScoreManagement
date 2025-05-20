#include "class.h"

Class::Class():DataObject("Class") {}
bool Class::createTable() {
    QSqlQuery query(*this->db);
    return query.exec("CREATE TABLE IF NOT EXISTS `class`  ("
  "`id` int(11) NOT NULL,"
 " `majorId` int(11) NOT NULL,"
  "`className` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
 " PRIMARY KEY (`id`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;"
                      );

}

bool Class::insert() {
    this->createTable();
   QSqlQuery query(*this->db);
   query.prepare(QString("INSERT INTO %1 (majorID, className) values ('%2', '%3')").arg(STDTOQSTR(this->tableName)).arg(majorId).arg(STDTOQSTR(this->className)));
   return query.exec(); 
}

bool Class::selectById(int id) {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("SELECT * FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id); 
    if(query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        this->majorId = query.value("majorID").toInt();
        this->className = query.value("className").toString().toStdString();
        return true;
    }
    else {
        return false; 
    }
}
bool Class::deleteData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if(!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Class::updateData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("UPDATE %1 SET majorID =?, className =? WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(majorId);
    query.addBindValue(STDTOQSTR(className));
    query.addBindValue(id);
    if(!query.exec()) {
        return false;
     }
    return query.numRowsAffected() > 0;
  
}

std::vector<DataObject*> Class::selectAll() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName))); 
    std::vector<DataObject*> result;
    if(query.exec()) {
        while(query.next()) {
            Class* obj = new Class();
            obj->id = query.value("id").toInt();
            obj->majorId = query.value("majorID").toInt();
            obj->className = query.value("className").toString().toStdString();
            result.push_back(obj);
        } 
}
    return result;
}
