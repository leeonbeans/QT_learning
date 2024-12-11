#include "idatabase.h"
#include <QUuid>

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

bool IDatabase::searchPatient(QString filter)
{
    patientTabModel->setFilter(filter);
    return patientTabModel->select();
}

bool IDatabase::deleteCurrentPatient()
{
    QModelIndex curIndex = thePatientSelection->currentIndex();
    patientTabModel->removeRow(curIndex.row());
    patientTabModel->submitAll();
    patientTabModel->select();
}

bool IDatabase::submitPatientEdit()
{
    return patientTabModel->submitAll();
}

void IDatabase::revertPatientEdit()
{
    patientTabModel->revertAll();
}

int IDatabase::addNewPatient()
{
    patientTabModel->insertRow(patientTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = patientTabModel->index(patientTabModel->rowCount()-1,1);

    int curRecNo = curIndex.row();
    QSqlRecord curRec = patientTabModel->record(curRecNo);
    curRec.setValue("CREATEDTIMESTAMP",QDateTime::currentDateTime().toString("yyyy-MM-dd"));
    curRec.setValue("ID", QUuid::createUuid().toString(QUuid::WithoutBraces));

    patientTabModel->setRecord(curRecNo, curRec);

    return curIndex.row();
}

bool IDatabase::initPatientModel()
{
    patientTabModel = new QSqlTableModel(this, database);
    patientTabModel->setTable("patient");
    patientTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    patientTabModel->setSort(patientTabModel->fieldIndex("NAME"),Qt::AscendingOrder);
    if(!(patientTabModel->select()))
        return false;

    thePatientSelection = new QItemSelectionModel(patientTabModel);
    return true;
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
