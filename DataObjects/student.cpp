#include "student.h"

Student::Student():DataObject("Student") {}
bool Student::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS `student`  ("
  "`id` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`stuName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`classID` int(11) NOT NULL,"
  "`gradeID` int(11) NOT NULL,"
  "`age` int(11) NOT NULL,"
  "`sex` char(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`nation` char(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`address` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "PRIMARY KEY (`id`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;)");

}
bool Student::insert() {
    QSqlQuery query;
    query.prepare("INSERT INTO `student` (`id`, `stuName`, `classID`, `gradeID`, `age`, `sex`, `nation`, `address`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(STDTOQSTR(stuName));
    query.addBindValue(classID);
    query.addBindValue(gradeID);
    query.addBindValue(age);
    query.addBindValue(STDTOQSTR(sex));
    query.addBindValue(STDTOQSTR(nation));
    query.addBindValue(STDTOQSTR(address));
    return query.exec();
}
bool Student::selectById(int id) {
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE `id` = ?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        stuName = query.value("StuName").toString().toStdString();
        classID = query.value("classID").toInt();
        gradeID = query.value("gradeID").toInt();
        age = query.value("age").toInt();
        sex = query.value("sex").toString().toStdString();
        nation = query.value("nation").toString().toStdString();
        address = query.value("address").toString().toStdString();
        return true; 
    } 
    return false;
}
bool Student::deleteData() {
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE `id` =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if(!query.exec()){
        return false; 
    }
    return query.numRowsAffected() > 0;

}
bool Student::updateData() {
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET `stuName` =?, `classID` =?, `gradeID` =?, `age` =?, `sex` =?, `nation` =?, `address` =? WHERE `id` =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(stuName));
    query.addBindValue(classID);
    query.addBindValue(gradeID);
    query.addBindValue(age);
    query.addBindValue(STDTOQSTR(sex));
    query.addBindValue(STDTOQSTR(nation));
    query.addBindValue(STDTOQSTR(address));
    query.addBindValue(id);
    if(!query.exec()){
        return false;
    }
    return query.numRowsAffected() > 0;

}
std::vector<DataObject*> Student::selectAll() {
    QSqlQuery query;
     query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName)));
    std::vector<DataObject*> students;
    while (query.next()) {
        Student* student = new Student();
        student->id = query.value("id").toInt();
        student->stuName = query.value("stuName").toString().toStdString();
        student->classID = query.value("classID").toInt();
        student->gradeID = query.value("gradeID").toInt();
        student->age = query.value("age").toInt();
        student->sex = query.value("sex").toString().toStdString();
        student->nation = query.value("nation").toString().toStdString();
        student->address = query.value("address").toString().toStdString();
        students.push_back(student); 
    }
    return students;
    
}

