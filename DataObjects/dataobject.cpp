#include "dataobject.h"

QSqlDatabase* DataObject::db = nullptr;

DataObject::DataObject(std::string name) : tableName(name) {

}

bool DataObject::openDataBase() {
    if(db != nullptr) {
        return db->isOpen();
    }
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(DATABASE_NAME);
    db->open();
    return db->isOpen();
}

bool DataObject::closeDataBase() {
    if(db != nullptr) {
        db->close();
    }
    return true;
}
