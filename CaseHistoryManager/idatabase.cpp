#include "idatabase.h"
#include <QUuid>

void IDatabase::initDatabase()
{

    database = QSqlDatabase::addDatabase("QSQLITE");
    QString aFile = "C:/Users/weii/Documents/Navicat/Premium/profiles/Lab4.db";
    database.setDatabaseName(aFile);

    if(!database.open()){
        qDebug()<<"数据库异常";
    }else{

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

QString IDatabase::doctorLogin(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT username, password FROM doctor WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec()) {
        qDebug() << "Login query failed:" << query.lastError();
        return "loginFail";
    }

    if (query.next()) {
        QString storedPassword = query.value("password").toString();
        if (storedPassword == password) {
            return "loginOK";
        } else {
            return "wrongPassword";
        }
    } else {
        return "loginFail";
    }
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

bool IDatabase::initDoctorModel() {
    doctorTabModel = new QSqlTableModel(this, database);
    doctorTabModel->setTable("doctor");
    doctorTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    doctorTabModel->setSort(doctorTabModel->fieldIndex("name"), Qt::AscendingOrder);
    if (!doctorTabModel->select()) {
        qDebug() << "Failed to initialize doctor model:" << doctorTabModel->lastError();
        return false;
    }

    theDoctorSelection = new QItemSelectionModel(doctorTabModel);
    return true;
}

int IDatabase::addNewDoctor() {
    doctorTabModel->insertRow(doctorTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = doctorTabModel->index(doctorTabModel->rowCount()-1,1);
    return curIndex.row();
}

bool IDatabase::deleteCurrentDoctor() {
    QModelIndex currentIndex = theDoctorSelection->currentIndex();
    if (!doctorTabModel->removeRow(currentIndex.row())) {
        qDebug() << "Failed to delete doctor:" << doctorTabModel->lastError();
        return false;
    }
    return doctorTabModel->submitAll();
}

bool IDatabase::searchDoctor(const QString &filter) {
    doctorTabModel->setFilter(filter);
    return doctorTabModel->select();
}

bool IDatabase::submitDoctorEdit() {
    return doctorTabModel->submitAll();
}

void IDatabase::revertDoctorEdit() {
    doctorTabModel->revertAll();
}

bool IDatabase::initMedicineModel() {
    medicineTabModel = new QSqlTableModel(this, database);
    medicineTabModel->setTable("medicine");
    medicineTabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    medicineTabModel->setSort(medicineTabModel->fieldIndex("name"), Qt::AscendingOrder);
    if (!medicineTabModel->select()) {
        qDebug() << "Failed to initialize medicine model:" << medicineTabModel->lastError();
        return false;
    }

    theMedicineSelection = new QItemSelectionModel(medicineTabModel);
    return true;
}

int IDatabase::addNewMedicine() {
    medicineTabModel->insertRow(medicineTabModel->rowCount(),QModelIndex());
    QModelIndex curIndex = medicineTabModel->index(medicineTabModel->rowCount()-1,1);
    return curIndex.row();
}

bool IDatabase::deleteCurrentMedicine() {
    QModelIndex currentIndex = theMedicineSelection->currentIndex();
    if (!medicineTabModel->removeRow(currentIndex.row())) {
        qDebug() << "Failed to delete medicine:" << medicineTabModel->lastError();
        return false;
    }
    return medicineTabModel->submitAll();
}

bool IDatabase::searchMedicine(const QString &filter) {
    medicineTabModel->setFilter(filter);
    return medicineTabModel->select();
}

bool IDatabase::submitMedicineEdit() {
    return medicineTabModel->submitAll();
}

void IDatabase::revertMedicineEdit() {
    medicineTabModel->revertAll();
}
