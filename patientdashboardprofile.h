#ifndef PATIENTDASHBOARDPROFILE_H
#define PATIENTDASHBOARDPROFILE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>

class QFrame;
class QPushButton;
class QLabel;

class PatientDashboardProfile : public QWidget {
    Q_OBJECT

public:
    explicit PatientDashboardProfile(const QString &patientId, QWidget *parent = nullptr);
    ~PatientDashboardProfile();

private:
    void setupUI();
    void setupStyles();
    void fetchPatientProfile();

    QString patientId;
    QLabel *titleLabel;
    QLabel *nameLabel;
    QLabel *ageLabel;
    QLabel *contactLabel;
    QLabel *diseaseLabel;
    QLabel *admitDateLabel;
    QLabel *assignedDoctorsCountLabel;
    QLabel *assignedDoctorsNamesLabel;
    QFrame *profileFrame;
    QPushButton *backButton;
};

#endif // PATIENTDASHBOARDPROFILE_H
