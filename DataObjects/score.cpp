#include "score.h"

Score::Score():DataObject("Score") {}

bool Score::createTable() {
    QSqlQuery query(*this->db);
    return query.exec("CREATE TABLE IF NOT EXISTS score ("
  "id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
  "stuId INTEGER NOT NULL,"
  "subId INTEGER NOT NULL,"
  "score REAL NOT NULL"
");");

}
bool Score::insert() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("INSERT INTO %1 (stuId,subId,score) values(?,?,?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(stuId);
    query.addBindValue(subId); 
    query.addBindValue(score);
    return query.exec();

}
bool Score::selectById(int id) {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("SELECT * FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        this->stuId = query.value("stuId").toInt();
        this->subId = query.value("subId").toInt(); 
        this->score = query.value("score").toDouble();
        return true;
    } 
    else {
        return false; 
    }
}
bool Score::deleteData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
       return false; 
    }
    return query.numRowsAffected() > 0;
}
bool Score::updateData() {
    this->createTable();
    QSqlQuery query(*this->db);
    query.prepare(QString("UPDATE %1 SET stuId=?,subId=?,score=? WHERE id=?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(stuId);
    query.addBindValue(subId); 
    query.addBindValue(score);
    query.addBindValue(id);
    if (!query.exec()) {
        return false; 
    }
    return query.numRowsAffected() > 0;
}
std::vector<DataObject*> Score::selectAll() {
    this->createTable();
    std::vector<DataObject*> scores;
    QSqlQuery query(*this->db);
    query.exec(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName))); 
    while(query.next()) {
        Score* score = new Score();
        score->id = query.value("id").toInt();
        score->stuId = query.value("stuId").toInt();
        score->subId = query.value("subId").toInt();
        score->score = query.value("score").toDouble();
        scores.push_back(score);
    }
    return scores;
}

