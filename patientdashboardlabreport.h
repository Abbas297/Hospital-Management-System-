#ifndef PATIENTDASHBOARDLABREPORT_H
#define PATIENTDASHBOARDLABREPORT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>

class QFrame;
class QPushButton;
class QLabel;

class PatientDashboardLabReport : public QWidget {
    Q_OBJECT

public:
    explicit PatientDashboardLabReport(const QString &patientId, QWidget *parent = nullptr);
    ~PatientDashboardLabReport();

private:
    void setupUI();
    void setupStyles();
    void fetchLabReportAvailability();

    QString patientId;
    QLabel *titleLabel;
    QLabel *statusLabel;
    QLabel *statusIcon;
    QFrame *statusFrame;
    QPushButton *backButton;
};

#endif // PATIENTDASHBOARDLABREPORT_H
