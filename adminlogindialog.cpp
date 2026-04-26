#include "adminlogindialog.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QDebug>
#include "admindashboard.h"

AdminLoginDialog::AdminLoginDialog(QWidget *parent) : QDialog(parent)
{
    // Verify database connection at construction
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qCritical() << "Database 'hospitalConnection' (ned_hospital.db) is not open in AdminLoginDialog! Path:" << db.databaseName();
    } else {
        qDebug() << "Database 'hospitalConnection' (ned_hospital.db) is open in AdminLoginDialog. Path:" << db.databaseName();
    }

    setupUI();
    setupStyles();
}

void AdminLoginDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    admintitleLabel = new QLabel("Welcome Back, ADMIN", this);
    admintitleLabel->setAlignment(Qt::AlignCenter);

    adminsubtitleLabel = new QLabel("Please Login,", this);
    adminsubtitleLabel->setAlignment(Qt::AlignLeft);

    // Error message label (initially hidden)
    errorLabel = new QLabel("Wrong ID/Password", this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false); // Hidden by default

    adminLabel = new QLabel("Admin ID:", this);
    adminIdInput = new QLineEdit(this);
    adminIdInput->setPlaceholderText("eg: 123456");

    adminpasswordLabel = new QLabel("Password:", this);
    adminpasswordInput = new QLineEdit(this);
    adminpasswordInput->setEchoMode(QLineEdit::Password);

    adminloginButton = new QPushButton("Login", this);
    adminbackButton = new QPushButton("Back", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(adminloginButton);
    buttonLayout->addWidget(adminbackButton);

    mainLayout->addWidget(admintitleLabel);
    mainLayout->addWidget(adminsubtitleLabel);
    mainLayout->addWidget(errorLabel); // Added error label
    mainLayout->addWidget(adminLabel);
    mainLayout->addWidget(adminIdInput);
    mainLayout->addWidget(adminpasswordLabel);
    mainLayout->addWidget(adminpasswordInput);
    mainLayout->addLayout(buttonLayout);

    connect(adminloginButton, &QPushButton::clicked, this, &AdminLoginDialog::handleAdminLoginClick);
    connect(adminbackButton, &QPushButton::clicked, this, &AdminLoginDialog::handleAdminBackClick);
}

void AdminLoginDialog::setupStyles()
{
    setStyleSheet("background-color: #f5f5f5;");

    QFont titleFont("Arial", 20, QFont::Bold);
    admintitleLabel->setFont(titleFont);
    admintitleLabel->setStyleSheet("color: #2c3e50;");

    QFont subtitleFont("Arial", 14, QFont::Normal);
    adminsubtitleLabel->setFont(subtitleFont);
    adminsubtitleLabel->setStyleSheet("color: #7f8c8d; margin-bottom: 20px;");

    // Error label style
    QFont errorFont("Arial", 12, QFont::Normal);
    errorLabel->setFont(errorFont);
    errorLabel->setStyleSheet("color: #e74c3c; margin-bottom: 10px;"); // Red text for error

    QFont labelFont("Arial", 12, QFont::Normal);
    adminLabel->setFont(labelFont);
    adminpasswordLabel->setFont(labelFont);
    adminLabel->setStyleSheet("color: #2c3e50;");
    adminpasswordLabel->setStyleSheet("color: #2c3e50;");

    QString inputStyle =
        "QLineEdit {"
        "   border: 1px solid #ccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-size: 14px;"
        "}";
    adminIdInput->setStyleSheet(inputStyle);
    adminpasswordInput->setStyleSheet(inputStyle);

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

    adminloginButton->setStyleSheet(loginButtonStyle);
    adminbackButton->setStyleSheet(backButtonStyle);
}

void AdminLoginDialog::handleAdminLoginClick()
{
    qDebug() << "Admin Login button clicked";

    // Reset error label visibility
    errorLabel->setVisible(false);

    // Basic validation: check if inputs are not empty
    QString id = adminIdInput->text().trimmed();
    QString password = adminpasswordInput->text().trimmed();

    if (id.isEmpty() || password.isEmpty()) {
        errorLabel->setText("Please enter both Admin ID and Password");
        errorLabel->setVisible(true);
        qDebug() << "Login failed: Admin ID or Password is empty";
        return;
    }

    // Validate credentials using the database
    bool isValid = validateCredentials(id, password);

    if (isValid) {
        qDebug() << "Admin login successful for ID:" << id;
        // Create and show the AdminDashboard
        AdminDashboard *dashboard = new AdminDashboard(id, nullptr);
        dashboard->setAttribute(Qt::WA_DeleteOnClose); // Added for consistency
        dashboard->show();
        accept(); // Close the login dialog
    } else {
        qDebug() << "Admin login failed: Invalid ID or password";
        errorLabel->setText("Wrong ID/Password");
        errorLabel->setVisible(true);
    }
}

bool AdminLoginDialog::validateCredentials(const QString &adminId, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qCritical() << "Database connection is not open for ned_hospital.db! Path:" << db.databaseName();
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT password FROM admins WHERE id = :adminId");
    query.bindValue(":adminId", adminId);

    if (!query.exec()) {
        qCritical() << "Query failed for ned_hospital.db:" << query.lastError().text();
        qCritical() << "Last query:" << query.lastQuery();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            qDebug() << "Login successful for admin ID:" << adminId;
            return true;
        }
    }

    qDebug() << "No matching admin found with ID:" << adminId << " or incorrect password in ned_hospital.db";
    return false;
}

void AdminLoginDialog::handleAdminBackClick()
{
    qDebug() << "Admin Back button clicked";
    reject();
}

AdminLoginDialog::~AdminLoginDialog() {}
