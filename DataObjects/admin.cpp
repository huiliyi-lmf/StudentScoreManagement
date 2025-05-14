#include "admin.h"

Admin::Admin() : DataObject("Admin") {

}
bool Admin::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS `admin`  ("
                        "`userName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
                        "`userPwd` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
                        "PRIMARY KEY (`userPwd`) USING BTREE"
                        ") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;");
}
bool Admin::insert() {
    QSqlQuery query;
    // query.prepare("INSERT INTO admin (userName, userPwd) VALUES (?, ?)");
    // query.addBindValue(userName); // 第一个 ? 替换为 name
    // query.addBindValue(userPwd);
    // query.exec();
    return query.exec(QString("INSERT INTO %3 (userName, userPwd) values ('%1', '%2')").arg(STDTOQSTR(this->userName)).arg(STDTOQSTR(this->userPwd)).arg(STDTOQSTR(this->tableName)));
}
bool Admin::selectById(int id) {
    QSqlQuery query;
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
    // QSqlQuery query;

    // query.prepare("DELETE FROM users WHERE id = ?");
    // query.addBindValue(id);

    // if (!query.exec()) {
    //     qDebug() << "DeleteById error:" << query.lastError().text();
    //     return false;
    // }
    // return query.numRowsAffected() > 0;
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
      
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Admin::updateData() {
    // QSqlQuery query;

    // query.prepare("UPDATE users SET name = ?, age = ? WHERE id = ?");
    // query.addBindValue(newName);
    // query.addBindValue(newAge);
    // query.addBindValue(id);

    // if (!query.exec()) {
    //     qDebug() << "UpdateById error:" << query.lastError().text();
    //     return false;
    // }
    // return query.numRowsAffected() > 0; // 确认实际影响了行
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET userName=?,usrPwd=?WHERE id=?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(this->userName));
    query.addBindValue(STDTOQSTR(this->userPwd));
    query.addBindValue(this->id);
    if (!query.exec()) {
       
        return false; 
    }
    return query.numRowsAffected() > 0;
}
std::vector<DataObject*> Admin::selectAll() {
    // QList<QVariantMap> result;
    // QSqlQuery query;

    // if (query.exec("SELECT * FROM users")) {
    //     while (query.next()) {
    //         QVariantMap row;
    //         row["id"] = query.value("id");
    //         row["name"] = query.value("name");
    //         row["age"] = query.value("age");
    //         result.append(row);
    //     }
    // } else {
    //     qDebug() << "SelectAll error:" << query.lastError().text();
    // }
    // return result;
    QSqlQuery query;
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
std::vector<DataObject*> Admin::selectByQuery(QSqlQuery sql) {
    return std::vector<DataObject*>();
}
