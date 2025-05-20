#include "score.h"

Score::Score():DataObject("Score") {}

bool Score::createTable() {
    QSqlQuery query;
    return query.exec("CREATE TABLE IF NOT EXISTS `score`  ("
  "`id` int(11) NOT NULL,"
  "`stuId` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
  "`kindId` int(11) NOT NULL,"
  "`subId` int(11) NOT NULL,"
  "`score` double NOT NULL,"
 " PRIMARY KEY (`id`) USING BTREE"
") ENGINE = MyISAM CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;)");

}
bool Score::insert() {
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (id,stuId,kindId,subId,score) values(?,?,?,?,?)").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    query.addBindValue(STDTOQSTR(stuId)); 
    query.addBindValue(kindId); 
    query.addBindValue(subId); 
    query.addBindValue(score);
    return query.exec();

}
bool Score::selectById(int id) {
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (query.exec() && query.next()) {
        this->id = query.value("id").toInt();
        this->stuId = query.value("stuId").toString().toStdString(); 
        this->kindId = query.value("kindId").toInt();
        this->subId = query.value("subId").toInt(); 
        this->score = query.value("score").toDouble();
        return true;
    } 
    else {
        return false; 
    }
}
bool Score::deleteData() {
    QSqlQuery query;
    query.prepare(QString("DELETE FROM %1 WHERE id =?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(id);
    if (!query.exec()) {
       return false; 
    }
    return query.numRowsAffected() > 0;
}
bool Score::updateData() {
    QSqlQuery query;
    query.prepare(QString("UPDATE %1 SET stuId=?,kindId=?,subId=?,score=? WHERE id=?").arg(STDTOQSTR(this->tableName)));
    query.addBindValue(STDTOQSTR(stuId)); 
    query.addBindValue(kindId);
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
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM %1").arg(STDTOQSTR(this->tableName))); 
    while(query.next()) {
        Score* score = new Score();
        score->id = query.value("id").toInt();
        score->stuId = query.value("stuId").toString().toStdString();
        score->kindId = query.value("kindId").toInt();
        score->subId = query.value("subId").toInt();
        score->score = query.value("score").toDouble();
        scores.push_back(score);
    }
    return scores;
}
std::vector<DataObject*> Score::selectByQuery(QSqlQuery sql) {
    return std::vector<DataObject*>();
}
