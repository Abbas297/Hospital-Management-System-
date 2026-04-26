#include "doctordashboardsalary.h"
#include <QDebug>
#include <QHeaderview>

DoctorDashboardSalary::DoctorDashboardSalary(const QString &doctorId, QWidget *parent)
    : QDialog(parent), m_doctorId(doctorId.trimmed())
{
    qDebug() << "DoctorDashboardSalary constructor called with doctorId:" << m_doctorId;
    if (m_doctorId.isEmpty()) {
        qCritical() << "ERROR: DoctorDashboardSalary received an empty doctorId!";
    }
    setupUi();
    fetchSalaryDetails();
}

DoctorDashboardSalary::~DoctorDashboardSalary()
{
    qDebug() << "DoctorDashboardSalary destroyed";
}

void DoctorDashboardSalary::setupUi()
{
    setWindowTitle("Doctor Salary Details");
    resize(600, 400);
    setStyleSheet("background-color: #f0f2f5;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    headerLabel = new QLabel("Salary Details", this);
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
    statusLabel->setVisible(false); // Hidden by default, shown only if no data
    mainLayout->addWidget(statusLabel);

    salaryTable = new QTableWidget(this);
    salaryTable->setColumnCount(4);
    salaryTable->setHorizontalHeaderLabels({"Salary", "Date", "Overtime Salary", "Reduction Due to Offs"});
    salaryTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: #ffffff;"
        "   border: 1px solid #1e90ff;"
        "   border-radius: 5px;"
        "   font-family: 'Sans Serif';"
        "   font-size: 14px;"
        "}"
        "QHeaderView::section {"
        "   background-color: #1e90ff;"
        "   color: white;"
        "   padding: 5px;"
        "   font-weight: bold;"
        "}"
        );
    salaryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    salaryTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Make table read-only
    mainLayout->addWidget(salaryTable);

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

void DoctorDashboardSalary::fetchSalaryDetails()
{
    qDebug() << "Fetching salary details for doctor ID:" << m_doctorId;

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isValid()) {
        qCritical() << "Database connection 'hospitalConnection' is invalid.";
        statusLabel->setText("Error: Database connection not found.");
        statusLabel->setVisible(true);
        salaryTable->setVisible(false);
        return;
    }

    if (!db.isOpen()) {
        if (!db.open()) {
            qCritical() << "Failed to open database:" << db.lastError().text();
            statusLabel->setText("Error: Unable to connect to the database.");
            statusLabel->setVisible(true);
            salaryTable->setVisible(false);
            return;
        }
    }

    if (m_doctorId.isEmpty()) {
        qCritical() << "ERROR: Empty doctor ID provided!";
        statusLabel->setText("Error: Invalid doctor ID.");
        statusLabel->setVisible(true);
        salaryTable->setVisible(false);
        return;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT salary, payment_date, overtime_salary, salary_reduction "
        "FROM doctor_salaries "
        "WHERE doctor_id = :doctorId"
        );
    query.bindValue(":doctorId", m_doctorId);

    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
        statusLabel->setText("Error: Failed to fetch salary details.");
        statusLabel->setVisible(true);
        salaryTable->setVisible(false);
        return;
    }

    if (!query.next()) {
        qDebug() << "No salary records found for doctor:" << m_doctorId;
        statusLabel->setText("Please wait, we will get back to you.");
        statusLabel->setVisible(true);
        salaryTable->setVisible(false);
        return;
    }

    // Populate the table
    salaryTable->setRowCount(0); // Clear any existing rows
    do {
        int row = salaryTable->rowCount();
        salaryTable->insertRow(row);

        salaryTable->setItem(row, 0, new QTableWidgetItem(QString::number(query.value("salary").toDouble())));
        salaryTable->setItem(row, 1, new QTableWidgetItem(query.value("payment_date").toString()));
        salaryTable->setItem(row, 2, new QTableWidgetItem(QString::number(query.value("overtime_salary").toDouble())));
        salaryTable->setItem(row, 3, new QTableWidgetItem(QString::number(query.value("salary_reduction").toDouble())));
    } while (query.next());

    salaryTable->setVisible(true);
    statusLabel->setVisible(false);
}
