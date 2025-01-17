#include "masterview.h"
#include "ui_masterview.h"
#include <QDebug>
#include "idatabase.h"

MasterView::MasterView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MasterView)
{
    ui->setupUi(this);
    goLoginView();
    ui->btnBack->setEnabled(false);
    ui->btnLogout->setEnabled(false);

    this->setWindowFlag(Qt::FramelessWindowHint);

    IDatabase::getInstance();
}

MasterView::~MasterView()
{
    delete ui;
}

void MasterView::goLoginView()
{
    loginView = new LoginView(this);
    pushWidgetToStackView(loginView);

    connect(loginView, SIGNAL(loginSuccess(QString)), this, SLOT(goWelcomeView(QString)));


}

void MasterView::goWelcomeView(const QString &role)
{
    welcomeView = new WelcomeView(this);
    pushWidgetToStackView(welcomeView);

    if (role == "医生") {
        welcomeView->hideAdminButtons();  // 隐藏管理员按钮
    }

    connect(welcomeView, SIGNAL(goAppointmentView()), this, SLOT(goAppointmentView()));
    connect(welcomeView, SIGNAL(goDoctorView()), this, SLOT(goDoctorView()));
    connect(welcomeView, SIGNAL(goPatientView()), this, SLOT(goPatientView()));
    connect(welcomeView, SIGNAL(goMedicineView()), this, SLOT(goMedicineView()));
}

void MasterView::goDoctorView()
{
    doctorView = new DoctorView(this);
    pushWidgetToStackView(doctorView);

    connect(doctorView, SIGNAL(goDoctorEditView(int)), this, SLOT(goDoctorEditView(int)));
}

void MasterView::goDepartmentView()
{
    departmentView = new DepartmentView(this);
    pushWidgetToStackView(departmentView);
}

void MasterView::goPatientEditView(int rowNo)
{
    patientEditView = new PatientEditView(this, rowNo);
    pushWidgetToStackView(patientEditView);
    connect(patientEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void MasterView::goDoctorEditView(int rowNo)
{
    doctorEditView = new DoctorEditView(this, rowNo);
    pushWidgetToStackView(doctorEditView);
    connect(doctorEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void MasterView::goPatientView()
{
    patientView = new PatientView(this);
    pushWidgetToStackView(patientView);

    connect(patientView, SIGNAL(goPatientEditView(int)), this, SLOT(goPatientEditView(int)));
}

void MasterView::goPreviousView()
{
    int count = ui->stackedWidget->count();

    if (count > 1)
    {
        ui->stackedWidget->setCurrentIndex(count - 2);
        ui->labelTitle->setText(ui->stackedWidget->currentWidget()->windowTitle());

        QWidget *widget = ui->stackedWidget->widget(count - 1);
        ui->stackedWidget->removeWidget(widget);
        delete widget;
    }
}

void MasterView::goMedicineView()
{
    medicineView = new MedicineView(this);
    pushWidgetToStackView(medicineView);

    connect(medicineView, SIGNAL(goMedicineEditView(int)), this, SLOT(goMedicineEditView(int)));
}

void MasterView::goMedicineEditView(int rowNo)
{
    medicineEditView = new MedicineEditView(this, rowNo);
    pushWidgetToStackView(medicineEditView);
    connect(medicineEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void MasterView::goAppointmentView()
{
    appointmentView = new AppointmentView(this);
    pushWidgetToStackView(appointmentView);

    connect(appointmentView, SIGNAL(goAppointmentEditView(int)), this, SLOT(goAppointmentEditView(int)));
}

void MasterView::goAppointmentEditView(int rowNo)
{
    appointmentEditView = new AppointmentEditView(this, rowNo);
    pushWidgetToStackView(appointmentEditView);
    connect(appointmentEditView,SIGNAL(goPreviousView()),this,SLOT(goPreviousView()));
}

void MasterView::pushWidgetToStackView(QWidget *widget)
{
    ui->stackedWidget->addWidget(widget);

    int count = ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(count - 1);
    ui->labelTitle->setText(widget->windowTitle());
}

void MasterView::on_btnBack_clicked()
{
    goPreviousView();
}


void MasterView::on_stackedWidget_currentChanged(int arg1)
{
    int count = ui->stackedWidget->count();
    if(count > 1){
        ui->btnBack->setEnabled(true);
    }
    else
        ui->btnBack->setEnabled(false);

    QString title = ui->stackedWidget->currentWidget()->windowTitle();

    if (title == "欢迎")
    {
        ui->btnBack->setEnabled(false);
        ui->btnLogout->setEnabled(true);
    }
    else
    {
        ui->btnLogout->setEnabled(false);
    }
    if (title == "登录")
    {
        ui->btnBack->setEnabled(false);
        ui->btnLogout->setEnabled(false);
    }
}


void MasterView::on_btnLogout_clicked()
{
    goPreviousView();
}

