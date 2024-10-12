#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStack>

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
    QString opcode;
    QStack<QString> operandStack;
    QString opcodePrevious = "";

    QString calculation(bool &ifLegaL);

private slots:
    void btnNumClicked();

    void btnBinaryOperatorClicked();

    void btnUnaryOperatorClicked();

    void on_btnDecimal_clicked();

    void on_btnBackSpace_clicked();

    void on_btnCleanAll_clicked();

    void on_btnEqual_clicked();

    void on_btnCleanBorad_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
