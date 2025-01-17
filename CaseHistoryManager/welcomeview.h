#ifndef WELCOMEVIEW_H
#define WELCOMEVIEW_H

#include <QWidget>

namespace Ui {
class WelcomeView;
}

class WelcomeView : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeView(QWidget *parent = nullptr);
    ~WelcomeView();
    void hideAdminButtons();

private slots:
    void on_btnDepartment_clicked();

    void on_btnDoctor_clicked();

    void on_btnPatient_clicked();

    void on_btnMedicine_clicked();

    void on_btnAppointment_clicked();

signals:
    void goAppointmentView();
    void goDoctorView();
    void goPatientView();
    void goMedicineView();

private:
    Ui::WelcomeView *ui;
};

#endif // WELCOMEVIEW_H
