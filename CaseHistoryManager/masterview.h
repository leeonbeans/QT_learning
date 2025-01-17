#ifndef MASTERVIEW_H
#define MASTERVIEW_H

#include <QWidget>
#include "loginview.h"
#include "doctorview.h"
#include "departmentview.h"
#include "patienteditview.h"
#include "patientview.h"
#include "welcomeview.h"
#include "doctoreditview.h"
#include "medicineview.h"
#include "medicineeditview.h"
#include "appointmentview.h"
#include "appointmenteditview.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MasterView;
}
QT_END_NAMESPACE

class MasterView : public QWidget
{
    Q_OBJECT

public:
    MasterView(QWidget *parent = nullptr);
    ~MasterView();

public slots:
    void goLoginView();
    void goWelcomeView(const QString &role);
    void goDoctorView();
    void goDepartmentView();
    void goPatientEditView(int rowNo);
    void goDoctorEditView(int rowNo);
    void goPatientView();
    void goPreviousView();
    void goMedicineView();
    void goMedicineEditView(int rowNo);
    void goAppointmentView();
    void goAppointmentEditView(int rowNo);

private slots:
    void on_btnBack_clicked();

    void on_stackedWidget_currentChanged(int arg1);

    void on_btnLogout_clicked();

private:
    void pushWidgetToStackView(QWidget *widget);

    Ui::MasterView *ui;

    WelcomeView *welcomeView;
    DoctorView *doctorView;
    PatientView *patientView;
    DepartmentView *departmentView;
    LoginView *loginView;
    PatientEditView *patientEditView;
    DoctorEditView *doctorEditView;
    MedicineView *medicineView;
    MedicineEditView *medicineEditView;
    AppointmentView *appointmentView;
    AppointmentEditView *appointmentEditView;
};
#endif // MASTERVIEW_H
