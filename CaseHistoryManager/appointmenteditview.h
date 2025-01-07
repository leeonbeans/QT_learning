#ifndef APPOINTMENTEDITVIEW_H
#define APPOINTMENTEDITVIEW_H

#include <QWidget>
#include <QDataWidgetMapper>

namespace Ui {
class AppointmentEditView;
}

class AppointmentEditView : public QWidget
{
    Q_OBJECT

public:
    explicit AppointmentEditView(QWidget *parent, int index);
    ~AppointmentEditView();

signals:
    void goPreviousView();

private slots:
    void on_btnSave_clicked();

    void on_btnCancel_clicked();

private:
    Ui::AppointmentEditView *ui;
    QDataWidgetMapper *dataMapper;
};

#endif // APPOINTMENTEDITVIEW_H
