#include "doctorview.h"
#include "ui_doctorview.h"
#include "idatabase.h"
#include <QMessageBox>

DoctorView::DoctorView(QWidget *parent) : QWidget(parent), ui(new Ui::DoctorView) {
    ui->setupUi(this);

    // 初始化医生信息模型
    if (IDatabase::getInstance().initDoctorModel()) {
        ui->tableView->setModel(IDatabase::getInstance().doctorTabModel);
        ui->tableView->setSelectionModel(IDatabase::getInstance().theDoctorSelection);
    }

    // 设置表格属性
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
}

DoctorView::~DoctorView()
{
    delete ui;
}



void DoctorView::on_btnAdd_clicked()
{
    int currow = IDatabase::getInstance().addNewDoctor();
    emit goDoctorEditView(currow);  // 跳转到编辑界面
}


void DoctorView::on_btnEdit_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theDoctorSelection->currentIndex();
    emit goDoctorEditView(curIndex.row());  // 跳转到编辑界面
}


void DoctorView::on_btnDelete_clicked()
{
    if (IDatabase::getInstance().deleteCurrentDoctor()) {
        QMessageBox::information(this, "成功", "医生信息已删除");
    } else {
        QMessageBox::warning(this, "错误", "删除医生信息失败");
    }
}


void DoctorView::on_btnSearch_clicked()
{
    QString filter = QString("name LIKE '%%1%'").arg(ui->txtSearch->text());
    if (IDatabase::getInstance().searchDoctor(filter)) {
        QMessageBox::information(this, "成功", "查询完成");
    } else {
        QMessageBox::warning(this, "错误", "查询失败");
    }
}

