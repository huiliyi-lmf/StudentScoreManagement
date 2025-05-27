#include "score.h"

Score::Score():DataObject("Score") {}

bool Score::insert() {
    QSqlQuery query(*this->db);
    query.prepare(QString("INSERT INTO %1 (stuId,subId,score) values(?,?,?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(stuId);
    query.addBindValue(subId); 
    query.addBindValue(score);
    return query.exec();

}
bool Score::selectById(int id) {
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
    QSqlQuery query(*this->db);
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
       return false; 
    }
    return query.numRowsAffected() > 0;
}
bool Score::updateData() {
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

