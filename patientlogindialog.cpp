#include "patientlogindialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include "patientdashboard.h"

PatientLoginDialog::PatientLoginDialog(QWidget *parent) : QDialog(parent)
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database 'hospitalConnection' (ned_hospital.db) is not open in PatientLoginDialog!";
    } else {
        qDebug() << "Database 'hospitalConnection' (ned_hospital.db) is open in PatientLoginDialog";
    }

    setupUI();
    setupStyles();
}

void PatientLoginDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    patienttitleLabel = new QLabel("Welcome Back, PATIENT", this);
    patienttitleLabel->setAlignment(Qt::AlignCenter);

    patientsubtitleLabel = new QLabel("Please Login,", this);
    patientsubtitleLabel->setAlignment(Qt::AlignLeft);

    errorLabel = new QLabel("Wrong ID/Password", this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);

    patientLabel = new QLabel("Patient ID:", this);
    patientIdInput = new QLineEdit(this);
    patientIdInput->setPlaceholderText("eg: 123456");

    patientpasswordLabel = new QLabel("Password:", this);
    patientpasswordInput = new QLineEdit(this);
    patientpasswordInput->setEchoMode(QLineEdit::Password);

    patientloginButton = new QPushButton("Login", this);
    patientbackButton = new QPushButton("Back", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(patientloginButton);
    buttonLayout->addWidget(patientbackButton);

    mainLayout->addWidget(patienttitleLabel);
    mainLayout->addWidget(patientsubtitleLabel);
    mainLayout->addWidget(errorLabel);
    mainLayout->addWidget(patientLabel);
    mainLayout->addWidget(patientIdInput);
    mainLayout->addWidget(patientpasswordLabel);
    mainLayout->addWidget(patientpasswordInput);
    mainLayout->addLayout(buttonLayout);

    connect(patientloginButton, &QPushButton::clicked, this, &PatientLoginDialog::handlePatientLoginClick);
    connect(patientbackButton, &QPushButton::clicked, this, &PatientLoginDialog::handlePatientBackClick);
}

void PatientLoginDialog::setupStyles()
{
    setStyleSheet("background-color: #f5f5f5;");

    QFont titleFont("Arial", 20, QFont::Bold);
    patienttitleLabel->setFont(titleFont);
    patienttitleLabel->setStyleSheet("color: #2c3e50;");

    QFont subtitleFont("Arial", 14, QFont::Normal);
    patientsubtitleLabel->setFont(subtitleFont);
    patientsubtitleLabel->setStyleSheet("color: #7f8c8d; margin-bottom: 20px;");

    QFont errorFont("Arial", 12, QFont::Normal);
    errorLabel->setFont(errorFont);
    errorLabel->setStyleSheet("color: #e74c3c; margin-bottom: 10px;");

    QFont labelFont("Arial", 12, QFont::Normal);
    patientLabel->setFont(labelFont);
    patientpasswordLabel->setFont(labelFont);
    patientLabel->setStyleSheet("color: #2c3e50;");
    patientpasswordLabel->setStyleSheet("color: #2c3e50;");

    QString inputStyle =
        "QLineEdit {"
        "   border: 1px solid #ccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-size: 14px;"
        "}";
    patientIdInput->setStyleSheet(inputStyle);
    patientpasswordInput->setStyleSheet(inputStyle);

    QString loginButtonStyle =
        "QPushButton {"
        "   background-color: #e74c3c;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "   font-size: 14px;"
        "   min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #c0392b;"
        "}";
    QString backButtonStyle =
        "QPushButton {"
        "   background-color: #34495e;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "   font-size: 14px;"
        "   min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2c3e50;"
        "}";

    patientloginButton->setStyleSheet(loginButtonStyle);
    patientbackButton->setStyleSheet(backButtonStyle);
}

void PatientLoginDialog::handlePatientLoginClick()
{
    qDebug() << "Patient Login button clicked";

    errorLabel->setVisible(false);

    QString id = patientIdInput->text().trimmed();
    QString password = patientpasswordInput->text().trimmed();

    if (id.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Please enter both Patient ID and Password");
        errorLabel->setVisible(true);
        qDebug() << "Login failed: Patient ID or Password is empty";
        return;
    }

    bool isValid = validateCredentials(id, password);

    if (isValid) {
        qDebug() << "Patient login successful for ID:" << id;
        // Fixed: Create PatientData and pass it to PatientDashboard
        PatientData data;
        data.id = id; // Set the patient ID for database query in PatientDashboard
        PatientDashboard *dashboard = new PatientDashboard(data);
        dashboard->setAttribute(Qt::WA_DeleteOnClose);
        dashboard->setWindowState(Qt::WindowMaximized);
        dashboard->show();
        accept();
    } else {
        qDebug() << "Patient login failed: Invalid ID or password";
        errorLabel->setText("Wrong ID/Password");
        errorLabel->setVisible(true);
    }
}

bool PatientLoginDialog::validateCredentials(const QString &patientId, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qCritical() << "Database connection is not open for ned_hospital.db!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT password FROM patients WHERE id = :patientId");
    query.bindValue(":patientId", patientId);

    if (!query.exec()) {
        qCritical() << "Query failed for ned_hospital.db:" << query.lastError().text();
        qCritical() << "Last query:" << query.lastQuery();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            qDebug() << "Login successful for patient ID:" << patientId;
            return true;
        }
    }

    qDebug() << "No matching patient found with ID:" << patientId << "or incorrect password in ned_hospital.db";
    return false;
}

void PatientLoginDialog::handlePatientBackClick()
{
    qDebug() << "Patient Back button clicked";
    reject();
}

PatientLoginDialog::~PatientLoginDialog() {}
