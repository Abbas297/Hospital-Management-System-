#include "doctordashboardtodayswork.h"
#include <QHeaderView>
#include <QDateTime>
#include <QTextEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QDate>
#include <QTabWidget>

PatientHistoryDialog::PatientHistoryDialog(const QString &patientId, QWidget *parent)
    : QDialog(parent)
{
    setupUi(patientId);
}

void PatientHistoryDialog::setupUi(const QString &patientId)
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    QString patientName = "Unknown";
    QString historyText;

    if (!db.isOpen()) {
        if (!db.open()) {
            qCritical() << "Failed to open database:" << db.lastError().text();
            historyText = "Error: Unable to connect to the database.";
        }
    }

    if (db.isOpen()) {
        QSqlQuery query(db);
        query.prepare("SELECT name, age, disease FROM patients WHERE id = :patientId");
        query.bindValue(":patientId", patientId);

        if (query.exec() && query.next()) {
            patientName = query.value("name").toString();
            historyText = QString("Name: %1\nAge: %2\nDisease: %3")
                              .arg(patientName)
                              .arg(query.value("age").toInt())
                              .arg(query.value("disease").toString());
        } else {
            historyText = "No history available for patient ID: " + patientId;
            qWarning() << "Failed to fetch patient history:" << query.lastError().text();
        }
    }

    setWindowTitle("Patient History - " + patientName);
    resize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QTextEdit *historyDisplay = new QTextEdit(this);
    historyDisplay->setReadOnly(true);
    historyDisplay->setText(historyText);
    layout->addWidget(historyDisplay);

    QPushButton *closeButton = new QPushButton("Close", this);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeButton);

    setLayout(layout);
}

DoctorDashboardTodaysWork::DoctorDashboardTodaysWork(const QString &doctorId, QWidget *parent)
    : QDialog(parent), m_doctorId(doctorId.trimmed())
{
    qDebug() << "DoctorDashboardTodaysWork constructor called with doctorId:" << m_doctorId;
    if (m_doctorId.isEmpty()) {
        qCritical() << "ERROR: DoctorDashboardTodaysWork received an empty doctorId!";
    }
    setupUi();
    populateAppointments();
    populateTasks();
}

DoctorDashboardTodaysWork::~DoctorDashboardTodaysWork()
{
    qDebug() << "Dashboard destroyed";
}

void DoctorDashboardTodaysWork::setupUi()
{
    setWindowTitle("Today's Work");
    resize(800, 500);
    setStyleSheet("background-color: #f0f2f5;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QString headerText = QString("Today's Work - %1").arg(QDate::currentDate().toString("MMMM dd, yyyy"));
    headerLabel = new QLabel(headerText, this);
    headerLabel->setStyleSheet(
        "font-family: 'Sans Serif'; font-size: 20px; font-weight: bold; color: #1a3c5e; padding: 10px;"
        );
    headerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(headerLabel);

    tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #1e90ff; border-radius: 5px; background: #ffffff; }"
        "QTabBar::tab { background: #1e90ff; color: white; padding: 8px 15px; margin-right: 2px; border-top-left-radius: 5px; border-top-right-radius: 5px; }"
        "QTabBar::tab:selected { background: #1565c0; }"
        "QTabBar::tab:hover { background: #63b3ed; }"
        );

    // Appointments Tab
    QWidget *appointmentsTab = new QWidget();
    QVBoxLayout *appointmentsLayout = new QVBoxLayout(appointmentsTab);
    appointmentsTable = new QTableWidget(0, 4, this);
    QStringList appointmentHeaders{"Date", "Patient Name", "Status", "Action"};
    appointmentsTable->setHorizontalHeaderLabels(appointmentHeaders);

    appointmentsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    appointmentsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    appointmentsTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    appointmentsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    appointmentsTable->setColumnWidth(3, 100);

    appointmentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    appointmentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    appointmentsTable->setStyleSheet(
        "QTableWidget { background: #ffffff; border: 1px solid #1e90ff; border-radius: 5px; }"
        "QHeaderView::section { background: #1e90ff; color: white; padding: 5px; border: none; }"
        );
    appointmentsLayout->addWidget(appointmentsTable);
    tabWidget->addTab(appointmentsTab, "Appointments");

    // Tasks Tab
    QWidget *tasksTab = new QWidget();
    QVBoxLayout *tasksLayout = new QVBoxLayout(tasksTab);
    tasksTable = new QTableWidget(0, 5, this);
    QStringList taskHeaders{"Description", "Assigned Date", "Due Date", "Status", "Action"};
    tasksTable->setHorizontalHeaderLabels(taskHeaders);

    tasksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    tasksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tasksTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tasksTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tasksTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Fixed);
    tasksTable->setColumnWidth(4, 100);

    tasksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tasksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    tasksTable->setStyleSheet(
        "QTableWidget { background: #ffffff; border: #1e90ff; border-radius: 5px; }"
        "QHeaderView::section { background: #1e90ff; color: white; padding: 5px; border: none; }"
        );
    tasksLayout->addWidget(tasksTable);
    tabWidget->addTab(tasksTab, "Tasks");

    mainLayout->addWidget(tabWidget);
    setLayout(mainLayout);
}

