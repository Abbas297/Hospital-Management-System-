#include "patientdashboard.h"
#include "patientdashboardlabreport.h" // Ensure this matches the exact filename
#include <QDebug>
#include <QFrame>
#include <QScreen>
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QHeaderView>

PatientDashboard::PatientDashboard(const PatientData &data, QWidget *parent)
    : QWidget(parent), patientData(data)
{
    setupUI();
    setupStyles();

    setWindowTitle("NED HOSPITAL SYSTEM - ADMIN PANEL");
    setMinimumSize(800, 600); // Ensure usability on smaller screens
    showMaximized(); // Maximize the window
    raise(); // Bring to the foreground
    activateWindow();
}

void PatientDashboard::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 20, 30, 20);

    patientdashboardtitleLabel = new QLabel("NED HOSPITAL SYSTEM - PATIENT PANEL", this);
    patientdashboardtitleLabel->setAlignment(Qt::AlignCenter);
    patientdashboardtitleLabel->setToolTip("Patient Dashboard");

    infoFrame = new QFrame(this);
    infoFrame->setFrameShape(QFrame::StyledPanel);
    infoFrame->setMaximumWidth(400);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setContentsMargins(15, 10, 15, 10);
    infoLayout->setSpacing(8);

    // Fetch patient data from the database
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection failed.";
        patientdashboardnameLabel = new QLabel("NAME: Error", infoFrame);
        patientdashboardageLabel = new QLabel("AGE: Error", infoFrame);
        patientdashboarddiseaseLabel = new QLabel("DISEASE: Error", infoFrame);
        patientdashboardtdateLabel = new QLabel("ADMIT DATE: Error", infoFrame);
    } else {
        QSqlQuery query(db);
        query.prepare("SELECT name, age, disease, admitDate FROM patients WHERE id = :id");
        query.bindValue(":id", patientData.id);

        if (query.exec() && query.next()) {
            patientData.name = query.value("name").toString();
            patientData.age = query.value("age").toInt();
            patientData.disease = query.value("disease").toString();
            patientData.admitDate = query.value("admitDate").toString();
        } else {
            qDebug() << "Query failed or no patient found:" << query.lastError().text();
            patientData.name = "Not Found";
            patientData.age = 0;
            patientData.disease = "Not Found";
            patientData.admitDate = "Not Found";
        }

        patientdashboardnameLabel = new QLabel("NAME: " + patientData.name, infoFrame);
        patientdashboardageLabel = new QLabel("AGE: " + QString::number(patientData.age), infoFrame);
        patientdashboarddiseaseLabel = new QLabel("DISEASE: " + patientData.disease, infoFrame);
        patientdashboardtdateLabel = new QLabel("ADMIT DATE: " + patientData.admitDate, infoFrame);
    }

    patientdashboardnameLabel->setToolTip("Patient's full name");
    patientdashboardageLabel->setToolTip("Patient's age");
    patientdashboarddiseaseLabel->setToolTip("Primary diagnosis");
    patientdashboardtdateLabel->setToolTip("Date of admission");

    infoLayout->addWidget(patientdashboardnameLabel);
    infoLayout->addWidget(patientdashboardageLabel);
    infoLayout->addWidget(patientdashboarddiseaseLabel);
    infoLayout->addWidget(patientdashboardtdateLabel);
    infoLayout->setAlignment(Qt::AlignLeft);

    patientdashboardnameLabel->update();
    patientdashboardageLabel->update();
    patientdashboarddiseaseLabel->update();
    patientdashboardtdateLabel->update();
    infoFrame->update();

    patientdashboarddashboardLabel = new QLabel("DASHBOARD", this);
    patientdashboarddashboardLabel->setAlignment(Qt::AlignCenter);
    patientdashboarddashboardLabel->setToolTip("Available actions");

    QWidget *buttonsContainer = new QWidget(this);
    QGridLayout *buttonsLayout = new QGridLayout(buttonsContainer);
    buttonsLayout->setHorizontalSpacing(10);
    buttonsLayout->setVerticalSpacing(10);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);

    patientdashboardappointbtn = new QPushButton("APPOINTMENTS", buttonsContainer);
    patientdashboardlabbtn = new QPushButton("LAB REPORTS", buttonsContainer);
    patientdashboardpaymentbtn = new QPushButton("PAYMENTS", buttonsContainer);
    patientdashboarddocfeedbackbtn = new QPushButton("DOCTOR FEEDBACK", buttonsContainer);
    patientdashboardfeedbackbtn = new QPushButton("FEEDBACK", buttonsContainer);
    patientdashboardprofilebtn = new QPushButton("PROFILE", buttonsContainer);

    patientdashboardappointbtn->setToolTip("View or schedule appointments");
    patientdashboardlabbtn->setToolTip("Access lab test results");
    patientdashboardpaymentbtn->setToolTip("Manage billing and payments");
    patientdashboarddocfeedbackbtn->setToolTip("Provide feedback on doctors");
    patientdashboardfeedbackbtn->setToolTip("Submit general feedback");
    patientdashboardprofilebtn->setToolTip("View or edit patient profile");

    buttonsLayout->addWidget(patientdashboardappointbtn, 0, 0);
    buttonsLayout->addWidget(patientdashboardlabbtn, 0, 1);
    buttonsLayout->addWidget(patientdashboardpaymentbtn, 0, 2);
    buttonsLayout->addWidget(patientdashboardprofilebtn, 1, 0);
    buttonsLayout->addWidget(patientdashboarddocfeedbackbtn, 1, 1);
    buttonsLayout->addWidget(patientdashboardfeedbackbtn, 1, 2);

    QHBoxLayout *centerButtonsLayout = new QHBoxLayout();
    centerButtonsLayout->addStretch();
    centerButtonsLayout->addWidget(buttonsContainer);
    centerButtonsLayout->addStretch();

    patientdashboardlogoutbtn = new QPushButton("Logout", this);
    patientdashboardlogoutbtn->setToolTip("Log out of the system");
    QHBoxLayout *logoutLayout = new QHBoxLayout();
    logoutLayout->addStretch();
    logoutLayout->addWidget(patientdashboardlogoutbtn);

    mainLayout->addWidget(patientdashboardtitleLabel);
    mainLayout->addWidget(infoFrame, 0, Qt::AlignHCenter);
    mainLayout->addWidget(patientdashboarddashboardLabel);
    mainLayout->addLayout(centerButtonsLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(logoutLayout);

    connect(patientdashboardappointbtn, &QPushButton::clicked, this, &PatientDashboard::handleAppointmentsClick);
    connect(patientdashboardlabbtn, &QPushButton::clicked, this, &PatientDashboard::handleLabReportsClick);
    connect(patientdashboardpaymentbtn, &QPushButton::clicked, this, &PatientDashboard::handlePaymentsClick);
    connect(patientdashboarddocfeedbackbtn, &QPushButton::clicked, this, &PatientDashboard::handleDoctorFeedbackClick);
    connect(patientdashboardfeedbackbtn, &QPushButton::clicked, this, &PatientDashboard::handleFeedbackClick);
    connect(patientdashboardprofilebtn, &QPushButton::clicked, this, &PatientDashboard::handleProfileClick);
    connect(patientdashboardlogoutbtn, &QPushButton::clicked, this, &PatientDashboard::handleLogoutClick);
}

