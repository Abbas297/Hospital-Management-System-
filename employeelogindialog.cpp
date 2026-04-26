#include "employeelogindialog.h"
#include "employeedashboard.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>

EmployeeLoginDialog::EmployeeLoginDialog(QWidget *parent) : QDialog(parent)
{
    // Verify database connection at construction
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database 'hospitalConnection' (ned_hospital.db) is not open in EmployeeLoginDialog!";
    } else {
        qDebug() << "Database 'hospitalConnection' (ned_hospital.db) is open in EmployeeLoginDialog";
    }

    setupUI();
    setupStyles();
}

void EmployeeLoginDialog::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    employeetitleLabel = new QLabel("Welcome Back, EMPLOYEE", this);
    employeetitleLabel->setAlignment(Qt::AlignCenter);

    // Subtitle
    employeesubtitleLabel = new QLabel("Please Login,", this);
    employeesubtitleLabel->setAlignment(Qt::AlignLeft);

    // Error message label (initially hidden)
    errorLabel = new QLabel("Wrong ID/Password", this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false); // Hidden by default

    // Employee ID field
    employeeLabel = new QLabel("Employee ID:", this);
    employeeIdInput = new QLineEdit(this);
    employeeIdInput->setPlaceholderText("eg: 123456");

    // Password field
    employeepasswordLabel = new QLabel("Password:", this);
    employeepasswordInput = new QLineEdit(this);
    employeepasswordInput->setEchoMode(QLineEdit::Password); // Hide password input

    // Buttons
    employeeloginButton = new QPushButton("Login", this);
    employeebackButton = new QPushButton("Back", this);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(employeeloginButton);
    buttonLayout->addWidget(employeebackButton);

    // Add widgets to main layout
    mainLayout->addWidget(employeetitleLabel);
    mainLayout->addWidget(employeesubtitleLabel);
    mainLayout->addWidget(errorLabel); // Added error label
    mainLayout->addWidget(employeeLabel);
    mainLayout->addWidget(employeeIdInput);
    mainLayout->addWidget(employeepasswordLabel);
    mainLayout->addWidget(employeepasswordInput);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(employeeloginButton, &QPushButton::clicked, this, &EmployeeLoginDialog::handleEmployeeLoginClick);
    connect(employeebackButton, &QPushButton::clicked, this, &EmployeeLoginDialog::handleEmployeeBackClick);
}

void EmployeeLoginDialog::setupStyles()
{
    // Window styling
    setStyleSheet("background-color: #f5f5f5;");

    // Title style
    QFont titleFont("Arial", 20, QFont::Bold);
    employeetitleLabel->setFont(titleFont);
    employeetitleLabel->setStyleSheet("color: #2c3e50;");

    // Subtitle style
    QFont subtitleFont("Arial", 14, QFont::Normal);
    employeesubtitleLabel->setFont(subtitleFont);
    employeesubtitleLabel->setStyleSheet("color: #7f8c8d; margin-bottom: 20px;");

    // Error label style
    QFont errorFont("Arial", 12, QFont::Normal);
    errorLabel->setFont(errorFont);
    errorLabel->setStyleSheet("color: #e74c3c; margin-bottom: 10px;"); // Red text for error

    // Label style
    QFont labelFont("Arial", 12, QFont::Normal);
    employeeLabel->setFont(labelFont);
    employeepasswordLabel->setFont(labelFont);
    employeeLabel->setStyleSheet("color: #2c3e50;");
    employeepasswordLabel->setStyleSheet("color: #2c3e50;");

    // Input field styling
    QString inputStyle =
        "QLineEdit {"
        "   border: 1px solid #ccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-size: 14px;"
        "}";
    employeeIdInput->setStyleSheet(inputStyle);
    employeepasswordInput->setStyleSheet(inputStyle);

    // Button styling
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

    employeeloginButton->setStyleSheet(loginButtonStyle);
    employeebackButton->setStyleSheet(backButtonStyle);
}

void EmployeeLoginDialog::handleEmployeeLoginClick()
{
    qDebug() << "Employee Login button clicked";

    // Reset error label visibility
    errorLabel->setVisible(false);

    // Basic validation: check if inputs are not empty
    QString id = employeeIdInput->text().trimmed();
    QString password = employeepasswordInput->text().trimmed();

    if (id.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Please enter both Employee ID and Password");
        errorLabel->setVisible(true);
        qDebug() << "Login failed: Employee ID or Password is empty";
        return;
    }

    // Validate credentials using the database
    bool isValid = validateCredentials(id, password);

    if (isValid) {
        qDebug() << "Employee login successful for ID:" << id;
        // Create and show the EmployeeDashboard
        EmployeeDashboard *dashboard = new EmployeeDashboard(id, nullptr);
        dashboard->setAttribute(Qt::WA_DeleteOnClose);
        dashboard->show();
        accept(); // Close the login dialog
    } else {
        qDebug() << "Employee login failed: Invalid ID or password";
        errorLabel->setText("Wrong ID/Password");
        errorLabel->setVisible(true);
    }
}

bool EmployeeLoginDialog::validateCredentials(const QString &employeeId, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qCritical() << "Database connection is not open for ned_hospital.db!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT password FROM employees WHERE id = :employeeId");
    query.bindValue(":employeeId", employeeId);

    if (!query.exec()) {
        qCritical() << "Query failed for ned_hospital.db:" << query.lastError().text();
        qCritical() << "Last query:" << query.lastQuery();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            qDebug() << "Login successful for employee ID:" << employeeId;
            return true;
        }
    }

    qDebug() << "No matching employee found with ID:" << employeeId << "or incorrect password in ned_hospital.db";
    return false;
}

void EmployeeLoginDialog::handleEmployeeBackClick()
{
    qDebug() << "Employee Back button clicked";
    reject(); // Close dialog and return QDialog::Rejected
}

EmployeeLoginDialog::~EmployeeLoginDialog() {}
