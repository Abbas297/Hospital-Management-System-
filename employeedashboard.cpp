#include "employeedashboard.h"
#include <QDebug>

EmployeeDashboard::EmployeeDashboard(const QString currentEmployee, QWidget *parent)
    : QWidget(parent), currentEmployee(currentEmployee) // Fixed: Properly initialize currentEmployee
{
    setupUI();
    setupStyles();
    loadEmployeeDetails();
    setWindowTitle("Employee Dashboard");
    setWindowState(Qt::WindowMaximized);
}

void EmployeeDashboard::loadEmployeeDetails()
{
    // Updated: Fetch employee details from the database
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection failed.";
        nameLabel->setText("NAME: Error");
        ageLabel->setText("AGE: Error");
        departmentLabel->setText("DEPARTMENT: Error");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT name, age, department FROM employees WHERE id = :id");
    query.bindValue(":id", currentEmployee);

    if (query.exec() && query.next()) {
        nameLabel->setText("NAME: " + query.value("name").toString());
        ageLabel->setText("AGE: " + query.value("age").toString());
        departmentLabel->setText("DEPARTMENT: " + query.value("department").toString());
    } else {
        qDebug() << "Query failed or no employee found:" << query.lastError().text();
        nameLabel->setText("NAME: Not Found");
        ageLabel->setText("AGE: Not Found");
        departmentLabel->setText("DEPARTMENT: Not Found");
    }
}

void EmployeeDashboard::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(50, 40, 50, 40);

    titleLabel = new QLabel("NED HOSPITAL SYSTEM - EMPLOYEE PANEL", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *infoLayout = new QHBoxLayout();
    QVBoxLayout *labelsLayout = new QVBoxLayout();

    nameLabel = new QLabel("NAME:", this);
    ageLabel = new QLabel("AGE:", this);
    departmentLabel = new QLabel("DEPARTMENT:", this);

    labelsLayout->addWidget(nameLabel);
    labelsLayout->addWidget(ageLabel);
    labelsLayout->addWidget(departmentLabel);
    infoLayout->addLayout(labelsLayout);
    infoLayout->addStretch();

    dashboardLabel = new QLabel("DASHBOARD", this);
    dashboardLabel->setAlignment(Qt::AlignCenter);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    workBtn = new QPushButton("TODAY'S WORK", this);
    reportsBtn = new QPushButton("REPORTS", this);
    salaryBtn = new QPushButton("SALARY", this);

    buttonsLayout->addWidget(workBtn);
    buttonsLayout->addWidget(reportsBtn);
    buttonsLayout->addWidget(salaryBtn);

    logoutBtn = new QPushButton("Logout", this);
    QHBoxLayout *logoutLayout = new QHBoxLayout();
    logoutLayout->addStretch();
    logoutLayout->addWidget(logoutBtn);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(infoLayout);
    mainLayout->addWidget(dashboardLabel);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(logoutLayout);

    connect(workBtn, &QPushButton::clicked, this, &EmployeeDashboard::handleWorkClick);
    connect(reportsBtn, &QPushButton::clicked, this, &EmployeeDashboard::handleReportsClick);
    connect(salaryBtn, &QPushButton::clicked, this, &EmployeeDashboard::handleSalaryClick);
    connect(logoutBtn, &QPushButton::clicked, this, &EmployeeDashboard::handleLogoutClick);
}

void EmployeeDashboard::setupStyles()
{
    setStyleSheet("background-color: #f5f5f5;");

    QFont titleFont("Arial", 24, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #2c3e50; margin-bottom: 20px;");

    QFont infoFont("Arial", 14, QFont::Bold);
    nameLabel->setFont(infoFont);
    ageLabel->setFont(infoFont);
    departmentLabel->setFont(infoFont);

    QString infoLabelStyle = "color: #2c3e50; margin-bottom: 10px;";
    nameLabel->setStyleSheet(infoLabelStyle);
    ageLabel->setStyleSheet(infoLabelStyle);
    departmentLabel->setStyleSheet(infoLabelStyle);

    QFont dashboardFont("Arial", 18, QFont::Bold);
    dashboardLabel->setFont(dashboardFont);
    dashboardLabel->setStyleSheet("color: #2c3e50; margin-top: 40px; margin-bottom: 30px;");

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #3498db;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 12px 15px;"
        "   font-size: 14px;"
        "   min-width: 150px;"
        "   margin: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #2980b9;"
        "}";

    QString logoutButtonStyle =
        "QPushButton {"
        "   background-color: #e74c3c;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 10px;"
        "   font-size: 14px;"
        "   min-width: 100px;"
        "   margin-top: 20px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #c0392b;"
        "}";

    workBtn->setStyleSheet(buttonStyle);
    reportsBtn->setStyleSheet(buttonStyle);
    salaryBtn->setStyleSheet(buttonStyle);
    logoutBtn->setStyleSheet(logoutButtonStyle);
}

void EmployeeDashboard::handleWorkClick()
{
    qDebug() << "Today's Work button clicked";
}

void EmployeeDashboard::handleReportsClick()
{
    qDebug() << "Reports button clicked";
}

void EmployeeDashboard::handleSalaryClick()
{
    qDebug() << "Salary button clicked";
}

void EmployeeDashboard::handleLogoutClick()
{
    qDebug() << "Logout button clicked";
    close();
}

EmployeeDashboard::~EmployeeDashboard() {}
