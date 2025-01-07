#ifndef DOCTOREDITVIEW_H
#define DOCTOREDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class DoctorEditView;
}

class DoctorEditView : public QWidget
{
    Q_OBJECT

public:
    explicit DoctorEditView(QWidget *parent, int index);
    ~DoctorEditView();

signals:
    void goPreviousView();

private slots:
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

private:
    Ui::DoctorEditView *ui;
    QDataWidgetMapper *dataMapper;


};

#endif // DOCTOREDITVIEW_H
