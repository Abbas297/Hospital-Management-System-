#include "admindashboard.h"
#include <QDebug>
#include <QFrame>
#include <QScreen>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlError>
#include "admindashboardpatient.h"
#include "doctordashboardsalary.h"

// Constructor
AdminDashboard::AdminDashboard(const QString &adminId, QWidget *parent)
    : QWidget(parent), adminId(adminId), adminName("Unknown")
{
    // Load admin data from database
    loadAdminData();

    // Debug: Verify admin data
    qDebug() << "Admin Data Loaded - Name:" << adminName;

    // Setup UI and styles
    setupUI();
    setupStyles();

    // Set window properties
    setWindowTitle("NED HOSPITAL SYSTEM - ADMIN PANEL");
    setMinimumSize(800, 600);
    this->showMaximized();
    raise(); // Bring to the foreground
    activateWindow();
}

// Load admin data from database
void AdminDashboard::loadAdminData()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection"); // Added: Explicit database connection
    if (!db.isOpen()) {
        qDebug() << "Database connection failed.";
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT name FROM admins WHERE id = :id");
    query.bindValue(":id", adminId);

    if (!query.exec()) {
        qDebug() << "Failed to load admin data:" << query.lastError().text();
        return;
    }

    if (query.next()) {
        adminName = query.value("name").toString();
    } else {
        qDebug() << "No admin found with ID:" << adminId;
    }
}

void AdminDashboard::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 20, 30, 20);

    // Title
    admindashboardtitleLabel = new QLabel("NED HOSPITAL SYSTEM - ADMIN PANEL", this);
    admindashboardtitleLabel->setObjectName("admindashboardtitleLabel");
    admindashboardtitleLabel->setAlignment(Qt::AlignCenter);
    admindashboardtitleLabel->setToolTip("Admin Dashboard");

    // Info section (Frame 1)
    admindashboardframe = new QFrame(this);
    admindashboardframe->setObjectName("admindashboardframe");
    admindashboardframe->setFrameShape(QFrame::StyledPanel);
    admindashboardframe->setMaximumWidth(400); // Limit width to prevent stretching
    QVBoxLayout *infoLayout = new QVBoxLayout(admindashboardframe);
    infoLayout->setContentsMargins(15, 10, 15, 10);
    infoLayout->setSpacing(8);

    // Info labels with data
    admindashboardnameLabel = new QLabel(QString("Name: %1").arg(adminName), admindashboardframe);
    admindashboardnameLabel->setObjectName("admindashboardnameLabel");

    // Removed: admindashboardageLabel since age is not in the admins table

    // Debug: Verify label text
    qDebug() << "Name Label Text:" << admindashboardnameLabel->text();

    // Add tooltip for accessibility
    admindashboardnameLabel->setToolTip("Admin's full name");

    // Add labels to info layout
    infoLayout->addWidget(admindashboardnameLabel);
    infoLayout->setAlignment(Qt::AlignLeft);

    // Force repaint
    admindashboardnameLabel->update();
    admindashboardframe->update();

    // Dashboard label
    admindashboardLabel = new QLabel("DASHBOARD", this);
    admindashboardLabel->setObjectName("admindashboardLabel");
    admindashboardLabel->setAlignment(Qt::AlignCenter);
    admindashboardLabel->setToolTip("Available actions");

    // Buttons section (Frame 2)
    admindashboardframe2 = new QFrame(this);
    admindashboardframe2->setObjectName("admindashboardframe2");
    admindashboardframe2->setFrameShape(QFrame::StyledPanel);
    QHBoxLayout *buttonsLayout = new QHBoxLayout(admindashboardframe2);
    buttonsLayout->setSpacing(10);
    buttonsLayout->setContentsMargins(15, 10, 15, 10);

    admindashboarddoctorbtn = new QPushButton("Doctor", admindashboardframe2);
    admindashboarddoctorbtn->setObjectName("admindashboarddoctorbtn");
    admindashboardpatientbtn = new QPushButton("Patient", admindashboardframe2);
    admindashboardpatientbtn->setObjectName("admindashboardpatientbtn");
    admindashboardemployeebtn = new QPushButton("Salary", admindashboardframe2);
    admindashboardemployeebtn->setObjectName("admindashboardemployeebtn");

    // Add tooltips for accessibility
    admindashboarddoctorbtn->setToolTip("Manage doctors");
    admindashboardpatientbtn->setToolTip("Manage patients");
    admindashboardemployeebtn->setToolTip("Salary");

    // Add buttons to layout
    buttonsLayout->addWidget(admindashboarddoctorbtn);
    buttonsLayout->addWidget(admindashboardpatientbtn);
    buttonsLayout->addWidget(admindashboardemployeebtn);

    // Center buttons
    QHBoxLayout *centerButtonsLayout = new QHBoxLayout();
    centerButtonsLayout->addStretch();
    centerButtonsLayout->addWidget(admindashboardframe2);
    centerButtonsLayout->addStretch();

    // Logout button
    admindashboardlogoutbtn = new QPushButton("Logout", this);
    admindashboardlogoutbtn->setObjectName("admindashboardlogoutbtn");
    admindashboardlogoutbtn->setToolTip("Log out of the system");
    QHBoxLayout *logoutLayout = new QHBoxLayout();
    logoutLayout->addStretch();
    logoutLayout->addWidget(admindashboardlogoutbtn);

    // Add to main layout
    mainLayout->addWidget(admindashboardtitleLabel);
    mainLayout->addWidget(admindashboardframe, 0, Qt::AlignHCenter);
    mainLayout->addWidget(admindashboardLabel);
    mainLayout->addLayout(centerButtonsLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(logoutLayout);

    // Connections
    connect(admindashboarddoctorbtn, &QPushButton::clicked, this, &AdminDashboard::handleDoctorClick);
    connect(admindashboardpatientbtn, &QPushButton::clicked, this, &AdminDashboard::handlePatients);
    connect(admindashboardemployeebtn, &QPushButton::clicked, this, &AdminDashboard::handleEmployeeClick);
    connect(admindashboardlogoutbtn, &QPushButton::clicked, this, &AdminDashboard::handleLogoutClick);
}

