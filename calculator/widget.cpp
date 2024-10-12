#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->btnNum0,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum1,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum2,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum3,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum4,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum5,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum6,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum7,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum8,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    connect(ui->btnNum9,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::btnNumClicked()  //数字添加逻辑
{
    QString displayText=ui->display->text();
    QString btnText=qobject_cast<QPushButton*>(sender())->text();
    if (btnText != "0")
    {
        if (displayText == "0")
            ui->display->setText(btnText);
        else
             ui->display->setText(displayText+btnText);
    }
    else
    {
        if (displayText != "0")
            ui->display->setText(displayText+btnText);
    }
}

void Widget::on_btnDecimal_clicked()  //小数点添加逻辑
{
    QString displayText=ui->display->text();
    if (!displayText.contains("."))
        if (displayText == "")
            ui->display->setText("0.");
        else
            ui->display->setText(displayText+".");
}


void Widget::on_btnBackSpace_clicked()
{
    QString displayText = ui->display->text();
    displayText.removeLast();
    ui->display->setText(displayText);
}

