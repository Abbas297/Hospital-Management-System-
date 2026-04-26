#include "admindashboardpatient.h"
#include "admindashboard.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>

AdminDashboardPatient::AdminDashboardPatient(QWidget *parent, AdminDashboard *dashboardParent)
    : QWidget(parent), parentDashboard(dashboardParent)
{
    qDebug() << "AdminDashboardPatient constructor called";
    setWindowTitle("Manage Patients");
    setWindowFlags(Qt::Window);
    qDebug() << "Window properties set, calling setupUI";
    setupUI();
    qDebug() << "setupUI completed, calling setupStyles";
    setupStyles();
    qDebug() << "setupStyles completed, attempting to load data";

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    qDebug() << "Database connection status:" << (db.isOpen() ? "Open" : "Closed");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed. Functionality may be limited.", true);
        qDebug() << "Database connection failed in constructor";
    } else {
        loadPatients();
        qDebug() << "loadPatients completed";
        loadAppointments();
        qDebug() << "loadAppointments completed";
        loadLabReports();
        qDebug() << "loadLabReports completed";
        loadPayments();
        qDebug() << "loadPayments completed";
    }
    qDebug() << "AdminDashboardPatient initialization completed";

    showMaximized();
    raise();
    activateWindow();
    qDebug() << "AdminDashboardPatient window shown";
}

