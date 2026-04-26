#include "doctorlogindialog.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>

DoctorLoginDialog::DoctorLoginDialog(QWidget *parent) : QDialog(parent)
{
    // Verify database connection at construction
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database 'hospitalConnection' is not open in DoctorLoginDialog!";
    } else {
        qDebug() << "Database 'hospitalConnection' is open in DoctorLoginDialog";
    }

    setupUI();
    setupStyles();
}

void DoctorLoginDialog::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    titleLabel = new QLabel("Welcome Back, DOCTOR", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Subtitle
    subtitleLabel = new QLabel("Please Login,", this);
    subtitleLabel->setAlignment(Qt::AlignLeft);

    // Error message label (initially hidden)
    errorLabel = new QLabel("Wrong ID/Password", this);
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false); // Hidden by default

    // Doctor ID field
    doctorLabel = new QLabel("Doctor ID:", this);
    doctorIdInput = new QLineEdit(this);
    doctorIdInput->setPlaceholderText("eg: 123456");

    // Password field
    passwordLabel = new QLabel("Password:", this);
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password); // Hide password input

    // Buttons
    loginButton = new QPushButton("Login", this);
    backButton = new QPushButton("Back", this);

    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(backButton);

    // Add widgets to main layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addWidget(errorLabel);
    mainLayout->addWidget(doctorLabel);
    mainLayout->addWidget(doctorIdInput);
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordInput);
    mainLayout->addLayout(buttonLayout);

    // Connections
    connect(loginButton, &QPushButton::clicked, this, &DoctorLoginDialog::handleLoginClick);
    connect(backButton, &QPushButton::clicked, this, &DoctorLoginDialog::handleBackClick);
}

void DoctorLoginDialog::setupStyles()
{
    // Window styling
    setStyleSheet("background-color: #f5f5f5;");

    // Title style
    QFont titleFont("Arial", 20, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50;");

    // Subtitle style
    QFont subtitleFont("Arial", 14, QFont::Normal);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("color: #7f8c8d; margin-bottom: 20px;");

    // Error label style
    QFont errorFont("Arial", 12, QFont::Normal);
    errorLabel->setFont(errorFont);
    errorLabel->setStyleSheet("color: #e74c3c; margin-bottom: 10px;"); // Red text for error

    // Label style
    QFont labelFont("Arial", 12, QFont::Normal);
    doctorLabel->setFont(labelFont);
    passwordLabel->setFont(labelFont);
    doctorLabel->setStyleSheet("color: #2c3e50;");
    passwordLabel->setStyleSheet("color: #2c3e50;");

    // Input field styling
    QString inputStyle =
        "QLineEdit {"
        "   border: 1px solid #ccc;"
        "   border-radius: 5px;"
        "   padding: 5px;"
        "   font-size: 14px;"
        "}";

    doctorIdInput->setStyleSheet(inputStyle);
    passwordInput->setStyleSheet(inputStyle);

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

    loginButton->setStyleSheet(loginButtonStyle);
    backButton->setStyleSheet(backButtonStyle);
}

void DoctorLoginDialog::handleLoginClick()
{
    qDebug() << "Login button clicked";

    // Reset error label visibility
    errorLabel->setVisible(false);

    // Basic validation: check if inputs are not empty
    if (doctorIdInput->text().isEmpty() || passwordInput->text().isEmpty()) {
        errorLabel->setText("Please enter both Doctor ID and Password");
        errorLabel->setVisible(true);
        return;
    }

    // Validate credentials using the database
    QString doctorId = doctorIdInput->text();
    QString password = passwordInput->text();

    bool isValid = validateCredentials(doctorId, password);

    if (isValid) {
        qDebug() << "Login successful for Doctor ID:" << doctorId;
        emit loginSuccess(doctorId); // Emit signal with doctor ID
        accept(); // Close the login dialog
    } else {
        qDebug() << "Invalid credentials for Doctor ID:" << doctorId;
        errorLabel->setText("Wrong ID/Password");
        errorLabel->setVisible(true);
    }
}

bool DoctorLoginDialog::validateCredentials(const QString &doctorId, const QString &password)
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qCritical() << "Database connection is not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT password FROM doctors WHERE id = :doctorId");
    query.bindValue(":doctorId", doctorId);

    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
        qCritical() << "Last query:" << query.lastQuery();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        if (storedPassword == password) {
            qDebug() << "Login successful for doctor ID:" << doctorId;
            return true;
        }
    }

    qDebug() << "No matching doctor found with ID:" << doctorId << "or incorrect password";
    return false;
}

void DoctorLoginDialog::handleBackClick()
{
    qDebug() << "Back button clicked";
    reject(); // Close dialog and return QDialog::Rejected
}

DoctorLoginDialog::~DoctorLoginDialog() {}
