#include "appointmentview.h"
#include "ui_appointmentview.h"
#include "idatabase.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

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
    ui->tableView->setSortingEnabled(true);
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

void AppointmentView::on_btnExport_clicked()
{
    // 打开文件对话框，选择保存路径
    QString fileName = QFileDialog::getSaveFileName(this, "导出就诊记录", "就诊记录", "CSV文件 (*.csv)");
    if (fileName.isEmpty()) {
        return;  // 用户取消导出
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringEncoder::Utf8);  // 指定编码为UTF-8
    out.setGenerateByteOrderMark(true);  // 添加BOM头

    // 写入表头
    out << "病人姓名,医生姓名,就诊日期,诊断结果,开具药品\n";

    // 遍历所有就诊记录并写入文件
    QSqlTableModel *model = IDatabase::getInstance().appointmentTabModel;
    for (int row = 0; row < model->rowCount(); ++row) {
        out << model->data(model->index(row, model->fieldIndex("patient_name"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("doctor_name"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("appointment_date"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("diagnosis"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("medication"))).toString() << "\n";
    }

    file.close();
    QMessageBox::information(this, "成功", "就诊记录已导出到：" + fileName);
}


void AppointmentView::on_btnImport_clicked()
{
    // 打开文件对话框，选择CSV文件
    QString fileName = QFileDialog::getOpenFileName(this, "导入就诊记录", "", "CSV文件 (*.csv)");
    if (fileName.isEmpty()) {
        return;  // 用户取消导入
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringEncoder::Utf8);  // 指定编码为UTF-8

    // 读取表头（跳过第一行）
    QString header = in.readLine();

    // 逐行读取数据并插入数据库
    QSqlTableModel *model = IDatabase::getInstance().appointmentTabModel;
    int successCount = 0;  // 成功导入的记录数

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() < 5) {
            QMessageBox::warning(this, "错误", "文件格式不正确");
            return;
        }

        // 插入新行
        int row = model->rowCount();
        model->insertRow(row);

        // 设置数据
        model->setData(model->index(row, model->fieldIndex("patient_name")), fields[0].trimmed());
        model->setData(model->index(row, model->fieldIndex("doctor_name")), fields[1].trimmed());
        model->setData(model->index(row, model->fieldIndex("appointment_date")), fields[2].trimmed());
        model->setData(model->index(row, model->fieldIndex("diagnosis")), fields[3].trimmed());
        model->setData(model->index(row, model->fieldIndex("medication")), fields[4].trimmed());

        successCount++;
    }

    file.close();

    // 提交修改
    if (model->submitAll()) {
        QMessageBox::information(this, "成功", QString("就诊记录已导入\n成功导入记录：%1").arg(successCount));
    } else {
        QMessageBox::warning(this, "错误", "导入就诊记录失败");
    }
}

