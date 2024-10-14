#include "widget.h"
#include "ui_widget.h"
#include "QKeyEvent"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowIcon(QIcon("icon.ico"));

    btnNums =   {{Qt::Key_0, ui->btnNum0},
                {Qt::Key_1, ui->btnNum1},
                {Qt::Key_2, ui->btnNum2},
                {Qt::Key_3, ui->btnNum3},
                {Qt::Key_4, ui->btnNum4},
                {Qt::Key_5, ui->btnNum5},
                {Qt::Key_6, ui->btnNum6},
                {Qt::Key_7, ui->btnNum7},
                {Qt::Key_8, ui->btnNum8},
                {Qt::Key_9, ui->btnNum9},
    };

    foreach (auto btn, btnNums) {
        connect(btn,SIGNAL(clicked()),this,SLOT(btnNumClicked()));
    }

    btnBinary = {{Qt::Key_Plus,ui->btnPlus},
                 {Qt::Key_Minus,ui->btnMinus},
                 {Qt::Key_Asterisk,ui->btnMultiply},
                 {Qt::Key_Slash,ui->btnDivide}
    };

    foreach (auto btn, btnBinary) {
        connect(btn,SIGNAL(clicked()),this,SLOT(btnBinaryOperatorClicked()));
    }

    btnUnary = {{"Percent",ui->btnPercent},
                 {"Fraction",ui->btnFraction},
                 {"Square",ui->btnSquare},
                 {"SquareRoot",ui->btnSquareRoot},
                 {"Inverse",ui->btnInverse}
    };

    foreach (auto btn, btnUnary){
        connect(btn,SIGNAL(clicked()),this,SLOT(btnUnaryOperatorClicked()));
    }
}

Widget::~Widget()
{
    delete ui;
}

QString Widget::calculation(bool &ifLegal)  //计算
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

    if (opertor == "+")  //加法
    {
        result = num_1+num_2;
        ifLegal = true;
        return QString::number(result);
    }
    else if (opertor == "-")  //减法
    {
        result = num_1-num_2;
        ifLegal = true;
        return QString::number(result);
    }
    else if (opertor == "×")  //乘法
    {
        result = num_1*num_2;
        ifLegal = true;
        return QString::number(result);
    }
    else if (opertor == "÷")  //除法
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
        if (operand == "0")  //若当前显示数字为0,则直接替换成所按的数字
            operand = btnText;
        else
            operand += btnText;
    }
    else  //限制添加无意义的高位"0"
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

    continuousEqual.clear();

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
            continuousEqual.clear();
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

    continuousEqual.clear();

    if (operand.isNull()|| operand == "")
    {
        if (opcodePrevious.isNull() || opcodePrevious == "")  //当按了二元操作符又立刻按了一元操作符的特殊情况
        {
            return;
        }
        else
        {
            operand = operandStack.top();
            operandStack.pop();
            opcodePrevious = "";
        }
    }

    if (opcode == "¹/x")  //倒数
    {
        if (operand == "0")
        {
            operand = "";
            opcodePrevious = "";
            operandStack.clear();
            continuousEqual.clear();
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
        continuousEqual.clear();
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
    continuousEqual.clear();
    ui->display->setText(operand);
}


void Widget::on_btnCleanBorad_clicked()  //CE键清版
{
    operand.clear();
    ui->display->setText(operand);
}

void Widget::on_btnEqual_clicked()  //等于按钮逻辑
{
    bool ifLegal = false;  //判断计算有效的布尔值

    if (continuousEqual.size==1 && continuousEqual.size>0) //连续计算逻辑
    {
        operandStack.push(operand);  //添加目前结果为第一元
        operandStack.push(continuousEqual.operandLast);  //添加上一个第二元
        opcodePrevious = continuousEqual.opcodeLast;  //添加上一个操作符
        continuousEqual.clear();  //清除本次记录
    }
    else if (opcodePrevious.isNull() || opcodePrevious == "")  //如果先前没有输入操作符，那就什么都不做
    {
        return;
    }
    else if (operand == "")  //如果输入了操作符 但第二元什么都没输 默认为输入了0
    {
        operandStack.push_back("0");
    }
    else  //先前输入了操作符且也输入了第二元数值
    {
        operandStack.push_back(operand);
    }

    continuousEqual.add(operandStack.top(),opcodePrevious);  //连续计算逻辑--添加当前操作符与第二元操作数

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
        continuousEqual.clear();
    }
}

void Widget::on_display_editingFinished()  //文本框直接输入数字获取逻辑
{
    operand = qobject_cast<QLineEdit*>(sender())->text();
}


void Widget::keyPressEvent(QKeyEvent *event)  //键盘输入逻辑
{
    foreach(auto btn, btnNums.keys())  //数字
    {
        if (event->key() == btn)
            btnNums[btn]->animateClick();
    }

    foreach(auto btn, btnBinary.keys())  //二元操作键
    {
        if (event->key() == btn)
            btnBinary[btn]->animateClick();
    }

    if (event->key() == Qt::Key_Backspace)  //退格键
        ui->btnBackSpace->animateClick();

    if (event->key() == Qt::Key_Equal || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)  //等于键和enter键
        ui->btnEqual->animateClick();

    if (event->key() == Qt::Key_Period)  //小数点
        ui->btnDecimal->animateClick();
}

