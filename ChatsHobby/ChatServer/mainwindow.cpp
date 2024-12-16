#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_chatServer = new ChatServer(this);

    connect(m_chatServer, &ChatServer::logMessage, this, &MainWindow::logMessage);
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
    if (m_chatServer->isListening()){
        m_chatServer->stopServer();
        ui->startStopButton->setText(tr("启动服务器"));
        logMessage(tr("服务器已停止"));
    }else{
        if(!m_chatServer->listen(QHostAddress::Any, 1967)){
            QMessageBox::critical(this, tr("错误"), tr("无法启动服务器"));
            return;
        }
        logMessage(tr("服务器已启动"));
        ui->startStopButton->setText(tr("停止服务器"));
    }
}

