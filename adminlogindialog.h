#ifndef ADMINLOGINDIALOG_H
#define ADMINLOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

class AdminLoginDialog : public QDialog
{
    Q_OBJECT

public:
    AdminLoginDialog(QWidget *parent = nullptr);
    ~AdminLoginDialog();

private slots:
    void handleAdminLoginClick();
    void handleAdminBackClick();

private:
    void setupUI();
    void setupStyles();
bool validateCredentials(const QString &adminId, const QString &password);
    // UI Elements
    QLabel *admintitleLabel;      // "Welcome Back, ADMIN"
    QLabel *adminsubtitleLabel;    // "Please Login,"
    QLabel *errorLabel;
    QLabel *adminLabel;           // "Admin ID:"
    QLineEdit *adminIdInput;      // Input field for Admin ID
    QLabel *adminpasswordLabel;   // "Password:"
    QLineEdit *adminpasswordInput;// Input field for Password
    QPushButton *adminloginButton;// "Login" button
    QPushButton *adminbackButton; // "Back" button
};

#endif // ADMINLOGINDIALOG_H
