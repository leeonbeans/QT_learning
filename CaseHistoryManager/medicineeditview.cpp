#include "medicineeditview.h"
#include "ui_medicineeditview.h"
#include "idatabase.h"
#include <QSqlTableModel>
#include <QMessageBox>

MedicineEditView::MedicineEditView(QWidget *parent, int index) : QWidget(parent), ui(new Ui::MedicineEditView) {
    ui->setupUi(this);

    dataMapper = new QDataWidgetMapper();
    dataMapper->setModel(IDatabase::getInstance().medicineTabModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    // 绑定控件到数据库字段
    dataMapper->addMapping(ui->txtName, IDatabase::getInstance().medicineTabModel->fieldIndex("name"));
    dataMapper->addMapping(ui->txtDosage, IDatabase::getInstance().medicineTabModel->fieldIndex("dosage"));
    dataMapper->addMapping(ui->spinStock, IDatabase::getInstance().medicineTabModel->fieldIndex("stock"));

    dataMapper->setCurrentIndex(index);
}

void MedicineEditView::on_btnSave_clicked() {
    if (IDatabase::getInstance().submitMedicineEdit()) {
        QMessageBox::information(this, "成功", "药品信息已保存");
        emit goPreviousView();
    } else {
        QMessageBox::warning(this, "错误", "保存药品信息失败");
    }
}

void MedicineEditView::on_btnCancel_clicked() {
    IDatabase::getInstance().revertMedicineEdit();
    emit goPreviousView();
}

MedicineEditView::~MedicineEditView()
{
    delete ui;
}