void AdminDashboardPatient::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Header with Title and Back Button
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(10);

    backBtn = new QPushButton("Back", this);
    backBtn->setFixedSize(80, 30);

    QLabel *titleLabel = new QLabel("Manage Patients", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Sans Serif", 24, QFont::Bold));

    headerLayout->addWidget(backBtn);
    headerLayout->addStretch();
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    // Status Label
    statusLabel = new QLabel("", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFont(QFont("Sans Serif", 12));
    mainLayout->addWidget(statusLabel);

    // Patient Table
    patientTable = new QTableWidget(this);
    patientTable->setColumnCount(5);
    patientTable->setHorizontalHeaderLabels({"ID", "Name", "Password", "Age", "Contact"});
    patientTable->setSelectionMode(QAbstractItemView::SingleSelection);
    patientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(patientTable);

    // Tab Widget
    QTabWidget *tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // Patients Tab
    QWidget *patientsTab = new QWidget();
    QVBoxLayout *patientsLayout = new QVBoxLayout(patientsTab);

    QFrame *patientFormFrame = new QFrame(patientsTab);
    patientFormFrame->setFrameShape(QFrame::StyledPanel);
    QGridLayout *patientFormLayout = new QGridLayout(patientFormFrame);
    patientFormLayout->setSpacing(5);

    idEdit = new QLineEdit(patientFormFrame);
    nameEdit = new QLineEdit(patientFormFrame);
    passwordEdit = new QLineEdit(patientFormFrame);
    ageEdit = new QLineEdit(patientFormFrame);
    contactEdit = new QLineEdit(patientFormFrame);

    patientFormLayout->addWidget(new QLabel("ID:"), 0, 0);
    patientFormLayout->addWidget(idEdit, 0, 1);
    patientFormLayout->addWidget(new QLabel("Name:"), 1, 0);
    patientFormLayout->addWidget(nameEdit, 1, 1);
    patientFormLayout->addWidget(new QLabel("Password:"), 2, 0);
    patientFormLayout->addWidget(passwordEdit, 2, 1);
    patientFormLayout->addWidget(new QLabel("Age:"), 3, 0);
    patientFormLayout->addWidget(ageEdit, 3, 1);
    patientFormLayout->addWidget(new QLabel("Contact:"), 4, 0);
    patientFormLayout->addWidget(contactEdit, 4, 1);

    QHBoxLayout *patientButtonLayout = new QHBoxLayout();
    addPatientBtn = new QPushButton("Add Patient", patientFormFrame);
    updatePatientBtn = new QPushButton("Update Patient", patientFormFrame);
    deletePatientBtn = new QPushButton("Delete Patient", patientFormFrame);
    patientButtonLayout->addWidget(addPatientBtn);
    patientButtonLayout->addWidget(updatePatientBtn);
    patientButtonLayout->addWidget(deletePatientBtn);

    patientsLayout->addWidget(patientFormFrame);
    patientsLayout->addLayout(patientButtonLayout);
    tabWidget->addTab(patientsTab, "Patients");

    // Appointments Tab
    QWidget *appointmentsTab = new QWidget();
    QVBoxLayout *appointmentsLayout = new QVBoxLayout(appointmentsTab);

    appointmentTable = new QTableWidget(appointmentsTab);
    appointmentTable->setColumnCount(5);
    appointmentTable->setHorizontalHeaderLabels({"ID", "Patient ID", "Doctor ID", "Date", "Status"});
    appointmentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    appointmentTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QFrame *appointmentFrame = new QFrame(appointmentsTab);
    appointmentFrame->setFrameShape(QFrame::StyledPanel);
    QGridLayout *appointmentFormLayout = new QGridLayout(appointmentFrame);
    appointmentStatusEdit = new QLineEdit(appointmentFrame);
    appointmentDateEdit = new QDateEdit(appointmentFrame);
    appointmentDateEdit->setCalendarPopup(true);
    appointmentDateEdit->setDate(QDate::currentDate());

    appointmentFormLayout->addWidget(new QLabel("Status:"), 0, 0);
    appointmentFormLayout->addWidget(appointmentStatusEdit, 0, 1);
    appointmentFormLayout->addWidget(new QLabel("Date:"), 1, 0);
    appointmentFormLayout->addWidget(appointmentDateEdit, 1, 1);

    QHBoxLayout *appointmentButtonLayout = new QHBoxLayout();
    addAppointmentBtn = new QPushButton("Add Appointment", appointmentFrame);
    updateAppointmentBtn = new QPushButton("Update Appointment", appointmentFrame);
    deleteAppointmentBtn = new QPushButton("Delete Appointment", appointmentFrame);
    appointmentButtonLayout->addWidget(addAppointmentBtn);
    appointmentButtonLayout->addWidget(updateAppointmentBtn);
    appointmentButtonLayout->addWidget(deleteAppointmentBtn);

    appointmentsLayout->addWidget(appointmentTable);
    appointmentsLayout->addWidget(appointmentFrame);
    appointmentsLayout->addLayout(appointmentButtonLayout);
    tabWidget->addTab(appointmentsTab, "Appointments");

    // Lab Reports Tab
    QWidget *labReportsTab = new QWidget();
    QVBoxLayout *labReportsLayout = new QVBoxLayout(labReportsTab);

    labReportTable = new QTableWidget(labReportsTab);
    labReportTable->setColumnCount(6);
    labReportTable->setHorizontalHeaderLabels({"ID", "Patient ID", "Doctor ID", "Test Name", "Result", "Status"});
    labReportTable->setSelectionMode(QAbstractItemView::SingleSelection);
    labReportTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QFrame *labFrame = new QFrame(labReportsTab);
    labFrame->setFrameShape(QFrame::StyledPanel);
    QHBoxLayout *labLayout = new QHBoxLayout(labFrame);
    labReportCombo = new QComboBox(labFrame);
    assignLabReportBtn = new QPushButton("Assign Lab Report", labFrame);
    labLayout->addWidget(new QLabel("Select Lab Report:"));
    labLayout->addWidget(labReportCombo);
    labLayout->addWidget(assignLabReportBtn);

    labReportsLayout->addWidget(labReportTable);
    labReportsLayout->addWidget(labFrame);
    tabWidget->addTab(labReportsTab, "Lab Reports");

    // Payments Tab
    QWidget *paymentsTab = new QWidget();
    QVBoxLayout *paymentsLayout = new QVBoxLayout(paymentsTab);

    paymentTable = new QTableWidget(paymentsTab);
    paymentTable->setColumnCount(5);
    paymentTable->setHorizontalHeaderLabels({"ID", "Patient ID", "Amount", "Date", "Status"});
    paymentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    paymentTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QFrame *paymentFrame = new QFrame(paymentsTab);
    paymentFrame->setFrameShape(QFrame::StyledPanel);
    QGridLayout *paymentFormLayout = new QGridLayout(paymentFrame);
    paymentAmountEdit = new QLineEdit(paymentFrame);
    paymentDateEdit = new QDateEdit(paymentFrame);
    paymentDateEdit->setCalendarPopup(true);
    paymentDateEdit->setDate(QDate::currentDate());
    paymentStatusEdit = new QLineEdit(paymentFrame);

    paymentFormLayout->addWidget(new QLabel("Amount:"), 0, 0);
    paymentFormLayout->addWidget(paymentAmountEdit, 0, 1);
    paymentFormLayout->addWidget(new QLabel("Date:"), 1, 0);
    paymentFormLayout->addWidget(paymentDateEdit, 1, 1);
    paymentFormLayout->addWidget(new QLabel("Status:"), 2, 0);
    paymentFormLayout->addWidget(paymentStatusEdit, 2, 1);

    QHBoxLayout *paymentButtonLayout = new QHBoxLayout();
    addPaymentBtn = new QPushButton("Add Payment", paymentFrame);
    updatePaymentBtn = new QPushButton("Update Payment", paymentFrame);
    deletePaymentBtn = new QPushButton("Delete Payment", paymentFrame);
    paymentButtonLayout->addWidget(addPaymentBtn);
    paymentButtonLayout->addWidget(updatePaymentBtn);
    paymentButtonLayout->addWidget(deletePaymentBtn);

    paymentsLayout->addWidget(paymentTable);
    paymentsLayout->addWidget(paymentFrame);
    paymentsLayout->addLayout(paymentButtonLayout);
    tabWidget->addTab(paymentsTab, "Payments");

    // Connections
    connect(addPatientBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleAddPatient);
    connect(updatePatientBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleUpdatePatient);
    connect(deletePatientBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleDeletePatient);
    connect(addAppointmentBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleAddAppointment);
    connect(updateAppointmentBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleUpdateAppointment);
    connect(deleteAppointmentBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleDeleteAppointment);
    connect(addPaymentBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleAddPayment);
    connect(updatePaymentBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleUpdatePayment);
    connect(deletePaymentBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleDeletePayment);
    connect(assignLabReportBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleAssignLabReport);
    if (parentDashboard) {
        connect(backBtn, &QPushButton::clicked, this, &AdminDashboardPatient::handleBack);
    }
}

void AdminDashboardPatient::setupStyles()
{
    setStyleSheet("background-color: #ffffff;");

    statusLabel->setStyleSheet(
        "QLabel {"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "}"
        );

    patientTable->setStyleSheet(
        "QTableWidget { border: 1px solid #1e90ff; background-color: #ffffff; }"
        "QHeaderView::section { background-color: #1e90ff; color: #ffffff; }"
        );
    appointmentTable->setStyleSheet(
        "QTableWidget { border: 1px solid #1e90ff; background-color: #ffffff; }"
        "QHeaderView::section { background-color: #1e90ff; color: #ffffff; }"
        );
    labReportTable->setStyleSheet(
        "QTableWidget { border: 1px solid #1e90ff; background-color: #ffffff; }"
        "QHeaderView::section { background-color: #1e90ff; color: #ffffff; }"
        );
    paymentTable->setStyleSheet(
        "QTableWidget { border: 1px solid #1e90ff; background-color: #ffffff; }"
        "QHeaderView::section { background-color: #1e90ff; color: #ffffff; }"
        );

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #1e90ff;"
        "   color: #ffffff;"
        "   border: none;"
        "   border-radius: 8px;"
        "   padding: 8px 15px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1565c0;"
        "}";
    addPatientBtn->setStyleSheet(buttonStyle);
    updatePatientBtn->setStyleSheet(buttonStyle);
    deletePatientBtn->setStyleSheet(buttonStyle);
    addAppointmentBtn->setStyleSheet(buttonStyle);
    updateAppointmentBtn->setStyleSheet(buttonStyle);
    deleteAppointmentBtn->setStyleSheet(buttonStyle);
    addPaymentBtn->setStyleSheet(buttonStyle);
    updatePaymentBtn->setStyleSheet(buttonStyle);
    deletePaymentBtn->setStyleSheet(buttonStyle);
    assignLabReportBtn->setStyleSheet(buttonStyle);
    backBtn->setStyleSheet(buttonStyle);
}

void AdminDashboardPatient::showStatusMessage(const QString &message, bool isError)
{
    statusLabel->setText(message);
    statusLabel->setStyleSheet(
        QString("QLabel {"
                "   padding: 8px;"
                "   border-radius: 5px;"
                "   background-color: %1;"
                "   color: #ffffff;"
                "}").arg(isError ? "#e53e3e" : "#2f855a")
        );
    QTimer::singleShot(5000, statusLabel, [this]() {
        statusLabel->setText("");
        statusLabel->setStyleSheet(
            "QLabel {"
            "   padding: 8px;"
            "   border-radius: 5px;"
            "}"
            );
    });
}

void AdminDashboardPatient::loadPatients()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    qDebug() << "Loading patients, database open:" << db.isOpen();
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        qDebug() << "Database connection failed in loadPatients";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, name, password, age, contact FROM patients")) {
        showStatusMessage("Failed to load patients: " + query.lastError().text(), true);
        qDebug() << "Failed to load patients:" << query.lastError().text();
        return;
    }

    patientTable->setRowCount(0);
    while (query.next()) {
        int row = patientTable->rowCount();
        patientTable->insertRow(row);
        patientTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        patientTable->setItem(row, 1, new QTableWidgetItem(query.value("name").toString()));
        patientTable->setItem(row, 2, new QTableWidgetItem(query.value("password").toString()));
        patientTable->setItem(row, 3, new QTableWidgetItem(query.value("age").toString()));
        patientTable->setItem(row, 4, new QTableWidgetItem(query.value("contact").toString()));
    }
    patientTable->resizeColumnsToContents();
}

