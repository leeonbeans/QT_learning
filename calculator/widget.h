#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStack>
#include <QMap>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    QString operand = "";
    QString opcode = "";
    QStack<QString> operandStack;
    QString opcodePrevious = "";
    QMap<int, QPushButton*> btnNums;
    QMap<int, QPushButton*> btnBinary;
    QMap<QString, QPushButton*> btnUnary;
    QString calculation(bool &ifLegaL);

    struct{
        QString operandLast = "";
        QString opcodeLast = "";
        int size = 0;

        void add(QString operand,QString opcode)
        {
            operandLast = operand;
            opcodeLast = opcode;
            size+=1;
        }

        void clear()
        {
            operandLast = "";
            opcodeLast = "";
            size = 0;
        }

    }continuousEqual;

private slots:
    void btnNumClicked();

    void btnBinaryOperatorClicked();

    void btnUnaryOperatorClicked();

    void on_btnDecimal_clicked();

    void on_btnBackSpace_clicked();

    void on_btnCleanAll_clicked();

    void on_btnEqual_clicked();

    void on_btnCleanBorad_clicked();

    void on_display_editingFinished();

    virtual void keyPressEvent(QKeyEvent *event);

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
