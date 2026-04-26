#ifndef ADMINDASHBOARDPATIENT_H
#define ADMINDASHBOARDPATIENT_H

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

class AdminDashboardPatient : public QWidget
{
    Q_OBJECT

public:
    explicit AdminDashboardPatient(QWidget *parent = nullptr, AdminDashboard *dashboardParent = nullptr);
    ~AdminDashboardPatient();

private:
    void setupUI();
    void setupStyles();
    void loadPatients();
    void loadAppointments();
    void loadLabReports();
    void loadPayments();
    void showStatusMessage(const QString &message, bool isError = false);

private slots:
    void handleAddPatient();
    void handleUpdatePatient();
    void handleDeletePatient();
    void handleAddAppointment();
    void handleUpdateAppointment();
    void handleDeleteAppointment();
    void handleAddPayment();
    void handleUpdatePayment();
    void handleDeletePayment();
    void handleAssignLabReport();
    void handleBack();

private:
    QTableWidget *patientTable;
    QTableWidget *appointmentTable;
    QTableWidget *labReportTable;
    QTableWidget *paymentTable;
    QLineEdit *idEdit;
    QLineEdit *nameEdit;
    QLineEdit *passwordEdit;
    QLineEdit *ageEdit;
    QLineEdit *contactEdit;
    QLineEdit *appointmentStatusEdit; // Changed from appointmentDescriptionEdit
    QDateEdit *appointmentDateEdit;
    QLineEdit *paymentAmountEdit;
    QDateEdit *paymentDateEdit;
    QLineEdit *paymentStatusEdit; // Added for payment status
    QComboBox *labReportCombo;
    QPushButton *addPatientBtn;
    QPushButton *updatePatientBtn;
    QPushButton *deletePatientBtn;
    QPushButton *addAppointmentBtn;
    QPushButton *updateAppointmentBtn;
    QPushButton *deleteAppointmentBtn;
    QPushButton *addPaymentBtn;
    QPushButton *updatePaymentBtn;
    QPushButton *deletePaymentBtn;
    QPushButton *assignLabReportBtn;
    QPushButton *backBtn;
    QLabel *statusLabel;
    AdminDashboard *parentDashboard;
};

#endif // ADMINDASHBOARDPATIENT_H