void AdminDashboardPatient::loadAppointments()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    qDebug() << "Loading appointments, database open:" << db.isOpen();
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        qDebug() << "Database connection failed in loadAppointments";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, patient_id, doctor_id, appointment_date, status FROM appointments")) {
        showStatusMessage("Failed to load appointments: " + query.lastError().text(), true);
        qDebug() << "Failed to load appointments:" << query.lastError().text();
        return;
    }

    appointmentTable->setRowCount(0);
    while (query.next()) {
        int row = appointmentTable->rowCount();
        appointmentTable->insertRow(row);
        appointmentTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        appointmentTable->setItem(row, 1, new QTableWidgetItem(query.value("patient_id").toString()));
        appointmentTable->setItem(row, 2, new QTableWidgetItem(query.value("doctor_id").toString()));
        appointmentTable->setItem(row, 3, new QTableWidgetItem(query.value("appointment_date").toString()));
        appointmentTable->setItem(row, 4, new QTableWidgetItem(query.value("status").toString()));
    }
    appointmentTable->resizeColumnsToContents();
}

void AdminDashboardPatient::loadLabReports()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    qDebug() << "Loading lab reports, database open:" << db.isOpen();
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        qDebug() << "Database connection failed in loadLabReports";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, patient_id, doctor_id, test_name, result, status FROM lab_reports")) {
        showStatusMessage("Failed to load lab reports: " + query.lastError().text(), true);
        qDebug() << "Failed to load lab reports:" << query.lastError().text();
        return;
    }

    labReportTable->setRowCount(0);
    labReportCombo->clear();
    while (query.next()) {
        int row = labReportTable->rowCount();
        labReportTable->insertRow(row);
        labReportTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        labReportTable->setItem(row, 1, new QTableWidgetItem(query.value("patient_id").toString()));
        labReportTable->setItem(row, 2, new QTableWidgetItem(query.value("doctor_id").toString()));
        labReportTable->setItem(row, 3, new QTableWidgetItem(query.value("test_name").toString()));
        labReportTable->setItem(row, 4, new QTableWidgetItem(query.value("result").toString()));
        labReportTable->setItem(row, 5, new QTableWidgetItem(query.value("status").toString()));
        labReportCombo->addItem(QString("Report %1: %2").arg(query.value("id").toString(), query.value("test_name").toString()), query.value("id"));
    }
    labReportTable->resizeColumnsToContents();
}

