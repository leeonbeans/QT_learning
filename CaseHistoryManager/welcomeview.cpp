#include "welcomeview.h"
#include "ui_welcomeview.h"

WelcomeView::WelcomeView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomeView)
{
    ui->setupUi(this);
}

WelcomeView::~WelcomeView()
{
    delete ui;
}

void WelcomeView::on_btnDepartment_clicked()
{
    //
}


void WelcomeView::on_btnDoctor_clicked()
{
    emit goDoctorView();
}


void WelcomeView::on_btnPatient_clicked()
{
    emit goPatientView();
}

void WelcomeView::hideAdminButtons() {
    ui->btnDoctor->setVisible(false);
}

void WelcomeView::on_btnMedicine_clicked()
{
    emit goMedicineView();
}


void WelcomeView::on_btnAppointment_clicked()
{
    emit goAppointmentView();
}

