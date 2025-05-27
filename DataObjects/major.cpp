#include "major.h"

Major::Major(): DataObject("Major") {}

bool Major::insert() {
    QSqlQuery query(*this->db);
    query.prepare(QString("INSERT INTO %1 (majorName) values (?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(this->majorName));
    return query.exec();
   

}

bool Major::selectById(int id) {
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
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
        return false; 
    }
    return query.numRowsAffected() > 0;
}
bool Major::updateData() {
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
    std::vector<DataObject*> majors;
    QSqlQuery query(*this->db);
    if(query.exec(QString("SELECT * FROM %1 ORDER BY id ASC").arg(STDTOQSTR(this->tableName)))) {
    while (query.next()) {
        Major* major = new Major();
        major->id = query.value("id").toInt();
       major->majorName = query.value("majorName").toString().toStdString();
       majors.push_back(major);
    }}
    else{
        qWarning()<<"获取全部专业失败："<<query.lastError().text();
    }
    return majors;
}
bool Major::isNameExists(QWidget* parent) {
    QSqlQuery query(*DataObject::getDb());
    query.prepare("SELECT COUNT(*) FROM major WHERE majorName = ?");
    query.addBindValue(QString::fromStdString(this->majorName)); // 使用当前对象的majorName

    if (!query.exec()) {
        QMessageBox::warning(parent, "错误", "查询专业名称是否存在失败: " + query.lastError().text());
        return false; // 发生错误时，可以认为检查未通过或按需处理
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            QMessageBox::warning(parent, "警告", "该专业名称已存在！");
            return true;
        }
    }
    return false;
}

// 新增的 isNameTakenByOther 方法
bool Major::isNameTakenByOther(const std::string& nameToCheck, int currentMajorId, QWidget* parent) {
    QSqlQuery query(*DataObject::getDb());
    // 查询数据库中是否存在某个专业，其名称为 nameToCheck，且其ID不等于 currentMajorId
    query.prepare("SELECT COUNT(*) FROM major WHERE majorName = ? AND id != ?");
    query.addBindValue(QString::fromStdString(nameToCheck));
    query.addBindValue(currentMajorId);

    if (!query.exec()) {
        QMessageBox::critical(parent, "数据库错误", "检查专业名称冲突时发生错误: " + query.lastError().text());
        // 在数据库查询失败时，为安全起见，可以返回true（认为名称已被占用）以防止潜在的重复。
        return true;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0; // 如果count > 0，说明名称已被其他专业占用，返回true
    }
    // 理论上，如果query.exec()成功，总会进入query.next()。为保险起见：
    return false;
}
