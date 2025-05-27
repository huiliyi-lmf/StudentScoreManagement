#include "class.h"

Class::Class():DataObject("Class") {}


bool Class::insert() {
    QSqlQuery query(*this->db);
    // Prepare statement with placeholders for SQLite compatibility and security
    query.prepare(QString("INSERT INTO %1 (majorId, className) VALUES (?, ?)")
                      .arg(STDTOQSTR(this->tableName)));
    query.addBindValue(this->majorId); // Bind majorId
    query.addBindValue(QString::fromStdString(this->className)); // Bind className
    
    if (!query.exec()) {
        // qDebug() << "Insert failed:" << query.lastError().text(); // Optional: for debugging
        return false;
    }
    // Optionally, retrieve the last inserted ID if needed (especially with AUTOINCREMENT)
    // this->id = query.lastInsertId().toInt(); 
    return true;
}

bool Class::selectById(int id) {
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
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if(!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Class::updateData() {
    QSqlQuery query(*this->db);
    // Ensure column names match the SQLite table definition (e.g., majorId)
    query.prepare(QString("UPDATE %1 SET majorId = ?, className = ? WHERE id = ?")
                      .arg(STDTOQSTR(this->tableName)));
    query.addBindValue(this->majorId); // Bind majorId
    query.addBindValue(QString::fromStdString(this->className)); // Bind className
    query.addBindValue(this->id); // Bind id for the WHERE clause
    
    if (!query.exec()) {
        // qDebug() << "Update failed:" << query.lastError().text(); // Optional: for debugging
        return false;
    }
    return query.numRowsAffected() > 0; // Check if any row was actually updated
}

std::vector<DataObject*> Class::selectAll() {
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

// 新增函数实现：检查在同一专业下，指定班级名称是否已被其他班级使用
bool Class::isClassNameTakenInMajor(const std::string& classNameToCheck, int targetMajorId, int currentClassId) {
    if (!this->db || !this->db->isOpen()) {
        // 实际项目中可能需要更健壮的错误处理，例如日志记录或抛出异常
        QMessageBox::critical(nullptr, "数据库错误", "数据库未连接，无法检查班级名称。");
        return true; // 保守起见，如果数据库不可用，则认为名称已被占用
    }

    QString queryString = QString("SELECT COUNT(*) FROM %1 WHERE className = :className AND majorId = :majorId")
                              .arg(STDTOQSTR(this->tableName));
    
    if (currentClassId > 0) { // 如果提供了 currentClassId，表示我们正在更新一个现有班级，需要排除它自身
        queryString += " AND id != :currentClassId";
    }

    QSqlQuery query(*this->db);
    query.prepare(queryString);
    query.bindValue(":className", QString::fromStdString(classNameToCheck));
    query.bindValue(":majorId", targetMajorId);

    if (currentClassId > 0) {
        query.bindValue(":currentClassId", currentClassId);
    }

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0; // 如果 count > 0，说明名称已被占用
    } else {
        // 查询失败，也保守地认为名称可能已被占用，或者记录错误
        QMessageBox::critical(nullptr, "查询错误", QString("检查班级名称时发生错误: %1").arg(query.lastError().text()));
        return true; 
    }
    return false; // 默认情况下，如果查询成功且count为0，则名称未被占用
}
