#ifndef MEDICINEEDITVIEW_H
#define MEDICINEEDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class MedicineEditView;
}

class MedicineEditView : public QWidget
{
    Q_OBJECT

public:
    explicit MedicineEditView(QWidget *parent, int index);
    ~MedicineEditView();

private:
    Ui::MedicineEditView *ui;
    QDataWidgetMapper *dataMapper;

signals:
    void goPreviousView();
private slots:
    void on_btnSave_clicked();
    void on_btnCancel_clicked();
};

#endif // MEDICINEEDITVIEW_H
