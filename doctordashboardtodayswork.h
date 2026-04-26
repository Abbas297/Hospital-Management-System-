#ifndef DOCTORDASHBOARDTODAYSWORK_H
#define DOCTORDASHBOARDTODAYSWORK_H

#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>

class PatientHistoryDialog;

class DoctorDashboardTodaysWork : public QDialog
{
    Q_OBJECT

public:
    explicit DoctorDashboardTodaysWork(const QString &doctorId, QWidget *parent = nullptr);
    ~DoctorDashboardTodaysWork();

private slots:
    void onViewButtonClicked();
    void onMarkCompleteButtonClicked();

private:
    void setupUi();
    void populateAppointments();
    void populateTasks();

    QString m_doctorId;
    QVBoxLayout *mainLayout;
    QLabel *headerLabel;
    QTabWidget *tabWidget;
    QTableWidget *appointmentsTable;
    QTableWidget *tasksTable;
};

class PatientHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PatientHistoryDialog(const QString &patientId, QWidget *parent = nullptr);

private:
    void setupUi(const QString &patientId);
};

#endif // DOCTORDASHBOARDTODAYSWORK_H
