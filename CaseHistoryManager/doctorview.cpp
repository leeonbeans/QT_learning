#include "doctorview.h"
#include "ui_doctorview.h"
#include "idatabase.h"
#include <QMessageBox>
#include <QFileDialog>

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
    ui->tableView->setSortingEnabled(true);
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


void DoctorView::on_btnExport_clicked() {
    // 打开文件对话框，选择保存路径
    QString fileName = QFileDialog::getSaveFileName(this, "导出医生数据", "医生数据", "CSV文件 (*.csv)");
    if (fileName.isEmpty()) {
        return;  // 用户取消导出
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);  // 指定编码为UTF-8
    out.setGenerateByteOrderMark(true);  // 添加BOM头

    // 写入表头
    out << "用户名,密码,姓名,性别,年龄,执业证书号\n";

    // 遍历所有医生数据并写入文件
    QSqlTableModel *model = IDatabase::getInstance().doctorTabModel;
    for (int row = 0; row < model->rowCount(); ++row) {
        out << model->data(model->index(row, model->fieldIndex("username"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("password"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("name"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("gender"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("age"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("license_number"))).toString() << "\n";
    }

    file.close();
    QMessageBox::information(this, "成功", "医生数据已导出到：" + fileName);
}

void DoctorView::on_btnImport_clicked() {
    // 打开文件对话框，选择CSV文件
    QString fileName = QFileDialog::getOpenFileName(this, "导入医生数据", "", "CSV文件 (*.csv)");
    if (fileName.isEmpty()) {
        return;  // 用户取消导入
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);  // 指定编码为UTF-8

    // 读取表头（跳过第一行）
    QString header = in.readLine();

    // 清空当前数据库中的所有医生数据
    QSqlTableModel *model = IDatabase::getInstance().doctorTabModel;
    model->removeRows(0, model->rowCount());  // 删除所有行
    if (!model->submitAll()) {
        QMessageBox::warning(this, "错误", "清空数据库失败");
        return;
    }

    // 逐行读取数据并插入数据库
    int successCount = 0;  // 成功导入的记录数

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() < 6) {
            QMessageBox::warning(this, "错误", "文件格式不正确");
            return;
        }

        // 插入新行
        int row = model->rowCount();
        model->insertRow(row);

        // 设置数据
        model->setData(model->index(row, model->fieldIndex("username")), fields[0].trimmed());
        model->setData(model->index(row, model->fieldIndex("password")), fields[1].trimmed());
        model->setData(model->index(row, model->fieldIndex("name")), fields[2].trimmed());
        model->setData(model->index(row, model->fieldIndex("gender")), fields[3].trimmed());
        model->setData(model->index(row, model->fieldIndex("age")), fields[4].trimmed());
        model->setData(model->index(row, model->fieldIndex("license_number")), fields[5].trimmed());

        successCount++;
    }

    file.close();

    // 提交修改
    if (model->submitAll()) {
        QMessageBox::information(this, "成功", QString("医生数据已导入\n成功导入记录：%1").arg(successCount));
    } else {
        QMessageBox::warning(this, "错误", "导入医生数据失败");
    }
}

