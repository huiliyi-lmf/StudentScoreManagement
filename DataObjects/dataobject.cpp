#include "dataobject.h"

DataObject::DataObject(std::string name) : tableName(name), isDataBaseOpen(false) {
    isDataBaseOpen = DataObject::openDataBase();
}

bool DataObject::openDataBase() {
    if(db != nullptr) {
        return db->isOpen();
    }
    db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    db->setDatabaseName(DATABASE_NAME);
    return db->isOpen();
}

bool DataObject::closeDataBase() {
    if(db != nullptr) {
        db->close();
    }
    return true;
}
