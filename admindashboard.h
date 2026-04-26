    #ifndef ADMINDASHBOARD_H
    #define ADMINDASHBOARD_H

    #include <QWidget>
    #include <QLabel>
    #include <QPushButton>
    #include <QVBoxLayout>
    #include <QHBoxLayout>
    #include <QFrame>
    #include <QSqlDatabase> // Added: For database access
    #include <QSqlQuery>    // Added: For querying the database
    #include <QSqlError>    // Added: For error handling
    #include "admindashboarddoctor.h"
    #include "admindashboardpatient.h"

    class AdminDashboard : public QWidget
    {
        Q_OBJECT

    public:
        AdminDashboard(const QString &adminId, QWidget *parent = nullptr);
        ~AdminDashboard();

    private slots:
        void handleDoctorClick();
        void handlePatients();
        void handleEmployeeClick();
        void handleLogoutClick();


    private:
        void setupUI();
        void setupStyles();
        void loadAdminData();

        QString adminId;
        QString adminName;
        // Removed: adminAge since it's not in the admins table
        QLabel *admindashboardtitleLabel;
        QFrame *admindashboardframe;
        QLabel *admindashboardnameLabel;
        // Removed: admindashboardageLabel
        QLabel *admindashboardLabel;
        QFrame *admindashboardframe2;
        QPushButton *admindashboarddoctorbtn;
        QPushButton *admindashboardpatientbtn;
        QPushButton *admindashboardemployeebtn;
        QPushButton *admindashboardlogoutbtn;
        AdminDashboardDoctor *doctorWindow;
        AdminDashboardPatient *patientWindow;
    };

    #endif // ADMINDASHBOARD_H
