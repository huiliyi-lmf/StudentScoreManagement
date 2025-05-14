#include "result.h"

Result::Result():DataObject("Result") {}

bool Result::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS `result`  ("
  "`ID` int(11) NOT NULL,"
  "`stuId` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`kindId` int(11) NOT NULL,"
  "`subId` int(11) NOT NULL,"
  "`result` double NOT NULL,"
 " PRIMARY KEY (`ID`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;)");

}
bool Result::insert() {
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (ID,stuId,kindId,subId,result) values(?,?,?,?,?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(ID); 
    query.addBindValue(STDTOQSTR(stuId)); 
    query.addBindValue(kindId); 
    query.addBindValue(subId); 
    query.addBindValue(result); 
    return query.exec();

}
bool Result::selectById(int id) {
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE ID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->ID = query.value("ID").toInt(); 
        this->stuId = query.value("stuId").toString().toStdString(); 
        this->kindId = query.value("kindId").toInt();
        this->subId = query.value("subId").toInt(); 
        this->result = query.value("result").toDouble(); 
        return true;
    } 
    else {
        return false; 
    }
}
bool Result::deleteData() {
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE ID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(ID); 
    if (!query.exec()) {
       return false; 
    }
    return query.numRowsAffected() > 0;
}
bool Result::updateData() {
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET stuId=?,kindId=?,subId=?,result=? WHERE ID=?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(stuId)); 
    query.addBindValue(kindId);
    query.addBindValue(subId); 
    query.addBindValue(result); 
    query.addBindValue(ID); 
    if (!query.exec()) {
        return false; 
    }
    return query.numRowsAffected() > 0;
}
std::vector<DataObject*> Result::selectAll() {
    std::vector<DataObject*> results;
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName))); 
    while(query.next()) {
        Result* result = new Result();
        result->ID = query.value("ID").toInt();
        result->stuId = query.value("stuId").toString().toStdString(); 
        result->kindId = query.value("kindId").toInt();
        result->subId = query.value("subId").toInt();
        result->result = query.value("result").toDouble();
        results.push_back(result);
    }
    return results;
}
std::vector<DataObject*> Result::selectByQuery(QSqlQuery sql) {
    return std::vector<DataObject*>();
}