void PatientDashboard::setupStyles()
{
    setStyleSheet("background-color: #f0f2f5;");

    QFont titleFont("Sans Serif", 26, QFont::Bold);
    patientdashboardtitleLabel->setFont(titleFont);
    patientdashboardtitleLabel->setStyleSheet(
        "color: #1a3c5e;"
        "margin-bottom: 20px;"
        "padding: 8px;"
        );

    infoFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #ffffff;"
        "   border: 2px solid #1e90ff;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        );

    QFont infoFont("Sans Serif", 14, QFont::DemiBold);
    patientdashboardnameLabel->setFont(infoFont);
    patientdashboardageLabel->setFont(infoFont);
    patientdashboarddiseaseLabel->setFont(infoFont);
    patientdashboardtdateLabel->setFont(infoFont);

    QString infoLabelStyle =
        "QLabel {"
        "   color: #000000;"
        "   padding: 8px 12px;"
        "}";
    patientdashboardnameLabel->setStyleSheet(infoLabelStyle);
    patientdashboardageLabel->setStyleSheet(infoLabelStyle);
    patientdashboarddiseaseLabel->setStyleSheet(infoLabelStyle);
    patientdashboardtdateLabel->setStyleSheet(infoLabelStyle);

    QFont dashboardFont("Sans Serif", 20, QFont::Bold);
    patientdashboarddashboardLabel->setFont(dashboardFont);
    patientdashboarddashboardLabel->setStyleSheet(
        "color: #1a3c5e;"
        "margin: 25px 0;"
        "padding: 8px;"
        );

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #1e90ff;"
        "   color: #ffffff;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 160px;"
        "   min-height: 45px;"
        "   margin: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1565c0;"
        "}"
        "QPushButton:focus {"
        "   outline: none;"
        "   border: 2px solid #63b3ed;"
        "}";

    patientdashboardappointbtn->setStyleSheet(buttonStyle);
    patientdashboardlabbtn->setStyleSheet(buttonStyle);
    patientdashboardpaymentbtn->setStyleSheet(buttonStyle);
    patientdashboarddocfeedbackbtn->setStyleSheet(buttonStyle);
    patientdashboardfeedbackbtn->setStyleSheet(buttonStyle);
    patientdashboardprofilebtn->setStyleSheet(buttonStyle);

    patientdashboardlogoutbtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #e53e3e;"
        "   color: #ffffff;"
        "   border: 2px solid #c53030;"
        "   border-radius: 8px;"
        "   padding: 8px 15px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 100px;"
        "   margin-top: 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #c53030;"
        "}"
        "QPushButton:focus {"
        "   outline: none;"
        "   border: 2px solid #f56565;"
        "}"
        );
}

