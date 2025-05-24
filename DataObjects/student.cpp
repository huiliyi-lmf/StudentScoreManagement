#include "student.h"

Student::Student():DataObject("Student") {}
bool Student::createTable() {
    QSqlQuery query(*this->db);
    return query.exec("CREATE TABLE IF NOT EXISTS `student`  ("
  "`id` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`stuName` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`classID` int(11) NOT NULL,"
  "`majorID` int(11) NOT NULL,"
  "`age` int(11) NOT NULL,"
  "`sex` char(4) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`phoneNum` char(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`address` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "PRIMARY KEY (`id`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;)");

}
bool Student::insert() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare("INSERT INTO `student` (`id`, `stuName`, `classID`, `majorID`, `age`, `sex`, `phoneNum`, `address`) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(STDTOQSTR(stuName));
    query.addBindValue(classID);
    query.addBindValue(majorID);
    query.addBindValue(age);
    query.addBindValue(STDTOQSTR(sex));
    query.addBindValue(STDTOQSTR(phoneNum));
    query.addBindValue(STDTOQSTR(address));
    return query.exec();
}
bool Student::selectById(int id) {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("SELECT * FROM %1 WHERE `id` = ?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        stuName = query.value("StuName").toString().toStdString();
        classID = query.value("classID").toInt();
        majorID = query.value("majorID").toInt();
        age = query.value("age").toInt();
        sex = query.value("sex").toString().toStdString();
        phoneNum = query.value("phoneNum").toString().toStdString();
        address = query.value("address").toString().toStdString();
        return true; 
    } 
    return false;
}
bool Student::deleteData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE `id` =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if(!query.exec()){
        return false; 
    }
    return query.numRowsAffected() > 0;

}
bool Student::updateData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("UPDATE %1 SET `stuName` =?, `classID` =?, `majorID` =?, `age` =?, `sex` =?, `phoneNum` =?, `address` =? WHERE `id` =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(stuName));
    query.addBindValue(classID);
    query.addBindValue(majorID);
    query.addBindValue(age);
    query.addBindValue(STDTOQSTR(sex));
    query.addBindValue(STDTOQSTR(phoneNum));
    query.addBindValue(STDTOQSTR(address));
    query.addBindValue(id);
    if(!query.exec()){
        return false;
    }
    return query.numRowsAffected() > 0;

}
std::vector<DataObject*> Student::selectAll() {
    this->createTable();
    QSqlQuery query(*this->db);
     query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName)));
    std::vector<DataObject*> students;
    while (query.next()) {
        Student* student = new Student();
        student->id = query.value("id").toInt();
        student->stuName = query.value("stuName").toString().toStdString();
        student->classID = query.value("classID").toInt();
        student->majorID = query.value("majorID").toInt();
        student->age = query.value("age").toInt();
        student->sex = query.value("sex").toString().toStdString();
        student->phoneNum = query.value("phoneNum").toString().toStdString();
        student->address = query.value("address").toString().toStdString();
        students.push_back(student); 
    }
    return students;
    
}

