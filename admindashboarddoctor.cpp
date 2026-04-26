#include "admindashboarddoctor.h"
#include "admindashboard.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>

AdminDashboardDoctor::AdminDashboardDoctor(QWidget *parent, AdminDashboard *dashboardParent)
    : QWidget(parent), parentDashboard(dashboardParent)
{
    qDebug() << "AdminDashboardDoctor constructor called";
    setWindowTitle("Manage Doctors");
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
        loadDoctors();
        qDebug() << "loadDoctors completed";
        loadLabReports();
        qDebug() << "loadLabReports completed";
        loadTasks();
        qDebug() << "loadTasks completed";
    }
    qDebug() << "AdminDashboardDoctor initialization completed";

    showMaximized(); // Open the window maximized
    raise();
    activateWindow();
    qDebug() << "AdminDashboardDoctor window shown";
}

void AdminDashboardDoctor::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20); // Increased margins for better spacing

    // Header with Title and Back Button
    QHBoxLayout *headerLayout = new QHBoxLayout();
    headerLayout->setSpacing(10);

    // Back Button on the left (smaller size)
    backBtn = new QPushButton("Back", this);
    backBtn->setFixedSize(80, 30); // Smaller button size

    // Title in the center
    QLabel *titleLabel = new QLabel("Manage Doctors", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(QFont("Sans Serif", 24, QFont::Bold)); // Larger and bold font for elegance

    // Add stretch on both sides to center the title
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

    // Doctor Table
    doctorTable = new QTableWidget(this);
    doctorTable->setColumnCount(5);
    doctorTable->setHorizontalHeaderLabels({"ID", "Name", "Password", "Specialization", "Age"});
    doctorTable->setSelectionMode(QAbstractItemView::SingleSelection);
    doctorTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mainLayout->addWidget(doctorTable);

    // Tab Widget
    QTabWidget *tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // Doctors Tab
    QWidget *doctorsTab = new QWidget();
    QVBoxLayout *doctorsLayout = new QVBoxLayout(doctorsTab);

    QFrame *doctorFormFrame = new QFrame(doctorsTab);
    doctorFormFrame->setFrameShape(QFrame::StyledPanel);
    QGridLayout *doctorFormLayout = new QGridLayout(doctorFormFrame);
    doctorFormLayout->setSpacing(5);

    idEdit = new QLineEdit(doctorFormFrame);
    nameEdit = new QLineEdit(doctorFormFrame);
    passwordEdit = new QLineEdit(doctorFormFrame);
    specializationEdit = new QLineEdit(doctorFormFrame);
    ageEdit = new QLineEdit(doctorFormFrame);

    doctorFormLayout->addWidget(new QLabel("ID:"), 0, 0);
    doctorFormLayout->addWidget(idEdit, 0, 1);
    doctorFormLayout->addWidget(new QLabel("Name:"), 1, 0);
    doctorFormLayout->addWidget(nameEdit, 1, 1);
    doctorFormLayout->addWidget(new QLabel("Password:"), 2, 0);
    doctorFormLayout->addWidget(passwordEdit, 2, 1);
    doctorFormLayout->addWidget(new QLabel("Specialization:"), 3, 0);
    doctorFormLayout->addWidget(specializationEdit, 3, 1);
    doctorFormLayout->addWidget(new QLabel("Age:"), 4, 0);
    doctorFormLayout->addWidget(ageEdit, 4, 1);

    QHBoxLayout *doctorButtonLayout = new QHBoxLayout();
    addDoctorBtn = new QPushButton("Add Doctor", doctorFormFrame);
    updateDoctorBtn = new QPushButton("Update Doctor", doctorFormFrame);
    deleteDoctorBtn = new QPushButton("Delete Doctor", doctorFormFrame);
    doctorButtonLayout->addWidget(addDoctorBtn);
    doctorButtonLayout->addWidget(updateDoctorBtn);
    doctorButtonLayout->addWidget(deleteDoctorBtn);

    doctorsLayout->addWidget(doctorFormFrame);
    doctorsLayout->addLayout(doctorButtonLayout);
    tabWidget->addTab(doctorsTab, "Doctors");

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

    // Tasks Tab
    QWidget *tasksTab = new QWidget();
    QVBoxLayout *tasksLayout = new QVBoxLayout(tasksTab);

    taskTable = new QTableWidget(tasksTab);
    taskTable->setColumnCount(5);
    taskTable->setHorizontalHeaderLabels({"ID", "Description", "Assigned Date", "Due Date", "Status"});
    taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    QFrame *taskFrame = new QFrame(tasksTab);
    taskFrame->setFrameShape(QFrame::StyledPanel);
    QGridLayout *taskLayout = new QGridLayout(taskFrame);
    taskDescriptionEdit = new QLineEdit(taskFrame);
    dueDateEdit = new QDateEdit(taskFrame);
    dueDateEdit->setCalendarPopup(true);
    dueDateEdit->setDate(QDate::currentDate());
    assignTaskBtn = new QPushButton("Assign Task", taskFrame);
    taskLayout->addWidget(new QLabel("Task Description:"), 0, 0);
    taskLayout->addWidget(taskDescriptionEdit, 0, 1);
    taskLayout->addWidget(new QLabel("Due Date:"), 1, 0);
    taskLayout->addWidget(dueDateEdit, 1, 1);
    taskLayout->addWidget(assignTaskBtn, 2, 1);

    tasksLayout->addWidget(taskTable);
    tasksLayout->addWidget(taskFrame);
    tabWidget->addTab(tasksTab, "Tasks");

    // Connections
    connect(addDoctorBtn, &QPushButton::clicked, this, &AdminDashboardDoctor::handleAddDoctor);
    connect(updateDoctorBtn, &QPushButton::clicked, this, &AdminDashboardDoctor::handleUpdateDoctor);
    connect(deleteDoctorBtn, &QPushButton::clicked, this, &AdminDashboardDoctor::handleDeleteDoctor);
    connect(assignTaskBtn, &QPushButton::clicked, this, &AdminDashboardDoctor::handleAssignTask);
    connect(assignLabReportBtn, &QPushButton::clicked, this, &AdminDashboardDoctor::handleAssignLabReport);
    if (parentDashboard) {
        connect(backBtn, &QPushButton::clicked, this, &AdminDashboardDoctor::handleBack);
    }
}

