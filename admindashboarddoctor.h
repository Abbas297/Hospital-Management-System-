#ifndef ADMINDASHBOARDDOCTOR_H
#define ADMINDASHBOARDDOCTOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QSqlDatabase>

class AdminDashboard; // Forward declaration

class AdminDashboardDoctor : public QWidget
{
    Q_OBJECT

public:
    explicit AdminDashboardDoctor(QWidget *parent = nullptr, AdminDashboard *dashboardParent = nullptr);
    ~AdminDashboardDoctor();

private:
    void setupUI();
    void setupStyles();
    void loadDoctors();
    void loadLabReports();
    void loadTasks();
    void showStatusMessage(const QString &message, bool isError = false);

private slots:
    void handleAddDoctor();
    void handleUpdateDoctor();
    void handleDeleteDoctor();
    void handleAssignTask();
    void handleAssignLabReport();
    void handleBack();

private:
    QTableWidget *doctorTable;
    QTableWidget *labReportTable;
    QTableWidget *taskTable;
    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *specializationEdit;
    QLineEdit *ageEdit;
    QLineEdit *taskDescriptionEdit;
    QDateEdit *dueDateEdit;
    QComboBox *labReportCombo;
    QPushButton *addDoctorBtn;
    QPushButton *updateDoctorBtn;
    QPushButton *deleteDoctorBtn;
    QPushButton *assignTaskBtn;
    QPushButton *assignLabReportBtn;
    QPushButton *backBtn;
    QLabel *statusLabel;
    AdminDashboard *parentDashboard;
};

#endif // ADMINDASHBOARDDOCTOR_H