void AdminDashboard::setupStyles()
{
    // Window styling
    setStyleSheet("background-color: #f0f2f5;");

    // Title style
    QFont titleFont("Sans Serif", 26, QFont::Bold);
    admindashboardtitleLabel->setFont(titleFont);
    admindashboardtitleLabel->setStyleSheet(
        "color: #1a3c5e;"
        "margin-bottom: 20px;"
        "padding: 8px;"
        );

    // Info frame styling
    admindashboardframe->setStyleSheet(
        "QFrame {"
        "   background-color: #ffffff;"
        "   border: 2px solid #1e90ff;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        );

    // Info labels style
    QFont infoFont("Sans Serif", 14, QFont::DemiBold);
    admindashboardnameLabel->setFont(infoFont);

    QString infoLabelStyle =
        "QLabel {"
        "   color: #000000;"
        "   padding: 8px 12px;"
        "}";
    admindashboardnameLabel->setStyleSheet(infoLabelStyle);

    // Dashboard label style
    QFont dashboardFont("Sans Serif", 20, QFont::Bold);
    admindashboardLabel->setFont(dashboardFont);
    admindashboardLabel->setStyleSheet(
        "color: #1a3c5e;"
        "margin: 25px 0;"
        "padding: 8px;"
        );

    // Buttons frame styling
    admindashboardframe2->setStyleSheet(
        "QFrame {"
        "   background-color: #ffffff;"
        "   border: 2px solid #1e90ff;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "}"
        );

    // Button styling
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

    admindashboarddoctorbtn->setStyleSheet(buttonStyle);
    admindashboardpatientbtn->setStyleSheet(buttonStyle);
    admindashboardemployeebtn->setStyleSheet(buttonStyle);

    // Logout button styling
    admindashboardlogoutbtn->setStyleSheet(
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

void AdminDashboard::handleDoctorClick()
{
    qDebug() << "Doctor button clicked";
    // Always create a new window to ensure visibility
    delete doctorWindow; // Clean up any existing window
    doctorWindow = new AdminDashboardDoctor(nullptr, this); // No parent to make it a top-level window
    doctorWindow->show();
    qDebug() << "Doctor window shown";
}
void AdminDashboard::handlePatients()
{
    qDebug() << "Patient button clicked";
    delete patientWindow; // Clean up any existing window
    patientWindow = new AdminDashboardPatient(nullptr, this);
    patientWindow->show();
    qDebug() << "Patient window shown";
}

void AdminDashboard::handleEmployeeClick()
{
    qDebug() << "Employee button clicked";
    // Placeholder doctor ID - replace with dynamic selection (e.g., from a QComboBox)
    QString doctorId = "doc1"; // Hardcoded for now, update with actual selection logic
    DoctorDashboardSalary *salaryDialog = new DoctorDashboardSalary(doctorId, this);
    salaryDialog->setAttribute(Qt::WA_DeleteOnClose);
    salaryDialog->show();
    salaryDialog->raise(); // Bring to the foreground
    salaryDialog->activateWindow(); // Ensure the window is focused
    qDebug() << "Salary dialog shown for doctor:" << doctorId;
}

void AdminDashboard::handleLogoutClick()
{
    qDebug() << "Logout button clicked";
    close();
}

AdminDashboard::~AdminDashboard() {}
