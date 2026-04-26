#ifndef PATIENTDASHBOARD_H
#define PATIENTDASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QFrame>
#include "appointmentswindow.h"
#include "patientdashboardpayment.h"
#include "patientdashboarddocfeedback.h"
#include "patientdashboardfeedback.h"
#include "patientdashboardprofile.h"

// Define PatientData struct in the header to resolve incomplete type error
struct PatientData {
    QString name;
    QString id;
    int age;
    QString disease;
    QString admitDate;
};
class AppointmentsWindow; // Forward declaration
class PatientDashboardLabReport;

class PatientDashboard : public QWidget
{
    Q_OBJECT

public:
    explicit PatientDashboard(const PatientData &data, QWidget *parent = nullptr);
    ~PatientDashboard();

private:
    // UI components
    QLabel *patientdashboardtitleLabel;
    QFrame *infoFrame;
    QLabel *patientdashboardnameLabel;
    QLabel *patientdashboardageLabel;
    QLabel *patientdashboarddiseaseLabel;
    QLabel *patientdashboardtdateLabel;
    QLabel *patientdashboarddashboardLabel;
    QPushButton *patientdashboardappointbtn;
    QPushButton *patientdashboardlabbtn;
    QPushButton *patientdashboardpaymentbtn;
    QPushButton *patientdashboarddocfeedbackbtn;
    QPushButton *patientdashboardfeedbackbtn;
    QPushButton *patientdashboardprofilebtn;
    QPushButton *patientdashboardlogoutbtn;

    // Patient data
    PatientData patientData;

    // Setup methods
    void setupUI();
    void setupStyles();
    // PatientData loadPatientData(); // Removed since data is now fetched in setupUI

private slots:
    void handleAppointmentsClick();
    void handleLabReportsClick();
    void handlePaymentsClick();
    void handleDoctorFeedbackClick();
    void handleFeedbackClick();
    void handleProfileClick();
    void handleLogoutClick();
};

#endif // PATIENTDASHBOARD_H
