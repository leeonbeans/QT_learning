#ifndef PATIENTVIEW_H
#define PATIENTVIEW_H

#include <QWidget>

namespace Ui {
class PatientView;
}

class PatientView : public QWidget
{
    Q_OBJECT

public:
    explicit PatientView(QWidget *parent = nullptr);
    ~PatientView();

private slots:
    void on_btnAdd_clicked();

    void on_btnSearch_clicked();

    void on_addDelete_clicked();

    void on_btnEdit_clicked();

    void on_btnExport_clicked();

    void on_btnImport_clicked();

    void on_btnPrevPage_clicked();

    void on_btnNextPage_clicked();

signals:
    void goPatientEditView(int idx);

private:
    Ui::PatientView *ui;
    int currentPage = 1;  // 当前页码
    int pageSize = 10;    // 每页显示的记录数
    int totalPages = 1;   // 总页数

    void updatePage();    // 更新分页数据
};

#endif // PATIENTVIEW_H
