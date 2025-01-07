#include "appointmenteditview.h"
#include "ui_appointmenteditview.h"
#include "idatabase.h"
#include <QMessageBox>

AppointmentEditView::AppointmentEditView(QWidget *parent, int index) : QWidget(parent), ui(new Ui::AppointmentEditView) {
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    dataMapper->setModel(IDatabase::getInstance().appointmentTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    // 绑定控件到数据库字段
    dataMapper->addMapping(ui->txtPatientName, IDatabase::getInstance().appointmentTabModel->fieldIndex("patient_name"));
    dataMapper->addMapping(ui->txtDoctorName, IDatabase::getInstance().appointmentTabModel->fieldIndex("doctor_name"));
    dataMapper->addMapping(ui->dateEdit, IDatabase::getInstance().appointmentTabModel->fieldIndex("appointment_date"));
    dataMapper->addMapping(ui->txtDiagnosis, IDatabase::getInstance().appointmentTabModel->fieldIndex("diagnosis"));
    dataMapper->addMapping(ui->txtMedication, IDatabase::getInstance().appointmentTabModel->fieldIndex("medication"));

    dataMapper->setCurrentIndex(index);
}

void AppointmentEditView::on_btnSave_clicked() {
    if (IDatabase::getInstance().submitAppointmentEdit()) {
        QMessageBox::information(this, "成功", "就诊记录已保存");
        emit goPreviousView();
    } else {
        QMessageBox::warning(this, "错误", "保存就诊记录失败");
    }
}

void AppointmentEditView::on_btnCancel_clicked() {
    IDatabase::getInstance().revertAppointmentEdit();
    emit goPreviousView();
}

AppointmentEditView::~AppointmentEditView()
{
    delete ui;
}
