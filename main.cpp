#include "mainwindow.h"
#include "welcomescreen.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <QDebug>
#include <QSplashScreen>
#include <QTimer>
#include <QFile>

bool createTable(QSqlQuery &query, const QString &sql, const QString &tableName) {
    if (!query.exec(sql)) {
        qCritical() << "[DB Init] Error creating" << tableName << "table:" << query.lastError().text();
        return false;
    }
    return true;
}

bool initializeDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "hospitalConnection");
    QString dbPath = QDir::currentPath() + "/ned_hospital.db";

    // Force delete the existing database to ensure fresh data
    QFile::remove(dbPath);
    qDebug() << "Deleted existing database at:" << dbPath;

    qDebug() << "Attempting to open database at:" << dbPath;

    db.setDatabaseName(dbPath);

    if (!db.open()) {
        qCritical() << "Failed to open database at" << dbPath;
        qCritical() << "Error:" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully at:" << dbPath;

    QSqlQuery query(db);

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS doctors ("
                     "id TEXT PRIMARY KEY, "
                     "password TEXT NOT NULL, "
                     "name TEXT, "
                     "specialization TEXT, "
                     "age INTEGER)", "doctors")) return false;

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS patients ("
                     "id TEXT PRIMARY KEY, "
                     "password TEXT NOT NULL, "
                     "name TEXT, "
                     "contact TEXT, "
                     "age INTEGER, "
                     "disease TEXT, "
                     "admitDate TEXT)", "patients")) return false;

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS appointments ("
                     "id TEXT PRIMARY KEY, "
                     "patient_id TEXT, "
                     "doctor_id TEXT, "
                     "appointment_date TEXT, "
                     "status TEXT, "
                     "FOREIGN KEY (patient_id) REFERENCES patients(id), "
                     "FOREIGN KEY (doctor_id) REFERENCES doctors(id))", "appointments")) return false;

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS admins ("
                     "id TEXT PRIMARY KEY, "
                     "password TEXT NOT NULL, "
                     "name TEXT)", "admins")) return false;

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS lab_reports ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "patient_id TEXT, "
                     "doctor_id TEXT, "
                     "test_name TEXT, "
                     "result TEXT, "
                     "date TEXT, "
                     "status TEXT, "
                     "FOREIGN KEY (patient_id) REFERENCES patients(id), "
                     "FOREIGN KEY (doctor_id) REFERENCES doctors(id))", "lab_reports")) return false;

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS tasks ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "assignee_id TEXT, "
                     "assignee_type TEXT, " // 'doctor', 'employee'
                     "task_description TEXT, "
                     "assigned_date TEXT, "
                     "due_date TEXT, "
                     "status TEXT, "
                     "FOREIGN KEY (assignee_id) REFERENCES doctors(id) "
                     "FOREIGN KEY (assignee_id) REFERENCES employees(id))", "tasks")) return false;

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS payments ("
                     "id TEXT PRIMARY KEY, "
                     "patient_id TEXT, "
                     "amount REAL, "
                     "payment_date TEXT, "
                     "status TEXT, "
                     "FOREIGN KEY (patient_id) REFERENCES patients(id))", "payments")) return false;

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS doctor_feedback ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "patient_id TEXT, "
                     "doctor_id TEXT, "
                     "rating INTEGER, "
                     "comment TEXT, "
                     "submission_date TEXT, "
                     "FOREIGN KEY (patient_id) REFERENCES patients(id), "
                     "FOREIGN KEY (doctor_id) REFERENCES doctors(id))", "doctor_feedback")) return false;

    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS general_feedback ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                     "patient_id TEXT, "
                     "rating INTEGER, "
                     "comment TEXT, "
                     "submission_date TEXT, "
                     "FOREIGN KEY (patient_id) REFERENCES patients(id))", "general_feedback")) return false;

    // Add doctor_salaries table
    if (!createTable(query,
                     "CREATE TABLE IF NOT EXISTS doctor_salaries ("
                     "doctor_id TEXT, "
                     "salary REAL, "
                     "payment_date TEXT, "
                     "overtime_salary REAL, "
                     "salary_reduction REAL, "
                     "PRIMARY KEY (doctor_id, payment_date), "
                     "FOREIGN KEY (doctor_id) REFERENCES doctors(id))", "doctor_salaries")) return false;

    // Insert placeholder salary data for existing doctors
    QDate today = QDate::currentDate();
    for (int i = 0; i < 2; ++i) {
        QString doctorId = (i == 0) ? "doc1" : "doc2";
        QString paymentDate = today.addDays(-i).toString("yyyy-MM-dd");
        double baseSalary = 5000.0 + (i * 1000.0); // Dynamic base salary
        double overtime = 200.0 + (i * 50.0); // Dynamic overtime
        double reduction = 100.0 + (i * 20.0); // Dynamic reduction

        query.prepare("INSERT OR IGNORE INTO doctor_salaries (doctor_id, salary, payment_date, overtime_salary, salary_reduction) "
                      "VALUES (:doctorId, :salary, :paymentDate, :overtime, :reduction)");
        query.bindValue(":doctorId", doctorId);
        query.bindValue(":salary", baseSalary);
        query.bindValue(":paymentDate", paymentDate);
        query.bindValue(":overtime", overtime);
        query.bindValue(":reduction", reduction);

        if (!query.exec()) {
            qWarning() << "Failed to insert salary for" << doctorId << ":" << query.lastError().text();
        } else {
            qDebug() << "Inserted placeholder salary for" << doctorId << "on" << paymentDate;
        }
    }

    // Insert sample data and confirm insertion
    if (query.exec("INSERT OR IGNORE INTO doctors (id, password, name, specialization, age) "
                   "VALUES ('doc1', 'pass123', 'Dr. Ahmed', 'Cardiology', 35)")) {
        qDebug() << "Inserted doctor doc1";
    } else {
        qWarning() << "Failed to insert doctor doc1:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO doctors (id, password, name, specialization, age) "
                   "VALUES ('doc2', 'pass456', 'Dr. Sarah', 'Neurology', 40)")) {
        qDebug() << "Inserted doctor doc2";
    } else {
        qWarning() << "Failed to insert doctor doc2:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO patients (id, password, name, contact, age, disease, admitDate) "
                   "VALUES ('pat1', 'pass123', 'Ali Khan', '1234567890', 30, 'Flu', '2025-05-01')")) {
        qDebug() << "Inserted patient pat1";
    } else {
        qWarning() << "Failed to insert patient pat1:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO patients (id, password, name, contact, age, disease, admitDate) "
                   "VALUES ('pat2', 'pass456', 'Sara Ali', '0987654321', 25, 'Asthma', '2025-05-02')")) {
        qDebug() << "Inserted patient pat2";
    } else {
        qWarning() << "Failed to insert patient pat2:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO appointments (id, patient_id, doctor_id, appointment_date, status) "
                   "VALUES ('appt1', 'pat1', 'doc1', '2025-05-06', 'Scheduled')")) {
        qDebug() << "Inserted appointment appt1 for pat1, doc1";
    } else {
        qWarning() << "Failed to insert appointment appt1:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO appointments (id, patient_id, doctor_id, appointment_date, status) "
                   "VALUES ('appt2', 'pat1', 'doc2', '2025-05-07', 'Scheduled')")) {
        qDebug() << "Inserted appointment appt2 for pat1, doc2";
    } else {
        qWarning() << "Failed to insert appointment appt2:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO appointments (id, patient_id, doctor_id, appointment_date, status) "
                   "VALUES ('appt3', 'pat2', 'doc1', '2025-05-06', 'Completed')")) {
        qDebug() << "Inserted appointment appt3 for pat2, doc1";
    } else {
        qWarning() << "Failed to insert appointment appt3:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO admins (id, password, name) "
                   "VALUES ('admin1', 'pass123', 'Hassan Anwar')")) {
        qDebug() << "Inserted admin admin1";
    } else {
        qWarning() << "Failed to insert admin admin1:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO admins (id, password, name) "
                   "VALUES ('admin2', 'pass456', 'Ayesha Khan')")) {
        qDebug() << "Inserted admin admin2";
    } else {
        qWarning() << "Failed to insert admin admin2:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO lab_reports (patient_id, doctor_id, test_name, result, date, status) "
                   "VALUES ('pat1', 'doc1', 'Blood Test', 'Normal', '2025-05-02', 'Completed')")) {
        qDebug() << "Inserted lab report for pat1";
    } else {
        qWarning() << "Failed to insert lab report for pat1:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO lab_reports (patient_id, doctor_id, test_name, result, date, status) "
                   "VALUES ('pat2', 'doc2', 'X-Ray', 'Pending', '2025-05-03', 'Pending')")) {
        qDebug() << "Inserted lab report for pat2";
    } else {
        qWarning() << "Failed to insert lab report for pat2:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO payments (id, patient_id, amount, payment_date, status) "
                   "VALUES ('pay1', 'pat1', 5000.00, '2025-05-03', 'Pending')")) {
        qDebug() << "Inserted payment pay1 for pat1";
    } else {
        qWarning() << "Failed to insert payment pay1:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO payments (id, patient_id, amount, payment_date, status) "
                   "VALUES ('pay2', 'pat2', 7000.00, '2025-05-04', 'Completed')")) {
        qDebug() << "Inserted payment pay2 for pat2";
    } else {
        qWarning() << "Failed to insert payment pay2:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO tasks (assignee_id, assignee_type, task_description, assigned_date, due_date, status) "
                   "VALUES ('doc1', 'doctor', 'Review patient pat1 lab report', '2025-05-05', '2025-05-06', 'Pending')")) {
        qDebug() << "Inserted task for doc1";
    } else {
        qWarning() << "Failed to insert task for doc1:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO doctor_feedback (patient_id, doctor_id, rating, comment, submission_date) "
                   "VALUES ('pat1', 'doc1', 5, 'Excellent care', '2025-05-05')")) {
        qDebug() << "Inserted doctor feedback for pat1, doc1";
    } else {
        qWarning() << "Failed to insert doctor feedback for pat1, doc1:" << query.lastError().text();
    }
    if (query.exec("INSERT OR IGNORE INTO general_feedback (patient_id, rating, comment, submission_date) "
                   "VALUES ('pat1', 4, 'Good facilities', '2025-05-05')")) {
        qDebug() << "Inserted general feedback for pat1";
    } else {
        qWarning() << "Failed to insert general feedback for pat1:" << query.lastError().text();
    }

    QStringList requiredTables = {
        QStringLiteral("doctors"),
        QStringLiteral("patients"),
        QStringLiteral("appointments"),
        QStringLiteral("admins"),
        QStringLiteral("lab_reports"),
        QStringLiteral("tasks"),
        QStringLiteral("payments"),
        QStringLiteral("doctor_feedback"),
        QStringLiteral("general_feedback"),
        QStringLiteral("doctor_salaries")
    };

    bool tablesExist = true;
    for (const QString &table : requiredTables) {
        if (!query.exec(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1'").arg(table))) {
            qCritical() << "Table check failed for" << table << ":" << query.lastError().text();
            tablesExist = false;
            break;
        }
        if (!query.next()) {
            qCritical() << "Required table missing:" << table;
            tablesExist = false;
            break;
        }
    }

    if (!tablesExist) {
        qCritical() << "Required tables are missing or inaccessible";
        return false;
    }

    qDebug() << "\nDatabase tables:";
    if (query.exec("SELECT name FROM sqlite_master WHERE type='table'")) {
        while (query.next()) {
            qDebug() << "Found table:" << query.value(0).toString();
        }
    }

    qDebug() << "Database initialized successfully.";
    return true;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    a.setApplicationName("Hospital Management System");
    a.setOrganizationName("MyOrganization");

    if (!initializeDatabase()) {
        qCritical() << "Failed to initialize database. Exiting.";
        return -1;
    }

    {
        QSplashScreen splash(QPixmap(":/images/splash.png"));
        splash.show();
        a.processEvents();
        QTimer::singleShot(3000, &splash, &QSplashScreen::close);
    }

    WelcomeScreen welcome;
    if (welcome.exec() == QDialog::Accepted) {
        MainWindow *mainWindow = new MainWindow();
        mainWindow->showMaximized();

        if (!QSqlDatabase::database("hospitalConnection").isOpen()) {
            qWarning() << "Database connection was lost";
        }
    }

    return a.exec();
}
