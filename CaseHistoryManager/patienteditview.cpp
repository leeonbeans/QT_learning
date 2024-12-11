#include "patienteditview.h"
#include "ui_patienteditview.h"
#include "idatabase.h"
#include <QSqlTableModel>


PatientEditView::PatientEditView(QWidget *parent, int index)
    : QWidget(parent)
    , ui(new Ui::PatientEditView)
{
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    QSqlTableModel *tabModel = IDatabase::getInstance().patientTabModel;
    dataMapper->setModel(IDatabase::getInstance().patientTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbEditID, tabModel->fieldIndex("ID"));
    dataMapper->addMapping(ui->dbEditName, tabModel->fieldIndex("NAME"));
    dataMapper->addMapping(ui->dbEditIDCard, tabModel->fieldIndex("ID_CARD"));
    dataMapper->addMapping(ui->dbSpinBoxHeight, tabModel->fieldIndex("HEIGHT"));
    dataMapper->addMapping(ui->dbSpinBoxWeight, tabModel->fieldIndex("WEIGHT"));
    dataMapper->addMapping(ui->dbEditPhone, tabModel->fieldIndex("MOBILEPHONE"));
    dataMapper->addMapping(ui->dbDateEditBirthday, tabModel->fieldIndex("DOB"));
    dataMapper->addMapping(ui->dbComboSex, tabModel->fieldIndex("SEX"));
    dataMapper->addMapping(ui->dbEditCreateTime, tabModel->fieldIndex("CREATEDTIMESTAMP"));

    dataMapper->setCurrentIndex(index);
}

PatientEditView::~PatientEditView()
{
    delete ui;
}

void PatientEditView::on_pushButton_clicked()
{
    IDatabase::getInstance().submitPatientEdit();

    emit goPreviousView();
}


void PatientEditView::on_pushButton_2_clicked()
{
    IDatabase::getInstance().revertPatientEdit();

    emit goPreviousView();
}

