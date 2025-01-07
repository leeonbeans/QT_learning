#include "doctoreditview.h"
#include "ui_doctoreditview.h"
#include "idatabase.h"
#include <QMessageBox>

DoctorEditView::DoctorEditView(QWidget *parent, int index) : QWidget(parent), ui(new Ui::DoctorEditView) {
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    dataMapper->setModel(IDatabase::getInstance().doctorTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    // 绑定控件到数据库字段
    dataMapper->addMapping(ui->txtUsername, IDatabase::getInstance().doctorTabModel->fieldIndex("username"));
    dataMapper->addMapping(ui->txtPassword, IDatabase::getInstance().doctorTabModel->fieldIndex("password"));
    dataMapper->addMapping(ui->txtName, IDatabase::getInstance().doctorTabModel->fieldIndex("name"));
    dataMapper->addMapping(ui->cbGender, IDatabase::getInstance().doctorTabModel->fieldIndex("gender"));
    dataMapper->addMapping(ui->spinAge, IDatabase::getInstance().doctorTabModel->fieldIndex("age"));
    dataMapper->addMapping(ui->txtLicense, IDatabase::getInstance().doctorTabModel->fieldIndex("license_number"));

    dataMapper->setCurrentIndex(index);
}

DoctorEditView::~DoctorEditView() {
    delete ui;
}

void DoctorEditView::on_btnSave_clicked() {
    // 检查用户名和密码是否为空
    if (ui->txtUsername->text().isEmpty() || ui->txtPassword->text().isEmpty()) {
        QMessageBox::warning(this, "错误", "用户名和密码不能为空");
        return;
    }

    if (IDatabase::getInstance().submitDoctorEdit()) {
        QMessageBox::information(this, "成功", "医生信息已保存");
        emit goPreviousView();
    } else {
        QMessageBox::warning(this, "错误", "保存医生信息失败");
    }
}

void DoctorEditView::on_btnCancel_clicked() {
    IDatabase::getInstance().revertDoctorEdit();
    emit goPreviousView();
}