void AdminDashboardPatient::loadPayments()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    qDebug() << "Loading payments, database open:" << db.isOpen();
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        qDebug() << "Database connection failed in loadPayments";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, patient_id, amount, payment_date, status FROM payments")) {
        showStatusMessage("Failed to load payments: " + query.lastError().text(), true);
        qDebug() << "Failed to load payments:" << query.lastError().text();
        return;
    }

    paymentTable->setRowCount(0);
    while (query.next()) {
        int row = paymentTable->rowCount();
        paymentTable->insertRow(row);
        paymentTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        paymentTable->setItem(row, 1, new QTableWidgetItem(query.value("patient_id").toString()));
        paymentTable->setItem(row, 2, new QTableWidgetItem(query.value("amount").toString()));
        paymentTable->setItem(row, 3, new QTableWidgetItem(query.value("payment_date").toString()));
        paymentTable->setItem(row, 4, new QTableWidgetItem(query.value("status").toString()));
    }
    paymentTable->resizeColumnsToContents();
}

void AdminDashboardPatient::handleAddPatient()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO patients (id, name, password, age, contact, disease, admitDate) VALUES (:id, :name, :password, :age, :contact, :disease, :admitDate)");
    query.bindValue(":id", idEdit->text());
    query.bindValue(":name", nameEdit->text());
    query.bindValue(":password", passwordEdit->text());
    query.bindValue(":age", ageEdit->text().toInt());
    query.bindValue(":contact", contactEdit->text());
    query.bindValue(":disease", ""); // Default empty as UI doesn't provide this
    query.bindValue(":admitDate", QDate::currentDate().toString("yyyy-MM-dd")); // Default to today

    if (!query.exec()) {
        showStatusMessage("Failed to add patient: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Patient added successfully.");
    loadPatients();
    idEdit->clear();
    nameEdit->clear();
    passwordEdit->clear();
    ageEdit->clear();
    contactEdit->clear();
}

