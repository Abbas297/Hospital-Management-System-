#ifndef DOCTORDASHBOARDSALARY_H
#define DOCTORDASHBOARDSALARY_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DoctorDashboardSalary : public QDialog
{
    Q_OBJECT

public:
    DoctorDashboardSalary(const QString &doctorId, QWidget *parent = nullptr);
    ~DoctorDashboardSalary();

private:
    void setupUi();
    void fetchSalaryDetails();

    QString m_doctorId;
    QVBoxLayout *mainLayout;
    QLabel *headerLabel;
    QLabel *statusLabel;
    QTableWidget *salaryTable;
    QPushButton *closeButton;
};

#endif // DOCTORDASHBOARDSALARY_H