void DoctorDashboardTodaysWork::populateAppointments()
{
    qDebug() << "Starting populateAppointments with doctor ID:" << m_doctorId;

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        if (!db.open()) {
            qCritical() << "Failed to open database:" << db.lastError().text();
            appointmentsTable->setRowCount(0);
            appointmentsTable->insertRow(0);
            appointmentsTable->setItem(0, 0, new QTableWidgetItem("Error: Unable to connect to the database."));
            appointmentsTable->setSpan(0, 0, 1, 4);
            return;
        }
    }

    if (m_doctorId.isEmpty()) {
        qCritical() << "ERROR: Empty doctor ID provided!";
        appointmentsTable->setRowCount(0);
        appointmentsTable->insertRow(0);
        appointmentsTable->setItem(0, 0, new QTableWidgetItem("Error: Invalid doctor ID."));
        appointmentsTable->setSpan(0, 0, 1, 4);
        return;
    }

    QSqlQuery doctorCheck(db);
    doctorCheck.prepare("SELECT name FROM doctors WHERE id = :doctorId");
    doctorCheck.bindValue(":doctorId", m_doctorId);

    if (!doctorCheck.exec()) {
        qCritical() << "Doctor check failed:" << doctorCheck.lastError().text();
        appointmentsTable->setRowCount(0);
        appointmentsTable->insertRow(0);
        appointmentsTable->setItem(0, 0, new QTableWidgetItem("Error: Failed to verify doctor."));
        appointmentsTable->setSpan(0, 0, 1, 4);
        return;
    }

    if (!doctorCheck.next()) {
        qCritical() << "No doctor found with ID:" << m_doctorId;
        appointmentsTable->setRowCount(0);
        appointmentsTable->insertRow(0);
        appointmentsTable->setItem(0, 0, new QTableWidgetItem("Error: Doctor not found."));
        appointmentsTable->setSpan(0, 0, 1, 4);
        return;
    }

    QString doctorName = doctorCheck.value("name").toString();
    qDebug() << "Loading appointments for doctor:" << doctorName;

    QSqlQuery query(db);
    query.prepare(
        "SELECT a.appointment_date, p.name, a.status, p.id "
        "FROM appointments a "
        "JOIN patients p ON a.patient_id = p.id "
        "WHERE a.doctor_id = :doctorId "
        "ORDER BY a.appointment_date"
        );
    query.bindValue(":doctorId", m_doctorId);
    qDebug() << "Executing appointment query:" << query.lastQuery();
    qDebug() << "Bound doctorId:" << m_doctorId;

    if (!query.exec()) {
        qCritical() << "Query failed:" << query.lastError().text();
        appointmentsTable->setRowCount(0);
        appointmentsTable->insertRow(0);
        appointmentsTable->setItem(0, 0, new QTableWidgetItem("Error: Failed to load appointments."));
        appointmentsTable->setSpan(0, 0, 1, 4);
        return;
    }

    appointmentsTable->setRowCount(0);
    while (query.next()) {
        int row = appointmentsTable->rowCount();
        appointmentsTable->insertRow(row);

        appointmentsTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        appointmentsTable->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        appointmentsTable->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));

        QPushButton *viewButton = new QPushButton("View", this);
        viewButton->setStyleSheet(
            "QPushButton { background: #1e90ff; color: white; border: none; border-radius: 5px; padding: 5px; }"
            "QPushButton:hover { background: #1565c0; }"
            "QPushButton:pressed { background: #0d47a1; }"
            );
        viewButton->setProperty("patientId", query.value(3).toString());
        connect(viewButton, &QPushButton::clicked, this, &DoctorDashboardTodaysWork::onViewButtonClicked);
        appointmentsTable->setCellWidget(row, 3, viewButton);
    }

    if (appointmentsTable->rowCount() == 0) {
        appointmentsTable->insertRow(0);
        appointmentsTable->setItem(0, 0, new QTableWidgetItem("No appointments found"));
        appointmentsTable->setSpan(0, 0, 1, 4);
    }

    qDebug() << "Loaded" << appointmentsTable->rowCount() << "appointments";
}