void AdminDashboardDoctor::setupStyles()
{
    setStyleSheet("background-color: #ffffff;");

    statusLabel->setStyleSheet(
        "QLabel {"
        "   padding: 8px;"
        "   border-radius: 5px;"
        "}"
        );

    doctorTable->setStyleSheet(
        "QTableWidget { border: 1px solid #1e90ff; background-color: #ffffff; }"
        "QHeaderView::section { background-color: #1e90ff; color: #ffffff; }"
        );
    labReportTable->setStyleSheet(
        "QTableWidget { border: 1px solid #1e90ff; background-color: #ffffff; }"
        "QHeaderView::section { background-color: #1e90ff; color: #ffffff; }"
        );
    taskTable->setStyleSheet(
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
    addDoctorBtn->setStyleSheet(buttonStyle);
    updateDoctorBtn->setStyleSheet(buttonStyle);
    deleteDoctorBtn->setStyleSheet(buttonStyle);
    assignTaskBtn->setStyleSheet(buttonStyle);
    assignLabReportBtn->setStyleSheet(buttonStyle);
    backBtn->setStyleSheet(buttonStyle);
}

void AdminDashboardDoctor::showStatusMessage(const QString &message, bool isError)
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

void AdminDashboardDoctor::loadDoctors()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    qDebug() << "Loading doctors, database open:" << db.isOpen();
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        qDebug() << "Database connection failed in loadDoctors";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, name, password, specialization, age FROM doctors")) {
        showStatusMessage("Failed to load doctors: " + query.lastError().text(), true);
        qDebug() << "Failed to load doctors:" << query.lastError().text();
        return;
    }

    doctorTable->setRowCount(0);
    while (query.next()) {
        int row = doctorTable->rowCount();
        doctorTable->insertRow(row);
        doctorTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        doctorTable->setItem(row, 1, new QTableWidgetItem(query.value("name").toString()));
        doctorTable->setItem(row, 2, new QTableWidgetItem(query.value("password").toString()));
        doctorTable->setItem(row, 3, new QTableWidgetItem(query.value("specialization").toString()));
        doctorTable->setItem(row, 4, new QTableWidgetItem(query.value("age").toString()));
    }
    doctorTable->resizeColumnsToContents();
}

void AdminDashboardDoctor::loadLabReports()
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

