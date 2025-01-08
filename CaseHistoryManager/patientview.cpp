#include "patientview.h"
#include "ui_patientview.h"
#include "idatabase.h"
#include <QFileDialog>
#include <QMessageBox>

PatientView::PatientView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PatientView)
{
    ui->setupUi(this);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);

    IDatabase &iDatabase = IDatabase::getInstance();
    if (iDatabase.initPatientModel()){
        ui->tableView->setModel(iDatabase.patientTabModel);
        ui->tableView->setSelectionModel(iDatabase.thePatientSelection);
    }
}

PatientView::~PatientView()
{
    delete ui;
}

void PatientView::on_btnAdd_clicked()
{
    int currow = IDatabase::getInstance().addNewPatient();
    emit goPatientEditView(currow);

}


void PatientView::on_btnSearch_clicked()
{
    QString filter = QString("name like '%%1%'").arg(ui->txtSearch->text());
    IDatabase::getInstance().searchPatient(filter);
}


void PatientView::on_addDelete_clicked()
{
    IDatabase::getInstance().deleteCurrentPatient();
}


void PatientView::on_btnEdit_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().thePatientSelection->currentIndex();

    emit goPatientEditView(curIndex.row());
}

void PatientView::on_btnExport_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "导出病人数据", "病人数据", "CSV文件 (*.csv)");
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
    out << "ID,姓名,身份证,性别,出生日期,身高,体重,手机号,创建时间\n";

    // 遍历所有病人数据并写入文件
    QSqlTableModel *model = IDatabase::getInstance().patientTabModel;
    for (int row = 0; row < model->rowCount(); ++row) {
        out << model->data(model->index(row, model->fieldIndex("ID"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("NAME"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("ID_CARD"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("SEX"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("DOB"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("HEIGHT"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("WEIGHT"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("MOBILEPHONE"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("CREATEDTIMESTAMP"))).toString() << "\n";
    }

    file.close();
    QMessageBox::information(this, "成功", "病人数据已导出到：" + fileName);
}


void PatientView::on_btnImport_clicked() {
    // 打开文件对话框，选择CSV文件
    QString fileName = QFileDialog::getOpenFileName(this, "导入病人数据", "", "CSV文件 (*.csv)");
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

    // 清空当前数据库中的所有病人数据
    QSqlTableModel *model = IDatabase::getInstance().patientTabModel;
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

        if (fields.size() < 9) {
            QMessageBox::warning(this, "错误", "文件格式不正确");
            return;
        }

        // 插入新行
        int row = model->rowCount();
        model->insertRow(row);

        // 设置数据
        model->setData(model->index(row, model->fieldIndex("ID")), fields[0].trimmed());
        model->setData(model->index(row, model->fieldIndex("NAME")), fields[1].trimmed());
        model->setData(model->index(row, model->fieldIndex("ID_CARD")), fields[2].trimmed());
        model->setData(model->index(row, model->fieldIndex("SEX")), fields[3].trimmed());
        model->setData(model->index(row, model->fieldIndex("DOB")), fields[4].trimmed());
        model->setData(model->index(row, model->fieldIndex("HEIGHT")), fields[5].trimmed());
        model->setData(model->index(row, model->fieldIndex("WEIGHT")), fields[6].trimmed());
        model->setData(model->index(row, model->fieldIndex("MOBILEPHONE")), fields[7].trimmed());
        model->setData(model->index(row, model->fieldIndex("CREATEDTIMESTAMP")), fields[8].trimmed());

        successCount++;
    }

    file.close();

    // 提交修改
    if (model->submitAll()) {
        QMessageBox::information(this, "成功", QString("病人数据已导入\n成功导入记录：%1").arg(successCount));
    } else {
        QMessageBox::warning(this, "错误", "导入病人数据失败");
    }
}
