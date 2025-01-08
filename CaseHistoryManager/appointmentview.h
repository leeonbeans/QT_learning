#ifndef APPOINTMENTVIEW_H
#define APPOINTMENTVIEW_H

#include <QWidget>

namespace Ui {
class AppointmentView;
}

class AppointmentView : public QWidget
{
    Q_OBJECT

public:
    explicit AppointmentView(QWidget *parent = nullptr);
    ~AppointmentView();

signals:
    void goAppointmentEditView(int idx);

private slots:
    void on_btnAdd_clicked();

    void on_btnEdit_clicked();

    void on_btnDelete_clicked();

    void on_btnSearch_clicked();

    void on_btnExport_clicked();

private:
    Ui::AppointmentView *ui;
};

#endif // APPOINTMENTVIEW_H
