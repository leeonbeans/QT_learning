#include "idatabase.h"

void IDatabase::initDatabase()
{

    database = QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = "C:/Users/weii/Documents/Navicat/Premium/profiles/Lab4.db";
    database.setDatabaseName(aFile);

    if(!database.open()){

    }else{
        qDebug()<<"xxx";
    }
}

IDatabase::IDatabase(QObject *parent): QObject{parent}
{
    initDatabase();
}