void DoctorDashboardTodaysWork::populateTasks()
{
    qDebug() << "Starting populateTasks with doctor ID:" << m_doctorId;

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        if (!db.open()) {
            qCritical() << "Failed to open database:" << db.lastError().text();
            tasksTable->setRowCount(0);
            tasksTable->insertRow(0);
            tasksTable->setItem(0, 0, new QTableWidgetItem("Error: Unable to connect to the database."));
            tasksTable->setSpan(0, 0, 1, 5);
            return;
        }
    }

    if (m_doctorId.isEmpty()) {
        qCritical() << "ERROR: Empty doctor ID provided!";
        tasksTable->setRowCount(0);
        tasksTable->insertRow(0);
        tasksTable->setItem(0, 0, new QTableWidgetItem("Error: Invalid doctor ID."));
        tasksTable->setSpan(0, 0, 1, 5);
        return;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT id, task_description, assigned_date, due_date, status "
        "FROM tasks "
        "WHERE assignee_id = :doctorId AND assignee_type = 'doctor' AND status = 'Pending' "
        "ORDER BY due_date"
        );
    query.bindValue(":doctorId", m_doctorId);

    if (!query.exec()) {
        qCritical() << "Task query failed:" << query.lastError().text();
        tasksTable->setRowCount(0);
        tasksTable->insertRow(0);
        tasksTable->setItem(0, 0, new QTableWidgetItem("Error: Failed to load tasks."));
        tasksTable->setSpan(0, 0, 1, 5);
        return;
    }

    tasksTable->setRowCount(0);
    while (query.next()) {
        int row = tasksTable->rowCount();
        tasksTable->insertRow(row);

        tasksTable->setItem(row, 0, new QTableWidgetItem(query.value(1).toString()));
        tasksTable->setItem(row, 1, new QTableWidgetItem(query.value(2).toString()));
        tasksTable->setItem(row, 2, new QTableWidgetItem(query.value(3).toString()));
        tasksTable->setItem(row, 3, new QTableWidgetItem(query.value(4).toString()));

        QPushButton *completeButton = new QPushButton("Mark Complete", this);
        completeButton->setStyleSheet(
            "QPushButton { background: #e53e3e; color: white; border: none; border-radius: 5px; padding: 5px; }"
            "QPushButton:hover { background: #c53030; }"
            "QPushButton:pressed { background: #b71c1c; }"
            );
        completeButton->setProperty("taskId", query.value(0).toString());
        connect(completeButton, &QPushButton::clicked, this, &DoctorDashboardTodaysWork::onMarkCompleteButtonClicked);
        tasksTable->setCellWidget(row, 4, completeButton);
    }

    if (tasksTable->rowCount() == 0) {
        tasksTable->insertRow(0);
        tasksTable->setItem(0, 0, new QTableWidgetItem("No pending tasks"));
        tasksTable->setSpan(0, 0, 1, 5);
    }

    qDebug() << "Loaded" << tasksTable->rowCount() << "tasks";
}

void DoctorDashboardTodaysWork::onViewButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        qWarning() << "Invalid sender in onViewButtonClicked";
        return;
    }
    QString patientId = button->property("patientId").toString();
    qDebug() << "Viewing history for patient ID:" << patientId;
    PatientHistoryDialog *historyDialog = new PatientHistoryDialog(patientId, this);
    historyDialog->setAttribute(Qt::WA_DeleteOnClose);
    historyDialog->exec();
}

void DoctorDashboardTodaysWork::onMarkCompleteButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) {
        qWarning() << "Invalid sender in onMarkCompleteButtonClicked";
        return;
    }
    QString taskId = button->property("taskId").toString();
    qDebug() << "Marking task ID:" << taskId << "as complete";

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        if (!db.open()) {
            qCritical() << "Failed to open database:" << db.lastError().text();
            return;
        }
    }

    QSqlQuery query(db);
    query.prepare("UPDATE tasks SET status = 'Completed' WHERE id = :taskId");
    query.bindValue(":taskId", taskId);

    if (!query.exec()) {
        qCritical() << "Failed to update task status:" << query.lastError().text();
        return;
    }

    qDebug() << "Task ID:" << taskId << "marked as complete";
    populateTasks(); // Refresh the tasks table
}
