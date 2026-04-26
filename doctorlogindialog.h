#ifndef DOCTORLOGINDIALOG_H
#define DOCTORLOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase> // Add this
#include <QSqlQuery>
#include<DoctorDashboard.h>// Add this

class DoctorLoginDialog : public QDialog
{
    Q_OBJECT
signals:
    void loginSuccess(const QString &doctorId);
public:
    QString getDoctorId() const { return doctorIdInput->text(); }
public:
    DoctorLoginDialog(QWidget *parent = nullptr);
    ~DoctorLoginDialog();

private slots:
    void handleLoginClick();
    void handleBackClick();

private:
    void setupUI();
    void setupStyles();

    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *errorLabel;
    QLabel *doctorLabel;
    QLineEdit *doctorIdInput;
    QLabel *passwordLabel;
    QLineEdit *passwordInput;
    QPushButton *loginButton;
    QPushButton *backButton;
    bool validateCredentials(const QString &doctorId, const QString &password); // Added declaration to fix the error

};

#endif // DOCTORLOGINDIALOG_H
