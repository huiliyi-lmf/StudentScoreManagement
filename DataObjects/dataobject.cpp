#include "dataobject.h"

QSqlDatabase* DataObject::db = nullptr;

DataObject::DataObject(std::string name) : tableName(name) {

}

bool DataObject::openDataBase() {
    if(db != nullptr) {
        return db->isOpen();
    }

    if(!QFile::exists(DATABASE_NAME)) {
        QFile dataFile(":/db/data/init_student.db");
        if(!dataFile.open(QIODevice::ReadOnly)) {
            return false;
        }
        QByteArray data = dataFile.readAll();
        dataFile.close();
        QSaveFile file(DATABASE_NAME);
        if(!file.open(QIODevice::WriteOnly)) {
            return false;
        }
        if (file.write(data) != data.size()) {
            file.cancelWriting();
            return false;
        }
        if(!file.commit()) {
            return false;
        }
    }

    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(DATABASE_NAME);
    db->open();
    if(db->isOpen()) {
        QSqlQuery query;
        return query.exec("PRAGMA foreign_keys = ON;");
    } else {
        return false;
    }
}

bool DataObject::closeDataBase() {
    if(db != nullptr) {
        db->close();
    }
    return true;
}
