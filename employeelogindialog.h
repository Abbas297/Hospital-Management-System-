#ifndef EMPLOYEELOGINDIALOG_H
#define EMPLOYEELOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class EmployeeLoginDialog : public QDialog
{
    Q_OBJECT

public:
    EmployeeLoginDialog(QWidget *parent = nullptr);
    ~EmployeeLoginDialog();

private slots:
    void handleEmployeeLoginClick();
    void handleEmployeeBackClick();

private:
    void setupUI();
    void setupStyles();
    bool validateCredentials(const QString &employeeId, const QString &password);
    // UI Elements
    QLabel *employeetitleLabel;      // "Welcome Back, EMPLOYEE"
    QLabel *employeesubtitleLabel;   // "Please Login,"
    QLabel *errorLabel;
    QLabel *employeeLabel;           // "Employee ID:"
    QLineEdit *employeeIdInput;      // Input field for Employee ID
    QLabel *employeepasswordLabel;   // "Password:"
    QLineEdit *employeepasswordInput;// Input field for Password
    QPushButton *employeeloginButton;// "Login" button
    QPushButton *employeebackButton; // "Back" button
};

#endif // EMPLOYEELOGINDIALOG_H
