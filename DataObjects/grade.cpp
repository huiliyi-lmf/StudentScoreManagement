#include "grade.h"

Grade::Grade(): DataObject("Grade") {}

bool Grade::createTable() {
    QSqlQuery query ;
    return query.exec("CREATE TABLE IF NOT EXISTS `grade`  ("
  "`gradeId` int(11) NOT NULL,"
  "`gradeName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "PRIMARY KEY (`gradeID`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;");
}
bool Grade::insert() {
    QSqlQuery query;
    return query.exec(QString("INSERT INTO %1 (gradeID, gradeName) values ('%2', '%3')").arg(STDTOQSTR(this->tableName)).arg(gradeId).arg(STDTOQSTR(this->gradeName)));

}

bool Grade::selectById(int id) {
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE gradeID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id); 
    if (query.exec() && query.next()) {
        this->gradeId = query.value("gradeID").toInt();
        this->gradeName = query.value("gradeName").toString().toStdString();
        return true; 
    }
    else {
        return false; 
    }
}
bool Grade::deleteData() {
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE gradeID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(gradeId); 
    if (!query.exec()) {
        return false; 
    }
    return query.numRowsAffected() > 0;
}
bool Grade::updateData() {
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET gradeName =? WHERE gradeID =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(gradeName));
    query.addBindValue(gradeId);
    if (!query.exec()) {
        return false;
    }
    return query.numRowsAffected() > 0; 
}
std::vector<DataObject*> Grade::selectAll() {
    std::vector<DataObject*> grades;
    QSqlQuery query;
    query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName))); 
    while (query.next()) {
        Grade* grade = new Grade();
        grade->gradeId = query.value("gradeID").toInt();
        grade->gradeName = query.value("gradeName").toString().toStdString(); 
        grades.push_back(grade);
    }
    return grades;
}
std::vector<DataObject*> Grade::selectByQuery(QSqlQuery sql) {
    std::vector<DataObject*> grades; 
}