void PatientDashboard::handleAppointmentsClick()
{
    AppointmentsWindow *appointmentsWindow = new AppointmentsWindow(patientData.id, this);
    appointmentsWindow->setAttribute(Qt::WA_DeleteOnClose);
    appointmentsWindow->show();
}

void PatientDashboard::handleLabReportsClick()
{
    PatientDashboardLabReport *labReportsWindow = new PatientDashboardLabReport(patientData.id, this);
    labReportsWindow->setAttribute(Qt::WA_DeleteOnClose);
    labReportsWindow->show();
}

void PatientDashboard::handlePaymentsClick()
{
    PatientDashboardPayment *paymentWindow = new PatientDashboardPayment(patientData.id, this);
    paymentWindow->setAttribute(Qt::WA_DeleteOnClose);
    paymentWindow->show();
}

void PatientDashboard::handleDoctorFeedbackClick()
{
    PatientDashboardDocFeedback *feedbackWindow = new PatientDashboardDocFeedback(patientData.id, this);
    feedbackWindow->setAttribute(Qt::WA_DeleteOnClose);
    feedbackWindow->show();
}
void PatientDashboard::handleFeedbackClick()
{
    PatientDashboardFeedback *feedbackWindow = new PatientDashboardFeedback(patientData.id, this);
    feedbackWindow->setAttribute(Qt::WA_DeleteOnClose);
    feedbackWindow->show();
}
void PatientDashboard::handleProfileClick()
{
    PatientDashboardProfile *profileWindow = new PatientDashboardProfile(patientData.id, this);
    profileWindow->setAttribute(Qt::WA_DeleteOnClose);
    profileWindow->show();
}

void PatientDashboard::handleLogoutClick()
{
    close();
}

PatientDashboard::~PatientDashboard() {}
