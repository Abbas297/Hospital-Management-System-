#include "doctorreportsdialog.h"
#include <QDebug>
#include <QDate>

DoctorReportsDialog::DoctorReportsDialog(const QString &doctorId, QWidget *parent)
    : QDialog(parent), m_doctorId(doctorId.trimmed())
{
    qDebug() << "DoctorReportsDialog constructor called with doctorId:" << m_doctorId;
    if (m_doctorId.isEmpty()) {
        qCritical() << "ERROR: DoctorReportsDialog received an empty doctorId!";
    }
    setupUi();
    checkReports();
}

DoctorReportsDialog::~DoctorReportsDialog()
{
    qDebug() << "DoctorReportsDialog destroyed";
}

void DoctorReportsDialog::setupUi()
{
    setWindowTitle("Lab Reports Status");
    resize(400, 200);
    setStyleSheet("background-color: #f0f2f5;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    headerLabel = new QLabel("Lab Reports Status", this);
    headerLabel->setStyleSheet(
        "font-family: 'Sans Serif'; font-size: 20px; font-weight: bold; color: #1a3c5e; padding: 10px;"
        );
    headerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(headerLabel);

    statusLabel = new QLabel("", this);
    statusLabel->setStyleSheet(
        "font-family: 'Sans Serif'; font-size: 14px; color: #000000; padding: 10px; background: #ffffff; border: 1px solid #1e90ff; border-radius: 5px;"
        );
    statusLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(statusLabel);

    closeButton = new QPushButton("Close", this);
    closeButton->setStyleSheet(
        "QPushButton { background: #e53e3e; color: white; border: none; border-radius: 5px; padding: 10px; }"
        "QPushButton:hover { background: #c53030; }"
        "QPushButton:pressed { background: #b71c1c; }"
        );
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton);

    setLayout(mainLayout);
}

void DoctorReportsDialog::checkReports()
{
    qDebug() << "Checking lab reports for doctor ID:" << m_doctorId;

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isValid()) {
        qCritical() << "Database connection 'hospitalConnection' is invalid.";
        statusLabel->setText("Error: Database connection not found.");
        return;
    }

    if (!db.isOpen()) {
        if (!db.open()) {
            qCritical() << "Failed to open database:" << db.lastError().text();
            statusLabel->setText("Error: Unable to connect to the database.");
            return;
        }
    }

    if (m_doctorId.isEmpty()) {
        qCritical() << "ERROR: Empty doctor ID provided!";
        statusLabel->setText("Error: Invalid doctor ID.");
        return;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT COUNT(*) "
        "FROM lab_reports "
        "WHERE doctor_id = :doctorId AND status = 'Completed'"
        );
    query.bindValue(":doctorId", m_doctorId);

    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
        statusLabel->setText("Error: Failed to check reports.");
        return;
    }

    if (query.next()) {
        int reportCount = query.value(0).toInt();
        qDebug() << "Found" << reportCount << "completed lab reports for doctor:" << m_doctorId;
        if (reportCount > 0) {
            statusLabel->setText("Reports are generated and sent to your email.");
        } else {
            statusLabel->setText("Reports are in process.");
        }
    } else {
        qWarning() << "No result from lab report query";
        statusLabel->setText("Error: Unable to retrieve report status.");
    }
}
