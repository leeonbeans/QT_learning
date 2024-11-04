#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //添加信息至状态栏
    statusLable.setMaximumWidth(150);
    statusLable.setText("Length: " + QString::number(0) + " Lines: " + QString::number(1) + "  ");
    ui->statusbar->addPermanentWidget(&statusLable);

    statusCursorLable.setMaximumWidth(150);
    statusCursorLable.setText("Ln: " + QString::number(0) + " Col: " + QString::number(1) + "  ");
    ui->statusbar->addPermanentWidget(&statusCursorLable);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText("Leeon");
    ui->statusbar->addPermanentWidget(author);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog aboutDialog;
    aboutDialog.exec();
}


void MainWindow::on_actionFind_triggered()
{
    SearchDialog searchDialog;
        searchDialog.exec();
}

