#include "appointmentview.h"
#include "ui_appointmentview.h"
#include "idatabase.h"
#include <QMessageBox>

AppointmentView::AppointmentView(QWidget *parent) : QWidget(parent), ui(new Ui::AppointmentView) {
    ui->setupUi(this);

    // 初始化就诊记录模型
    if (IDatabase::getInstance().initAppointmentModel()) {
        ui->tableView->setModel(IDatabase::getInstance().appointmentTabModel);
        ui->tableView->setSelectionModel(IDatabase::getInstance().theAppointmentSelection);
    }

    // 设置表格属性
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
}

AppointmentView::~AppointmentView()
{
    delete ui;
}

void AppointmentView::on_btnAdd_clicked()
{
    int currow = IDatabase::getInstance().addNewAppointment();
    emit goAppointmentEditView(currow);  // 跳转到编辑界面
}


void AppointmentView::on_btnEdit_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theAppointmentSelection->currentIndex();
    emit goAppointmentEditView(curIndex.row());  // 跳转到编辑界面
}


void AppointmentView::on_btnDelete_clicked()
{
    if (IDatabase::getInstance().deleteCurrentAppointment()) {
        QMessageBox::information(this, "成功", "就诊记录已删除");
    } else {
        QMessageBox::warning(this, "错误", "删除就诊记录失败");
    }
}


void AppointmentView::on_btnSearch_clicked() {
    QString filter = QString("patient_name LIKE '%%1%'").arg(ui->txtSearch->text());
    if (IDatabase::getInstance().searchAppointment(filter)) {
        QMessageBox::information(this, "成功", "查询完成");
    } else {
        QMessageBox::warning(this, "错误", "查询失败");
    }
}
