#include "patientdashboardprofile.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PatientDashboardProfile::PatientDashboardProfile(const QString &patientId, QWidget *parent)
    : QWidget(parent), patientId(patientId)
{
    setupUI();
    setupStyles();
    fetchPatientProfile();

    setWindowTitle("Patient Profile");
    setMinimumSize(400, 500);
    resize(450, 550);
}

PatientDashboardProfile::~PatientDashboardProfile() {}

void PatientDashboardProfile::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Single Frame for all content
    profileFrame = new QFrame(this);
    profileFrame->setFrameShape(QFrame::NoFrame);
    QVBoxLayout *contentLayout = new QVBoxLayout(profileFrame);
    contentLayout->setContentsMargins(15, 15, 15, 15);
    contentLayout->setSpacing(10);
    contentLayout->setAlignment(Qt::AlignCenter);

    // Title
    titleLabel = new QLabel("Patient Profile", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setToolTip("Detailed patient information");

    // Profile Details
    nameLabel = new QLabel("Name: Loading...", this);
    nameLabel->setAlignment(Qt::AlignLeft);
    nameLabel->setToolTip("Patient's full name");

    ageLabel = new QLabel("Age: Loading...", this);
    ageLabel->setAlignment(Qt::AlignLeft);
    ageLabel->setToolTip("Patient's age");

    contactLabel = new QLabel("Contact: Loading...", this);
    contactLabel->setAlignment(Qt::AlignLeft);
    contactLabel->setToolTip("Patient's contact number");

    diseaseLabel = new QLabel("Disease: Loading...", this);
    diseaseLabel->setAlignment(Qt::AlignLeft);
    diseaseLabel->setToolTip("Primary diagnosis");

    admitDateLabel = new QLabel("Admit Date: Loading...", this);
    admitDateLabel->setAlignment(Qt::AlignLeft);
    admitDateLabel->setToolTip("Date of admission");

    assignedDoctorsCountLabel = new QLabel("Number of Assigned Doctors: Loading...", this);
    assignedDoctorsCountLabel->setAlignment(Qt::AlignLeft);
    assignedDoctorsCountLabel->setToolTip("Total number of doctors assigned");

    assignedDoctorsNamesLabel = new QLabel("Assigned Doctors: Loading...", this);
    assignedDoctorsNamesLabel->setAlignment(Qt::AlignLeft);
    assignedDoctorsNamesLabel->setToolTip("Names of assigned doctors");

    // Back Button
    backButton = new QPushButton("Back", this);
    backButton->setToolTip("Return to the previous screen");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(backButton);

    // Add to content layout
    contentLayout->addWidget(titleLabel);
    contentLayout->addWidget(nameLabel);
    contentLayout->addWidget(ageLabel);
    contentLayout->addWidget(contactLabel);
    contentLayout->addWidget(diseaseLabel);
    contentLayout->addWidget(admitDateLabel);
    contentLayout->addWidget(assignedDoctorsCountLabel);
    contentLayout->addWidget(assignedDoctorsNamesLabel);
    contentLayout->addLayout(buttonLayout);

    mainLayout->addWidget(profileFrame);

    connect(backButton, &QPushButton::clicked, this, &QWidget::close);
}

void PatientDashboardProfile::setupStyles()
{
    setStyleSheet(
        "QWidget {"
        "   background-color: #f5f7fa;"
        "}"
        "QFrame {"
        "   background-color: #ffffff;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 10px;"
        "   box-shadow: 0 2px 8px rgba(0, 0, 0, 0.05);"
        "}"
        );

    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #1a3c5e;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   padding: 5px;"
        "}"
        );

    QString labelStyle =
        "QLabel {"
        "   color: #333333;"
        "   font-size: 16px;"
        "   font-weight: medium;"
        "   padding: 5px;"
        "}";
    nameLabel->setStyleSheet(labelStyle);
    ageLabel->setStyleSheet(labelStyle);
    contactLabel->setStyleSheet(labelStyle);
    diseaseLabel->setStyleSheet(labelStyle);
    admitDateLabel->setStyleSheet(labelStyle);
    assignedDoctorsCountLabel->setStyleSheet(labelStyle);
    assignedDoctorsNamesLabel->setStyleSheet(labelStyle);

    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #ff6b6b;"
        "   color: #ffffff;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 8px 20px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e63939;"
        "}"
        "QPushButton:focus {"
        "   outline: none;"
        "   border: 2px solid #ff8787;"
        "}"
        );
}

void PatientDashboardProfile::fetchPatientProfile()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection failed.";
        nameLabel->setText("Name: Error");
        ageLabel->setText("Age: Error");
        contactLabel->setText("Contact: Error");
        diseaseLabel->setText("Disease: Error");
        admitDateLabel->setText("Admit Date: Error");
        assignedDoctorsCountLabel->setText("Number of Assigned Doctors: Error");
        assignedDoctorsNamesLabel->setText("Assigned Doctors: Error");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT name, age, contact, disease, admitDate FROM patients WHERE id = :patientId");
    query.bindValue(":patientId", patientId);

    if (!query.exec() || !query.next()) {
        qDebug() << "Query failed or no patient found:" << query.lastError().text();
        nameLabel->setText("Name: Not Found");
        ageLabel->setText("Age: Not Found");
        contactLabel->setText("Contact: Not Found");
        diseaseLabel->setText("Disease: Not Found");
        admitDateLabel->setText("Admit Date: Not Found");
        assignedDoctorsCountLabel->setText("Number of Assigned Doctors: Not Found");
        assignedDoctorsNamesLabel->setText("Assigned Doctors: Not Found");
        return;
    }

    QString name = query.value("name").toString();
    int age = query.value("age").toInt();
    QString contact = query.value("contact").toString();
    QString disease = query.value("disease").toString();
    QString admitDate = query.value("admitDate").toString();

    nameLabel->setText("Name: " + name);
    ageLabel->setText("Age: " + QString::number(age));
    contactLabel->setText("Contact: " + contact);
    diseaseLabel->setText("Disease: " + disease);
    admitDateLabel->setText("Admit Date: " + admitDate);

    // Fetch number of assigned doctors
    query.prepare("SELECT COUNT(DISTINCT doctor_id) AS doctor_count FROM appointments WHERE patient_id = :patientId");
    query.bindValue(":patientId", patientId);
    if (query.exec() && query.next()) {
        int doctorCount = query.value("doctor_count").toInt();
        assignedDoctorsCountLabel->setText("Number of Assigned Doctors: " + QString::number(doctorCount));
    } else {
        assignedDoctorsCountLabel->setText("Number of Assigned Doctors: 0");
    }

    // Fetch names of assigned doctors
    query.prepare("SELECT DISTINCT d.name FROM doctors d "
                  "JOIN appointments a ON d.id = a.doctor_id "
                  "WHERE a.patient_id = :patientId");
    query.bindValue(":patientId", patientId);
    QStringList doctorNames;
    if (query.exec()) {
        while (query.next()) {
            doctorNames.append(query.value("name").toString());
        }
    }
    assignedDoctorsNamesLabel->setText("Assigned Doctors: " + (doctorNames.isEmpty() ? "None" : doctorNames.join(", ")));
}
