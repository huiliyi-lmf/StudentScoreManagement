#include "class.h"

Class::Class():DataObject("Class") {}
bool Class::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS `class`  ("
  "`classId` int(11) NOT NULL,"
 " `gradeId` int(11) NOT NULL,"
  "`className` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
 " PRIMARY KEY (`classID`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;"
                      );

}

bool Class::insert() {
   QSqlQuery query;
   query.prepare(QString("INSERT INTO %1 (classID, gradeID, className) values ('%2', '%3', '%4')").arg(STDTOQSTR(this->tableName)).arg(classId).arg(gradeId).arg(STDTOQSTR(this->className)));
   return query.exec(); 
}

bool Class::selectById(int id) {
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE classID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id); 
    if(query.exec() && query.next()) {
        this->classId = query.value("classID").toInt();
        this->gradeId = query.value("gradeID").toInt();
        this->className = query.value("className").toString().toStdString(); 
    }
    else {
        return false; 
    }
}
bool Class::deleteData() {
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE classID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(classId); 
    if(!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Class::updateData() {
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET gradeID =?, className =? WHERE classID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(gradeId); 
    query.addBindValue(STDTOQSTR(className));
    query.addBindValue(classId);
    if(!query.exec()) {
        return false;
     }
    return query.numRowsAffected() > 0;
  
}

std::vector<DataObject*> Class::selectAll() {
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName))); 
    std::vector<DataObject*> result;
    if(query.exec()) {
        while(query.next()) {
            Class* obj = new Class();
            obj->classId = query.value("classID").toInt();
            obj->gradeId = query.value("gradeID").toInt(); 
            obj->className = query.value("className").toString().toStdString();
            result.push_back(obj);
        } 
}
    return result;
       
    
}
std::vector<DataObject*> Class::selectByQuery(QSqlQuery sql) {
    std::vector<DataObject*> result; 
}
