#ifndef PATIENTLOGINDIALOG_H
#define PATIENTLOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
class PatientDashboard;
class PatientLoginDialog : public QDialog
{
    Q_OBJECT

public:
    PatientLoginDialog(QWidget *parent = nullptr);
    ~PatientLoginDialog();

private slots:
    void handlePatientLoginClick();
    void handlePatientBackClick();

private:
    void setupUI();
    void setupStyles();
    bool validateCredentials(const QString &patientId, const QString &password); // Added for
    // UI Elements
    QLabel *patienttitleLabel;      // "Welcome Back, PATIENT"
    QLabel *patientsubtitleLabel;   // "Please Login,"
    QLabel *errorLabel;
    QLabel *patientLabel;           // "Patient ID:"
    QLineEdit *patientIdInput;      // Input field for Patient ID
    QLabel *patientpasswordLabel;   // "Password:"
    QLineEdit *patientpasswordInput;// Input field for Password
    QPushButton *patientloginButton;// "Login" button
    QPushButton *patientbackButton; // "Back" button
};

#endif // PATIENTLOGINDIALOG_H
