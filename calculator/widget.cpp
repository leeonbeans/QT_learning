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

    connect(ui->btnPlus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMinus,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnMultiply,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    connect(ui->btnDivide,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));

    connect(ui->btnPercent,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnFraction,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquare,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnSquareRoot,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    connect(ui->btnInverse,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::calculation(bool &ifLegal)
{
    //取栈里面的数
    double num_2 = operandStack.top().toDouble();
    operandStack.pop();
    double num_1 = operandStack.top().toDouble();
    operandStack.pop();

    double result = 0;

    //保存并清除先前记录的操作符
    QString opertor = opcodePrevious;
    opcodePrevious = "";

    if (opertor == "+")
    {
        result = num_1+num_2;
        ifLegal = true;
        return QString::number(result);
    }
    else if (opertor == "-")
    {
        result = num_1-num_2;
        ifLegal = true;
        return QString::number(result);
    }
    else if (opertor == "×")
    {
        result = num_1*num_2;
        ifLegal = true;
        return QString::number(result);
    }
    else if (opertor == "÷")
    {
        if (num_2 != 0)
        {
            result = num_1/num_2;
            ifLegal = true;
            return QString::number(result);
        }
        else
        {
            ifLegal = false;
            return "除数不能为0!";
        }
    }
    else  //意外情况
    {
        ifLegal = false;
        return "ERROR";
    }
    return "ERROR";
}

void Widget::btnNumClicked()  //数字添加逻辑
{
    QString btnText=qobject_cast<QPushButton*>(sender())->text();
    if (btnText != "0")
    {
        if (operand == "0")
            operand = btnText;
        else
            operand += btnText;
    }
    else
    {
        if (operand != "0")
            operand += btnText;
    }

    ui->display->setText(operand);
}

void Widget::btnBinaryOperatorClicked()  //二元操作符逻辑
{
    bool ifLegal = false;  //操作有效布尔值
    opcode = qobject_cast<QPushButton*>(sender())->text();

    //存入数字
    if (operand == "")  //若空则只改变当前操作符
    {
        opcodePrevious = opcode;
        return;
    }
    else
        operandStack.push_back(operand);

    if (!opcodePrevious.isNull() && opcodePrevious != "")  //当已有表达式又点按了下一个操作符时，例如"4 + 3 ‘+’ "
    {
        QString result = calculation(ifLegal);
        if (ifLegal)
        {
            //合法计算则将结果作为二元计算中的第一元
            operandStack.push_back(result);
            ui->display->setText(result);
        }
        else
        {
            //非法则清除所有记录
            operand = "";
            opcodePrevious = "";
            operandStack.clear();
            ui->display->setText(result);
        }
    }

    //清除内容，输入第二元素
    operand = "";
    //存入操作符
    opcodePrevious = opcode;
}

void Widget::btnUnaryOperatorClicked()  //一元操作符逻辑
{
    double result=0;
    opcode = qobject_cast<QPushButton*>(sender())->text();

    if (operand.isNull()|| operand == "")
        return;

    if (opcode == "¹/x")  //倒数
    {
        if (operand == "0")
        {
            operand = "";
            opcodePrevious = "";
            operandStack.clear();
            ui->display->setText("除数不能为0!");
            return;
        }
        else
        {
            result = 1.0/operand.toDouble();
            operand = QString::number(result);
        }
    }
    else if (opcode == " x²")  //平方
    {
        result = operand.toDouble()*operand.toDouble();
        operand = QString::number(result);
    }
    else if (opcode == "²√ ")  //开方
    {
        result = sqrt(operand.toDouble());
        operand = QString::number(result);
    }
    else if (opcode == "%")  //百分号
    {
        result = operand.toDouble()/100;
        operand = QString::number(result);
    }
    else if (opcode == "⁺/-")  //相反数
    {
        result = 0-operand.toDouble();
        operand = QString::number(result);
    }
    else  //异常处理
    {
        operand = "";
        opcodePrevious = "";
        operandStack.clear();
        ui->display->setText("ERROR!");
    }

    ui->display->setText(operand);
}

void Widget::on_btnDecimal_clicked()  //小数点添加逻辑
{
    if (!operand.contains("."))
    {
        if (operand == "")
        {
            operand = "0.";
        }
        else
        {
            operand +=".";
        }
    }

    ui->display->setText(operand);
}


void Widget::on_btnBackSpace_clicked()  //退格键
{
    operand.removeLast();
    ui->display->setText(operand);
}


void Widget::on_btnCleanAll_clicked()  //C键清除所有
{
    operand = "";
    opcodePrevious = "";
    operandStack.clear();
    ui->display->setText(operand);
}


void Widget::on_btnCleanBorad_clicked()  //CE键清版
{
    operand.clear();
    ui->display->setText(operand);
}

void Widget::on_btnEqual_clicked()
{
    bool ifLegal = false;  //判断计算有效的布尔值

    if (opcodePrevious.isNull() || opcodePrevious == "")  //如果先前没有输入操作符，那就什么都不做
    {
        return;
    }
    else if (operand == "")  //如果什么都没输 默认为输入了0
    {
        operandStack.push_back("0");
    }
    else  //先前输入了操作符且也输入了第二元数值
    {
        operandStack.push_back(operand);
    }

    //计算逻辑
    QString result = calculation(ifLegal);
    if (ifLegal)
    {
        operand = result;
        ui->display->setText(result);
    }
    else
    {
        //非法则清除所有记录
        operand = "";
        opcodePrevious = "";
        operandStack.clear();
        ui->display->setText(result);
    }
}



