#ifndef EMPLOYEEDASHBOARD_H
#define EMPLOYEEDASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSqlDatabase> // Added: For database access
#include <QSqlQuery>    // Added: For querying the database
#include <QSqlError>

class EmployeeDashboard : public QWidget
{
    Q_OBJECT

public:
  EmployeeDashboard(const QString currentEmployee, QWidget *parent = nullptr);
    ~EmployeeDashboard();

private slots:
    void handleWorkClick();
    void handleReportsClick();
    void handleSalaryClick();
    void handleLogoutClick();

private:
    void setupUI();
    void setupStyles();
private:
    void loadEmployeeDetails(); // Declaration


    // UI Elements
    QLabel *titleLabel;            // "NED HOSPITAL SYSTEM - EMPLOYEE PANEL"
    QLabel *nameLabel;             // "NAME:"
    QLabel *ageLabel;              // "AGE:"
    QLabel *departmentLabel;       // "DEPARTMENT:"
    QLabel *dashboardLabel;        // "DASHBOARD"
    QPushButton *workBtn;          // "TODAY'S WORK"
    QPushButton *reportsBtn;       // "REPORTS"
    QPushButton *salaryBtn;        // "SALARY"
    // "Change Password"
    QPushButton *logoutBtn;        // "Logout"

    // Data
    QString currentEmployee;
};

#endif // EMPLOYEEDASHBOARD_H
