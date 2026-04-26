#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowState(Qt::WindowMaximized);
    setupUI();
    setupStyles();
}

void MainWindow::setupUI()
{
    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Title
    titleLabel = new QLabel("NED HOSPITAL SYSTEM", centralWidget);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Subtitle
    subtitleLabel = new QLabel("Who are you?", centralWidget);
    subtitleLabel->setAlignment(Qt::AlignCenter);

    // Buttons
    doctorBtn = new QPushButton("Doctor", centralWidget);
    patientBtn = new QPushButton("Patient", centralWidget);
    adminBtn = new QPushButton("Admin", centralWidget);

    // Button sizing
    for (QPushButton *button : {doctorBtn, patientBtn, adminBtn}) {
        button->setMinimumSize(200, 60);
    }

    // Buttons layout
    QWidget *buttonsContainer = new QWidget(centralWidget);
    QVBoxLayout *buttonsLayout = new QVBoxLayout(buttonsContainer);
    buttonsLayout->setSpacing(10);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);

    // Doctor and Patient buttons in a horizontal layout
    QHBoxLayout *topButtonsLayout = new QHBoxLayout();
    topButtonsLayout->addWidget(doctorBtn);
    topButtonsLayout->addWidget(patientBtn);
    topButtonsLayout->setSpacing(10);
    topButtonsLayout->setAlignment(Qt::AlignCenter);

    // Admin button centered below
    QHBoxLayout *adminButtonLayout = new QHBoxLayout();
    adminButtonLayout->addStretch();
    adminButtonLayout->addWidget(adminBtn);
    adminButtonLayout->addStretch();

    // Add button layouts to buttons container
    buttonsLayout->addLayout(topButtonsLayout);
    buttonsLayout->addLayout(adminButtonLayout);

    // Center buttons container
    QHBoxLayout *centerButtonsLayout = new QHBoxLayout();
    centerButtonsLayout->addStretch();
    centerButtonsLayout->addWidget(buttonsContainer);
    centerButtonsLayout->addStretch();

    // Layout organization
    mainLayout->addStretch(1);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addLayout(centerButtonsLayout);
    mainLayout->addStretch(2);

    // Connections
    connect(doctorBtn, &QPushButton::clicked, this, &MainWindow::handleDoctorClick);
    connect(patientBtn, &QPushButton::clicked, this, &MainWindow::handlePatientClick);
    connect(adminBtn, &QPushButton::clicked, this, &MainWindow::handleAdminClick);
}

void MainWindow::setupStyles()
{
    // Window styling
    setStyleSheet("background-color: #f5f5f5;");

    // Title style
    QFont titleFont("Arial", 28, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50;");

    // Subtitle style
    QFont subtitleFont("Arial", 18, QFont::Bold);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("color: #7f8c8d; margin-bottom: 30px;");

    // Button styling (same as PatientDashboard)
    QString buttonStyle =
        "QPushButton {"
        "   background-color: #1e90ff;"
        "   color: #ffffff;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 10px 15px;"
        "   font-size: 18px;"
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

    doctorBtn->setStyleSheet(buttonStyle);
    patientBtn->setStyleSheet(buttonStyle);
    adminBtn->setStyleSheet(buttonStyle);
}

// Slot implementations
void MainWindow::handleDoctorClick()
{
    qDebug() << "Doctor button clicked";
    DoctorLoginDialog doctorDialog(this);
    if (doctorDialog.exec() == QDialog::Accepted) {
        qDebug() << "Doctor logged in successfully";
        DoctorDashboard *dashboard = new DoctorDashboard(doctorDialog.getDoctorId());
        dashboard->setAttribute(Qt::WA_DeleteOnClose);
        dashboard->show();
    }
}

void MainWindow::handlePatientClick()
{
    qDebug() << "Patient button clicked";
    PatientLoginDialog patientDialog(this);
    if (patientDialog.exec() == QDialog::Accepted) {
        qDebug() << "Patient logged in successfully";
        // Add logic for what happens after successful login
    } else {
        qDebug() << "Patient login canceled";
    }
}

void MainWindow::handleAdminClick()
{
    qDebug() << "Admin button clicked";
    AdminLoginDialog adminDialog(this);
    if (adminDialog.exec() == QDialog::Accepted) {
        qDebug() << "Admin logged in successfully";
        // Add logic for what happens after successful login
    } else {
        qDebug() << "Admin login canceled";
    }
}

MainWindow::~MainWindow() {}
