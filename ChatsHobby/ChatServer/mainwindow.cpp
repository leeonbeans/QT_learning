#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    started = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::logMessage(const QString &msg)
{
    ui->logEditor->appendPlainText(msg);
}

void MainWindow::on_startStopButton_clicked()
{
    if(!started)
    {
        ui->startStopButton->setText(tr("停止服务器"));
        logMessage(tr("服务器已启动"));
    }
    else
    {
        ui->startStopButton->setText(tr("启动服务器"));
        logMessage(tr("服务器已停止"));
    }
    started = !started;
}

