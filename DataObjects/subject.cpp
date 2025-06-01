#include "subject.h"

Subject::Subject():DataObject("subject") {}

bool Subject::insert() 
{
    QSqlQuery query(*this->db);
    query.prepare(QString("INSERT INTO %1 (`majorId`, `subName`) VALUES (?,?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(majorId);
    query.addBindValue(STDTOQSTR(subName));
    return query.exec();
}

bool Subject::selectById(int id)
{
    QSqlQuery query(*this->db);
    query.prepare(QString("SELECT * FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        this->majorId=query.value("majorId").toInt();
        this->subName = query.value("subName").toString().toStdString();
        return true; 
    }
    else {
        return false; 
    }
}
bool Subject::deleteData()
{
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id=?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
bool Subject::updateData()
{
    QSqlQuery query(*this->db);
    query.prepare(QString("UPDATE %1 SET majorId=?,subName =? WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(majorId);
    query.addBindValue(STDTOQSTR(subName));
    query.addBindValue(id);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0;
}
std::vector<DataObject*> Subject::selectAll()
{
    std::vector<DataObject*> subjects;
    QSqlQuery query(*this->db);
    query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName)));
    while (query.next()) {
        Subject* subject = new Subject();
        subject->id = query.value("id").toInt();
        subject->majorId=query.value("majorId").toInt();
        subject->subName = query.value("subName").toString().toStdString();
        subjects.push_back(subject); 
    }
    return subjects;
}
bool Subject::isSubjectNameTakenInMajor(const std::string& subjectNameToCheck, int targetMajorId, int currentSubjectId ){
    if (!this->db || !this->db->isOpen()) {
        QMessageBox::critical(nullptr, "数据库错误", "数据库未连接，无法检查班级名称。");
        return true; 
    }

    QString queryString = QString("SELECT COUNT(*) FROM %1 WHERE subName = :subName AND majorId = :majorId")
                              .arg(STDTOQSTR(this->tableName));
    
    if (currentSubjectId > 0) { 
        queryString += " AND id != :currentSubjectId";
    }

    QSqlQuery query(*this->db);
    query.prepare(queryString);
    query.bindValue(":subName", QString::fromStdString(subjectNameToCheck));
    query.bindValue(":majorId", targetMajorId);

    if (currentSubjectId > 0) {
        query.bindValue(":currentSubjectId", currentSubjectId);
    }

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0; // 如果 count > 0，说明名称已被占用
    } else {
        // 查询失败，也保守地认为名称可能已被占用，或者记录错误
        QMessageBox::critical(nullptr, "查询错误", QString("检查学科名称时发生错误: %1").arg(query.lastError().text()));
        return true; 
    }
    return false; // 默认情况下，如果查询成功且count为0，则名称未被占用
}

    
