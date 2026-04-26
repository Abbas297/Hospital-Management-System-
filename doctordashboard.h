#ifndef DOCTORDASHBOARD_H
#define DOCTORDASHBOARD_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "doctordashboardtodayswork.h"
#include "doctorreportsdialog.h"
#include "doctordashboardsalary.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DoctorDashboard : public QWidget
{
    Q_OBJECT

public:
    DoctorDashboard(const QString &currentDoctor, QWidget *parent = nullptr);  // Cor
    ~DoctorDashboard();

private slots:
    void handleTodaysWorkClick();
    void handleReportsClick();
    void handleSalaryClick();
    void handleLogoutClick();

private:
    void setupUI();
    void setupStyles();
    void loadDoctorDetails();

    QString currentDoctor;
    DoctorDashboardTodaysWork *todaysWorkDialog;
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QFrame *infoFrame;
    QLabel *nameLabel;
    QLabel *ageLabel;
    QLabel *domainLabel;
    QLabel *dashboardLabel;
    QPushButton *todaysWorkBtn;
    QPushButton *reportsBtn;
    QPushButton *salaryBtn;
    QPushButton *logoutBtn;
    QLabel *avatarLabel;
};

#endif // DOCTORDASHBOARD_H