void AdminDashboardDoctor::loadTasks()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    qDebug() << "Loading tasks, database open:" << db.isOpen();
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        qDebug() << "Database connection failed in loadTasks";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, task_description, assigned_date, due_date, status FROM tasks WHERE assignee_type='doctor'")) {
        showStatusMessage("Failed to load tasks: " + query.lastError().text(), true);
        qDebug() << "Failed to load tasks:" << query.lastError().text();
        return;
    }

    taskTable->setRowCount(0);
    while (query.next()) {
        int row = taskTable->rowCount();
        taskTable->insertRow(row);
        taskTable->setItem(row, 0, new QTableWidgetItem(query.value("id").toString()));
        taskTable->setItem(row, 1, new QTableWidgetItem(query.value("task_description").toString()));
        taskTable->setItem(row, 2, new QTableWidgetItem(query.value("assigned_date").toString()));
        taskTable->setItem(row, 3, new QTableWidgetItem(query.value("due_date").toString()));
        taskTable->setItem(row, 4, new QTableWidgetItem(query.value("status").toString()));
    }
    taskTable->resizeColumnsToContents();
}

void AdminDashboardDoctor::handleAddDoctor()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO doctors (id, name, password, specialization, age) VALUES (:id, :name, :password, :specialization, :age)");
    query.bindValue(":id", idEdit->text());
    query.bindValue(":name", nameEdit->text());
    query.bindValue(":password", passwordEdit->text());
    query.bindValue(":specialization", specializationEdit->text());
    query.bindValue(":age", ageEdit->text().toInt());

    if (!query.exec()) {
        showStatusMessage("Failed to add doctor: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Doctor added successfully.");
    loadDoctors();
    idEdit->clear();
    nameEdit->clear();
    passwordEdit->clear();
    specializationEdit->clear();
    ageEdit->clear();
}

void AdminDashboardDoctor::handleUpdateDoctor()
{
    if (doctorTable->currentRow() < 0) {
        showStatusMessage("Please select a doctor to update.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE doctors SET name=:name, password=:password, specialization=:specialization, age=:age WHERE id=:id");
    query.bindValue(":id", idEdit->text());
    query.bindValue(":name", nameEdit->text());
    query.bindValue(":password", passwordEdit->text());
    query.bindValue(":specialization", specializationEdit->text());
    query.bindValue(":age", ageEdit->text().toInt());

    if (!query.exec()) {
        showStatusMessage("Failed to update doctor: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Doctor updated successfully.");
    loadDoctors();
}

void AdminDashboardDoctor::handleDeleteDoctor()
{
    if (doctorTable->currentRow() < 0) {
        showStatusMessage("Please select a doctor to delete.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString id = doctorTable->item(doctorTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    query.prepare("DELETE FROM doctors WHERE id=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        showStatusMessage("Failed to delete doctor: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Doctor deleted successfully.");
    loadDoctors();
}

void AdminDashboardDoctor::handleAssignTask()
{
    if (doctorTable->currentRow() < 0) {
        showStatusMessage("Please select a doctor from the doctor list above.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString doctorId = doctorTable->item(doctorTable->currentRow(), 0)->text();
    QSqlQuery query(db);
    query.prepare("INSERT INTO tasks (assignee_id, assignee_type, task_description, assigned_date, due_date, status) "
                  "VALUES (:assignee_id, :assignee_type, :task_description, :assigned_date, :due_date, :status)");
    query.bindValue(":assignee_id", doctorId);
    query.bindValue(":assignee_type", "doctor");
    query.bindValue(":task_description", taskDescriptionEdit->text());
    query.bindValue(":assigned_date", QDate::currentDate().toString("yyyy-MM-dd"));
    query.bindValue(":due_date", dueDateEdit->date().toString("yyyy-MM-dd"));
    query.bindValue(":status", "Pending");

    if (!query.exec()) {
        showStatusMessage("Failed to assign task: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Task assigned successfully.");
    loadTasks();
    taskDescriptionEdit->clear();
}

void AdminDashboardDoctor::handleAssignLabReport()
{
    if (doctorTable->currentRow() < 0) {
        showStatusMessage("Please select a doctor from the doctor list above.", true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        showStatusMessage("Database connection failed.", true);
        return;
    }

    QString doctorId = doctorTable->item(doctorTable->currentRow(), 0)->text();
    int reportId = labReportCombo->currentData().toInt();
    QSqlQuery query(db);
    query.prepare("UPDATE lab_reports SET doctor_id=:doctor_id, status='Assigned' WHERE id=:id");
    query.bindValue(":doctor_id", doctorId);
    query.bindValue(":id", reportId);

    if (!query.exec()) {
        showStatusMessage("Failed to assign lab report: " + query.lastError().text(), true);
        return;
    }

    showStatusMessage("Lab report assigned successfully.");
    loadLabReports();
}

void AdminDashboardDoctor::handleBack()
{
    if (parentDashboard) {
        parentDashboard->showMaximized();
        close();
    }
}

AdminDashboardDoctor::~AdminDashboardDoctor()
{
}
