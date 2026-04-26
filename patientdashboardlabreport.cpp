#include "patientdashboardlabreport.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PatientDashboardLabReport::PatientDashboardLabReport(const QString &patientId, QWidget *parent)
    : QWidget(parent), patientId(patientId)
{
    setupUI();
    setupStyles();
    fetchLabReportAvailability();

    setWindowTitle("Lab Reports");
    setMinimumSize(400, 200); // Smaller window size
    resize(450, 250); // Compact size
}

PatientDashboardLabReport::~PatientDashboardLabReport() {}

void PatientDashboardLabReport::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10); // Reduced spacing
    mainLayout->setContentsMargins(15, 15, 15, 15); // Reduced margins

    // Single Frame for all content
    statusFrame = new QFrame(this);
    statusFrame->setFrameShape(QFrame::NoFrame);
    QVBoxLayout *contentLayout = new QVBoxLayout(statusFrame);
    contentLayout->setContentsMargins(15, 15, 15, 15);
    contentLayout->setSpacing(10);
    contentLayout->setAlignment(Qt::AlignCenter);

    // Title
    titleLabel = new QLabel("Lab Report Status", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setToolTip("Lab report availability status");

    // Status Icon and Label
    QHBoxLayout *statusRow = new QHBoxLayout();
    statusRow->setAlignment(Qt::AlignCenter);
    statusRow->setSpacing(8);

    statusIcon = new QLabel(this);
    statusIcon->setFixedSize(20, 20);
    statusIcon->setAlignment(Qt::AlignCenter);

    statusLabel = new QLabel("Checking lab report availability...", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setWordWrap(true);
    statusLabel->setToolTip("Lab report availability status");

    statusRow->addWidget(statusIcon);
    statusRow->addWidget(statusLabel);

    // Back Button
    backButton = new QPushButton("Back", this);
    backButton->setToolTip("Return to the previous screen");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(backButton);

    // Add to content layout
    contentLayout->addWidget(titleLabel);
    contentLayout->addLayout(statusRow);
    contentLayout->addLayout(buttonLayout);

    mainLayout->addWidget(statusFrame);

    connect(backButton, &QPushButton::clicked, this, &QWidget::close);
}

void PatientDashboardLabReport::setupStyles()
{
    // Window and Frame Background
    setStyleSheet(
        "QWidget {"
        "   background-color: #ffffff;"
        "}"
        "QFrame {"
        "   background-color: #ffffff;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 10px;"
        "   box-shadow: 0 2px 6px rgba(0, 0, 0, 0.05);"
        "}"
        );

    // Title Styling
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #1a3c5e;"
        "   font-size: 20px;"
        "   font-weight: bold;"
        "   padding: 5px;"
        "}"
        );

    // Status Label Styling
    statusLabel->setStyleSheet(
        "QLabel {"
        "   color: #333333;"
        "   font-size: 16px;"
        "   font-weight: medium;"
        "   padding: 5px;"
        "}"
        "QLabel[accessibleName=\"available\"] {"
        "   color: #2e7d32;"
        "}"
        "QLabel[accessibleName=\"not-available\"] {"
        "   color: #d32f2f;"
        "}"
        );

    // Status Icon Styling
    statusIcon->setStyleSheet(
        "QLabel {"
        "   padding: 0px;"
        "}"
        "QLabel[accessibleName=\"available\"] {"
        "   background: url(:/icons/check.png) no-repeat center;"
        "}"
        "QLabel[accessibleName=\"not-available\"] {"
        "   background: url(:/icons/cross.png) no-repeat center;"
        "}"
        "QLabel[accessibleName=\"error\"] {"
        "   background: url(:/icons/error.png) no-repeat center;"
        "}"
        );

    // Back Button Styling
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

void PatientDashboardLabReport::fetchLabReportAvailability()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection failed.";
        statusLabel->setText("Error: Database connection failed");
        statusLabel->setAccessibleName("error");
        statusIcon->setAccessibleName("error");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM lab_reports WHERE patient_id = :patientId");
    query.bindValue(":patientId", patientId);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        statusLabel->setText("Error: Unable to fetch lab report availability");
        statusLabel->setAccessibleName("error");
        statusIcon->setAccessibleName("error");
        return;
    }

    if (query.next() && query.value(0).toInt() > 0) {
        statusLabel->setText("Reports are available");
        statusLabel->setAccessibleName("available");
        statusIcon->setAccessibleName("available");
    } else {
        statusLabel->setText("Reports are not available");
        statusLabel->setAccessibleName("not-available");
        statusIcon->setAccessibleName("not-available");
    }
}
