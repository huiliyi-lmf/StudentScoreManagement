#include "admin.h"

Admin::Admin() : DataObject("Admin") {

}
bool Admin::createTable() {
    QSqlQuery query(*this->db);
    return query.exec("CREATE TABLE IF NOT EXISTS admin ("
                        "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
                        "userName TEXT NOT NULL,"
                        "userPwd TEXT NOT NULL"
                        ");");
}
bool Admin::insert() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("INSERT INTO %1 (userName,userPwd) VALUES (?,?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(this->userName));
    query.addBindValue(STDTOQSTR(this->userPwd));
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Admin::selectById(int id) {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("SELECT * FROM %1 WHERE id = ?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        this->userName = query.value("userName").toString().toStdString();
        this->userPwd = query.value("userPwd").toString().toStdString();
        return true;
    } 
    else {
        return false;
    }
}
bool Admin::deleteData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
      
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Admin::updateData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("UPDATE %1 SET userName=?,userPwd=? WHERE id=?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(this->userName));
    query.addBindValue(STDTOQSTR(this->userPwd));
    query.addBindValue(this->id);
    if (!query.exec()) {
       
        return false; 
    }
    return query.numRowsAffected() > 0;
}
std::vector<DataObject*> Admin::selectAll() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName)));
    std::vector<DataObject*> result;
    while (query.next()) {
        Admin* admin = new Admin();
        admin->id = query.value("id").toInt();
        admin->userName = query.value("userName").toString().toStdString();
        admin->userPwd = query.value("userPwd").toString().toStdString();
        result.push_back(admin);
    }
    return result;

}