void AdminDashboardPatient::handleUpdatePatient()
{
    if (patientTable->currentRow() < 0) {
        showStatusMessage("Please select a patient to update.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE patients SET name=:name, password=:password, age=:age, contact=:contact WHERE id=:id");
    query.bindValue(":id", idEdit->text());
    query.bindValue(":name", nameEdit->text());
    query.bindValue(":password", passwordEdit->text());
    query.bindValue(":age", ageEdit->text().toInt());
    query.bindValue(":contact", contactEdit->text());

    if (!query.exec()) {
        showStatusMessage("Failed to update patient: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Patient updated successfully.");
    loadPatients();
}

void AdminDashboardPatient::handleDeletePatient()
{
    if (patientTable->currentRow() < 0) {
        showStatusMessage("Please select a patient to delete.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString id = patientTable->item(patientTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    query.prepare("DELETE FROM patients WHERE id=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        showStatusMessage("Failed to delete patient: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Patient deleted successfully.");
    loadPatients();
}

void AdminDashboardPatient::handleAddAppointment()
{
    if (patientTable->currentRow() < 0) {
        showStatusMessage("Please select a patient from the patient list above.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString patientId = patientTable->item(patientTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    QString appointmentId = QString("appt%1").arg(QDateTime::currentMSecsSinceEpoch()); // Generate unique ID
    query.prepare("INSERT INTO appointments (id, patient_id, appointment_date, status) VALUES (:id, :patient_id, :appointment_date, :status)");
    query.bindValue(":id", appointmentId);
    query.bindValue(":patient_id", patientId);
    query.bindValue(":appointment_date", appointmentDateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":status", appointmentStatusEdit->text());

    if (!query.exec()) {
        showStatusMessage("Failed to add appointment: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Appointment added successfully.");
    loadAppointments();
    appointmentStatusEdit->clear();
}

void AdminDashboardPatient::handleUpdateAppointment()
{
    if (appointmentTable->currentRow() < 0) {
        showStatusMessage("Please select an appointment to update.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString id = appointmentTable->item(appointmentTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    query.prepare("UPDATE appointments SET appointment_date=:appointment_date, status=:status WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":appointment_date", appointmentDateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":status", appointmentStatusEdit->text());

    if (!query.exec()) {
        showStatusMessage("Failed to update appointment: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Appointment updated successfully.");
    loadAppointments();
}

void AdminDashboardPatient::handleDeleteAppointment()
{
    if (appointmentTable->currentRow() < 0) {
        showStatusMessage("Please select an appointment to delete.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString id = appointmentTable->item(appointmentTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    query.prepare("DELETE FROM appointments WHERE id=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        showStatusMessage("Failed to delete appointment: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Appointment deleted successfully.");
    loadAppointments();
}

void AdminDashboardPatient::handleAddPayment()
{
    if (patientTable->currentRow() < 0) {
        showStatusMessage("Please select a patient from the patient list above.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString patientId = patientTable->item(patientTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    QString paymentId = QString("pay%1").arg(QDateTime::currentMSecsSinceEpoch()); // Generate unique ID
    query.prepare("INSERT INTO payments (id, patient_id, amount, payment_date, status) VALUES (:id, :patient_id, :amount, :payment_date, :status)");
    query.bindValue(":id", paymentId);
    query.bindValue(":patient_id", patientId);
    query.bindValue(":amount", paymentAmountEdit->text().toDouble());
    query.bindValue(":payment_date", paymentDateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":status", paymentStatusEdit->text());

    if (!query.exec()) {
        showStatusMessage("Failed to add payment: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Payment added successfully.");
    loadPayments();
    paymentAmountEdit->clear();
    paymentStatusEdit->clear();
}

void AdminDashboardPatient::handleUpdatePayment()
{
    if (paymentTable->currentRow() < 0) {
        showStatusMessage("Please select a payment to update.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString id = paymentTable->item(paymentTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    query.prepare("UPDATE payments SET amount=:amount, payment_date=:payment_date, status=:status WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":amount", paymentAmountEdit->text().toDouble());
    query.bindValue(":payment_date", paymentDateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":status", paymentStatusEdit->text());

    if (!query.exec()) {
        showStatusMessage("Failed to update payment: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Payment updated successfully.");
    loadPayments();
}

void AdminDashboardPatient::handleDeletePayment()
{
    if (paymentTable->currentRow() < 0) {
        showStatusMessage("Please select a payment to delete.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString id = paymentTable->item(paymentTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    query.prepare("DELETE FROM payments WHERE id=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        showStatusMessage("Failed to delete payment: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Payment deleted successfully.");
    loadPayments();
}

void AdminDashboardPatient::handleAssignLabReport()
{
    if (patientTable->currentRow() < 0) {
        showStatusMessage("Please select a patient from the patient list above.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString patientId = patientTable->item(patientTable->currentRow(), 0)->text();
    int reportId = labReportCombo->currentData().toInt();
    QSqlQuery query(db);
    query.prepare("UPDATE lab_reports SET patient_id=:patient_id, status='Assigned' WHERE id=:id");
    query.bindValue(":patient_id", patientId);
    query.bindValue(":id", reportId);

    if (!query.exec()) {
        showStatusMessage("Failed to assign lab report: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Lab report assigned successfully.");
    loadLabReports();
}

void AdminDashboardPatient::handleBack()
{
    if (parentDashboard) {
        parentDashboard->showMaximized();
        close();
    }
}

AdminDashboardPatient::~AdminDashboardPatient()
{
}
