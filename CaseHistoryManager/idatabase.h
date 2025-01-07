#ifndef IDATABASE_H
#define IDATABASE_H

#include <QObject>
#include <Qtsql>
#include <QSqlDatabase>
#include <QDataWidgetMapper>

class IDatabase : public QObject
{
    Q_OBJECT
public:
    static IDatabase & getInstance()
    {
        static IDatabase instance; // Guaranteed to be destroyed.
            // Instantiated on first use.
        return instance;
    }

    QString userLogin(QString username,QString password);
    bool initPatientModel();
    QSqlTableModel *patientTabModel;
    QItemSelectionModel *thePatientSelection;
    void initDatabase();
    bool searchPatient(QString filter);
    bool deleteCurrentPatient();
    bool submitPatientEdit();
    void revertPatientEdit();
    int addNewPatient();
    QString doctorLogin(const QString &username, const QString &password);

    bool initDoctorModel();  // 初始化医生模型
    int addNewDoctor();     // 添加新医生
    bool deleteCurrentDoctor();  // 删除当前医生
    bool searchDoctor(const QString &filter);  // 查询医生
    bool submitDoctorEdit();  // 提交医生信息修改
    void revertDoctorEdit();  // 撤销医生信息修改

    QSqlTableModel *doctorTabModel;  // 医生信息模型
    QItemSelectionModel *theDoctorSelection;  // 医生选择模型

    bool initMedicineModel();  // 初始化药品模型
    int addNewMedicine();     // 添加新药品
    bool deleteCurrentMedicine();  // 删除当前药品
    bool searchMedicine(const QString &filter);  // 查询药品
    bool submitMedicineEdit();  // 提交药品信息修改
    void revertMedicineEdit();  // 撤销药品信息修改

    QSqlTableModel *medicineTabModel;  // 药品信息模型
    QItemSelectionModel *theMedicineSelection;  // 药品选择模型

    bool initAppointmentModel();  // 初始化就诊记录模型
    int addNewAppointment();     // 添加新就诊记录
    bool deleteCurrentAppointment();  // 删除当前就诊记录
    bool searchAppointment(const QString &filter);  // 查询就诊记录
    bool submitAppointmentEdit();  // 提交就诊记录修改
    void revertAppointmentEdit();  // 撤销就诊记录修改

    QSqlTableModel *appointmentTabModel;  // 就诊记录模型
    QItemSelectionModel *theAppointmentSelection;  // 就诊记录选择模型

private:

    explicit IDatabase(QObject *parent = nullptr);
    IDatabase(IDatabase const&) = delete;
    void operator=(IDatabase const&)  = delete;

    QSqlDatabase database;
signals:
};

#endif // IDATABASE_H
