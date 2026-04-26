#include "DoctorDashboard.h"
#include <QDebug>
#include <QScreen>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>


DoctorDashboard::DoctorDashboard(const QString &currentDoctor, QWidget *parent)
    : QWidget(parent), currentDoctor(currentDoctor)
{
    qDebug() << "Constructing DoctorDashboard for doctor:" << currentDoctor;
    if (currentDoctor.isEmpty()) {
        qWarning() << "currentDoctor is empty, this may cause issues.";
    }
    todaysWorkDialog = nullptr;
    setupUI();
    setupStyles();
    loadDoctorDetails();

    setWindowTitle("NED HOSPITAL SYSTEM - DOCTOR PANEL");
    setMinimumSize(800, 600);
    this->showMaximized();
    qDebug() << "DoctorDashboard construction completed.";
}

DoctorDashboard::~DoctorDashboard()
{
    qDebug() << "Destroying DoctorDashboard, deleting todaysWorkDialog";
    delete todaysWorkDialog;
}

void DoctorDashboard::setupUI()
{
    qDebug() << "Setting up UI for DoctorDashboard";
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(30, 20, 30, 20);

    titleLabel = new QLabel("NED HOSPITAL SYSTEM - DOCTOR PANEL", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setToolTip("Doctor Dashboard");

    infoFrame = new QFrame(this);
    infoFrame->setFrameShape(QFrame::StyledPanel);
    infoFrame->setMaximumWidth(400);
    QVBoxLayout *infoLayout = new QVBoxLayout(infoFrame);
    infoLayout->setContentsMargins(15, 10, 15, 10);
    infoLayout->setSpacing(8);

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    QString name, specialization;
    int age = 0;
    if (!db.isOpen()) {
        qWarning() << "Database connection failed.";
        nameLabel = new QLabel("NAME: Error", infoFrame);
        ageLabel = new QLabel("AGE: Error", infoFrame);
        domainLabel = new QLabel("SPECIALIZATION: Error", infoFrame);
    } else {
        QSqlQuery query(db);
        query.prepare("SELECT name, age, specialization FROM doctors WHERE id = :id");
        query.bindValue(":id", currentDoctor);

        if (query.exec() && query.next()) {
            name = query.value("name").toString();
            age = query.value("age").toInt();
            specialization = query.value("specialization").toString();
            qDebug() << "Doctor details loaded: Name=" << name;
        } else {
            qWarning() << "Query failed or no doctor found:" << query.lastError().text();
            name = "Not Found";
            age = 0;
            specialization = "Not Found";
        }

        nameLabel = new QLabel("NAME: " + name, infoFrame);
        ageLabel = new QLabel("AGE: " + QString::number(age), infoFrame);
        domainLabel = new QLabel("SPECIALIZATION: " + specialization, infoFrame);
    }

    nameLabel->setToolTip("Doctor's full name");
    ageLabel->setToolTip("Doctor's age");
    domainLabel->setToolTip("Doctor's medical specialization");

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(ageLabel);
    infoLayout->addWidget(domainLabel);
    infoLayout->setAlignment(Qt::AlignLeft);

    nameLabel->update();
    ageLabel->update();
    domainLabel->update();
    infoFrame->update();

    dashboardLabel = new QLabel("DASHBOARD", this);
    dashboardLabel->setAlignment(Qt::AlignCenter);
    dashboardLabel->setToolTip("Available actions");

    QWidget *buttonsContainer = new QWidget(this);
    QGridLayout *buttonsLayout = new QGridLayout(buttonsContainer);
    buttonsLayout->setHorizontalSpacing(10);
    buttonsLayout->setVerticalSpacing(10);
    buttonsLayout->setContentsMargins(0, 0, 0, 0);

    todaysWorkBtn = new QPushButton("TODAY'S WORK", buttonsContainer);
    reportsBtn = new QPushButton("REPORTS", buttonsContainer);
    salaryBtn = new QPushButton("SALARY", buttonsContainer);

    todaysWorkBtn->setToolTip("View today's tasks and appointments");
    reportsBtn->setToolTip("Access patient reports and records");
    salaryBtn->setToolTip("View salary details");

    buttonsLayout->addWidget(todaysWorkBtn, 0, 0);
    buttonsLayout->addWidget(reportsBtn, 0, 1);
    buttonsLayout->addWidget(salaryBtn, 0, 2);

    QHBoxLayout *centerButtonsLayout = new QHBoxLayout();
    centerButtonsLayout->addStretch();
    centerButtonsLayout->addWidget(buttonsContainer);
    centerButtonsLayout->addStretch();

    logoutBtn = new QPushButton("Logout", this);
    logoutBtn->setToolTip("Log out of the system");
    QHBoxLayout *logoutLayout = new QHBoxLayout();
    logoutLayout->addStretch();
    logoutLayout->addWidget(logoutBtn);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoFrame, 0, Qt::AlignHCenter);
    mainLayout->addWidget(dashboardLabel);
    mainLayout->addLayout(centerButtonsLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(logoutLayout);

    connect(todaysWorkBtn, &QPushButton::clicked, this, &DoctorDashboard::handleTodaysWorkClick);
    connect(reportsBtn, &QPushButton::clicked, this, &DoctorDashboard::handleReportsClick);
    connect(salaryBtn, &QPushButton::clicked, this, &DoctorDashboard::handleSalaryClick);
    connect(logoutBtn, &QPushButton::clicked, this, &DoctorDashboard::handleLogoutClick);
    qDebug() << "UI setup completed.";
}

void DoctorDashboard::setupStyles()
{
    qDebug() << "Setting up styles for DoctorDashboard";
    setStyleSheet("background-color: #f0f2f5;");

    QFont titleFont("Sans Serif", 26, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet(
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
    nameLabel->setFont(infoFont);
    ageLabel->setFont(infoFont);
    domainLabel->setFont(infoFont);

    QString infoLabelStyle =
        "QLabel {"
        "   color: #000000;"
        "   padding: 8px 12px;"
        "}";
    nameLabel->setStyleSheet(infoLabelStyle);
    ageLabel->setStyleSheet(infoLabelStyle);
    domainLabel->setStyleSheet(infoLabelStyle);

    QFont dashboardFont("Sans Serif", 20, QFont::Bold);
    dashboardLabel->setFont(dashboardFont);
    dashboardLabel->setStyleSheet(
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

    todaysWorkBtn->setStyleSheet(buttonStyle);
    reportsBtn->setStyleSheet(buttonStyle);
    salaryBtn->setStyleSheet(buttonStyle);

    logoutBtn->setStyleSheet(
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
    qDebug() << "Styles setup completed.";
}

void DoctorDashboard::loadDoctorDetails()
{
    qDebug() << "Loading doctor details for ID:" << currentDoctor;
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qWarning() << "Database connection failed in loadDoctorDetails";
        nameLabel->setText("NAME: Database Error");
        ageLabel->setText("AGE: Database Error");
        domainLabel->setText("SPECIALIZATION: Database Error");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT name, age, specialization FROM doctors WHERE id = :id");
    query.bindValue(":id", currentDoctor);

    if (!query.exec()) {
        qWarning() << "Query execution failed:" << query.lastError().text();
        nameLabel->setText("NAME: Query Error");
        ageLabel->setText("AGE: Query Error");
        domainLabel->setText("SPECIALIZATION: Query Error");
        return;
    }

    if (query.next()) {
        nameLabel->setText("NAME: " + query.value("name").toString());
        ageLabel->setText("AGE: " + QString::number(query.value("age").toInt()));
        domainLabel->setText("SPECIALIZATION: " + query.value("specialization").toString());
        qDebug() << "Doctor details loaded successfully: Name=" << query.value("name").toString();
    } else {
        qWarning() << "No doctor found for ID:" << currentDoctor;
        nameLabel->setText("NAME: Not Found");
        ageLabel->setText("AGE: Not Found");
        domainLabel->setText("SPECIALIZATION: Not Found");
    }
}

void DoctorDashboard::handleTodaysWorkClick()
{
    qDebug() << "Today's Work button clicked.";
    if (!todaysWorkDialog) {
        qDebug() << "Creating new todaysWorkDialog for doctor:" << currentDoctor;
        todaysWorkDialog = new DoctorDashboardTodaysWork(currentDoctor, this);
    }
    qDebug() << "Showing todaysWorkDialog";
    todaysWorkDialog->show();
    todaysWorkDialog->raise();
    todaysWorkDialog->activateWindow();
}

void DoctorDashboard::handleReportsClick()
{
    qDebug() << "Reports button clicked for doctor ID:" << currentDoctor;
    if (currentDoctor.isEmpty()) {
        qCritical() << "Cannot open reports dialog: empty doctor ID.";
        return;
    }
    DoctorReportsDialog *reportsDialog = new DoctorReportsDialog(currentDoctor, this);
    reportsDialog->setAttribute(Qt::WA_DeleteOnClose);
    reportsDialog->exec();

}
void DoctorDashboard::handleSalaryClick()
{
    qDebug() << "Salary button clicked for doctor ID:" << currentDoctor;
    if (currentDoctor.isEmpty()) {
        qCritical() << "Cannot open salary dialog: empty doctor ID.";
        return;
    }
    DoctorDashboardSalary *salaryDialog = new DoctorDashboardSalary(currentDoctor, this);
    salaryDialog->setAttribute(Qt::WA_DeleteOnClose);
    salaryDialog->exec();
}

void DoctorDashboard::handleLogoutClick()
{
    qDebug() << "Logout button clicked.";
    close();
}
