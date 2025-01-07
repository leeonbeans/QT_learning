#include "medicineview.h"
#include "ui_medicineview.h"
#include <idatabase.h>
#include <QMessageBox>

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

