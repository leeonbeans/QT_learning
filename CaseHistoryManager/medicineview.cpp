#include "medicineview.h"
#include "ui_medicineview.h"
#include <idatabase.h>
#include <QMessageBox>
#include <QFileDialog>

MedicineView::MedicineView(QWidget *parent) : QWidget(parent), ui(new Ui::MedicineView) {
    ui->setupUi(this);

    // 初始化药品信息模型
    if (IDatabase::getInstance().initMedicineModel()) {
        ui->tableView->setModel(IDatabase::getInstance().medicineTabModel);
        ui->tableView->setSelectionModel(IDatabase::getInstance().theMedicineSelection);
    }

    // 设置表格属性
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSortingEnabled(true);
}

MedicineView::~MedicineView()
{
    delete ui;
}

void MedicineView::on_btnAdd_clicked()
{
    int currow = IDatabase::getInstance().addNewMedicine();
    emit goMedicineEditView(currow);  // 跳转到编辑界面
}



void MedicineView::on_btnEdit_clicked()
{
    QModelIndex curIndex = IDatabase::getInstance().theMedicineSelection->currentIndex();
    emit goMedicineEditView(curIndex.row());  // 跳转到编辑界面
}



void MedicineView::on_btnDelete_clicked()
{
    if (IDatabase::getInstance().deleteCurrentMedicine()) {
        QMessageBox::information(this, "成功", "药品信息已删除");
    } else {
        QMessageBox::warning(this, "错误", "删除药品信息失败");
    }
}


void MedicineView::on_btnSearch_clicked()
{
    QString filter = QString("name LIKE '%%1%'").arg(ui->txtSearch->text());
    if (IDatabase::getInstance().searchMedicine(filter)) {
        QMessageBox::information(this, "成功", "查询完成");
    } else {
        QMessageBox::warning(this, "错误", "查询失败");
    }
}


void MedicineView::on_btnCheckStock_clicked()
{
    QSqlTableModel *model = IDatabase::getInstance().medicineTabModel;
    bool hasLowStock = false;

    for (int row = 0; row < model->rowCount(); ++row) {
        int stock = model->data(model->index(row, model->fieldIndex("stock"))).toInt();
        if (stock < 10) {  // 假设库存低于10时触发警告
            QString medicineName = model->data(model->index(row, model->fieldIndex("name"))).toString();
            QMessageBox::warning(this, "库存警告", QString("药品【%1】库存不足，当前库存：%2").arg(medicineName).arg(stock));
            hasLowStock = true;
        }
    }

    if (!hasLowStock) {
        QMessageBox::information(this, "库存检查", "所有药品库存充足");
    }
}


void MedicineView::on_btnExport_clicked()
{
    // 打开文件对话框，选择保存路径
    QString fileName = QFileDialog::getSaveFileName(this, "导出药品信息", "药品记录", "CSV文件 (*.csv)");
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
    out << "药品名称,剂量,库存数量\n";

    // 遍历所有药品信息并写入文件
    QSqlTableModel *model = IDatabase::getInstance().medicineTabModel;
    for (int row = 0; row < model->rowCount(); ++row) {
        out << model->data(model->index(row, model->fieldIndex("name"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("dosage"))).toString() << ","
            << model->data(model->index(row, model->fieldIndex("stock"))).toString() << "\n";
    }

    file.close();
    QMessageBox::information(this, "成功", "药品信息已导出到：" + fileName);
}


void MedicineView::on_btnImport_clicked() {
    // 打开文件对话框，选择CSV文件
    QString fileName = QFileDialog::getOpenFileName(this, "导入药品信息", "", "CSV文件 (*.csv)");
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
    QSqlTableModel *model = IDatabase::getInstance().medicineTabModel;
    int successCount = 0;  // 成功导入的记录数
    int updateCount = 0;   // 更新的记录数

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() < 3) {
            QMessageBox::warning(this, "错误", "文件格式不正确");
            return;
        }

        QString name = fields[0].trimmed();  // 药品名称
        QString dosage = fields[1].trimmed();  // 剂量
        int stock = fields[2].trimmed().toInt();  // 库存数量

        // 检查药品是否已存在
        bool exists = false;
        for (int row = 0; row < model->rowCount(); ++row) {
            QString existingName = model->data(model->index(row, model->fieldIndex("name"))).toString();
            QString existingDosage = model->data(model->index(row, model->fieldIndex("dosage"))).toString();

            if (existingName == name && existingDosage == dosage) {
                // 药品已存在，更新库存数量
                int existingStock = model->data(model->index(row, model->fieldIndex("stock"))).toInt();
                model->setData(model->index(row, model->fieldIndex("stock")), existingStock + stock);
                exists = true;
                updateCount++;
                break;
            }
        }

        if (!exists) {
            // 药品不存在，插入新记录
            int row = model->rowCount();
            model->insertRow(row);
            model->setData(model->index(row, model->fieldIndex("name")), name);
            model->setData(model->index(row, model->fieldIndex("dosage")), dosage);
            model->setData(model->index(row, model->fieldIndex("stock")), stock);
            successCount++;
        }
    }

    file.close();

    // 提交修改
    if (model->submitAll()) {
        QMessageBox::information(this, "成功", QString("药品信息已导入\n新增记录：%1\n更新记录：%2").arg(successCount).arg(updateCount));
    } else {
        QMessageBox::warning(this, "错误", "导入药品信息失败");
    }
}
