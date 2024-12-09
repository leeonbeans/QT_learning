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

QString IDatabase::userLogin(QString username, QString password)
{
    QSqlQuery query;
    query.prepare("select USERNAME,PASSWORD from user where USERNAME = :USER");
    query.bindValue(":USER",username);
    query.exec();
    if(query.first() && query.value("USERNAME").isValid()){
        QString pd = query.value("PASSWORD").toString();
        if(pd == password)
        {
            return "loginOK";
        }
        else{
            qDebug()<<"sadasd";
            return "wrongPassword";
        }
    }
    else
    {
        return "loginFail";
    }
}

IDatabase::IDatabase(QObject *parent): QObject{parent}
{
    initDatabase();
}
