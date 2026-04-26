#include "appointmentswindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QHeaderView>
#include <QGridLayout>
#include <QDebug>
#include <QScreen>
#include <QApplication>
#include <QHBoxLayout>

DoctorProfileDialog::DoctorProfileDialog(const QString &doctorId, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Doctor Profile");
    setMinimumSize(400, 300);

    QGridLayout *layout = new QGridLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(10);

    loadDoctorData(doctorId, layout);
    setupUI();

    // Center the dialog relative to the parent (PatientDashboard)
    if (parent) {
        QWidget *parentWidget = parent->window();
        QRect parentGeometry = parentWidget->geometry();
        QPoint parentCenter = parentGeometry.center();
        QSize dialogSize = sizeHint();
        move(parentCenter.x() - dialogSize.width() / 2, parentCenter.y() - dialogSize.height() / 2);
    }
}

void DoctorProfileDialog::setupUI()
{
    QGridLayout *layout = qobject_cast<QGridLayout*>(this->layout());

    QPushButton *closeButton = new QPushButton("Close", this);
    closeButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #e0e0e0;"
        "   color: #333333;"
        "   border: 1px solid #b0b0b0;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   font-size: 14px;"
        "   font-weight: normal;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d0d0d0;"
        "}"
        "QPushButton:focus {"
        "   outline: none;"
        "   border: 1px solid #888888;"
        "}"
        );
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    layout->addWidget(closeButton, layout->rowCount(), 1, 1, 1, Qt::AlignRight);
}

void DoctorProfileDialog::loadDoctorData(const QString &doctorId, QGridLayout *layout)
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "DoctorProfileDialog: Database connection failed.";
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT name, specialization, age FROM doctors WHERE id = :id");
    query.bindValue(":id", doctorId);

    if (query.exec() && query.next()) {
        QString name = query.value("name").toString();
        QString specialization = query.value("specialization").toString();
        int age = query.value("age").toInt();

        // Style for labels
        QString labelStyle =
            "QLabel {"
            "   color: #333333;"
            "   font-size: 14px;"
            "   padding: 5px;"
            "}";

        QString fieldStyle =
            "QLabel {"
            "   color: #555555;"
            "   font-size: 14px;"
            "   padding: 5px;"
            "}";

        QLabel *nameLabel = new QLabel("Name:", this);
        nameLabel->setStyleSheet(labelStyle);
        QLabel *nameValue = new QLabel(name, this);
        nameValue->setStyleSheet(fieldStyle);

        QLabel *specializationLabel = new QLabel("Specialization:", this);
        specializationLabel->setStyleSheet(labelStyle);
        QLabel *specializationValue = new QLabel(specialization, this);
        specializationValue->setStyleSheet(fieldStyle);

        QLabel *ageLabel = new QLabel("Age:", this);
        ageLabel->setStyleSheet(labelStyle);
        QLabel *ageValue = new QLabel(QString::number(age), this);
        ageValue->setStyleSheet(fieldStyle);

        layout->addWidget(nameLabel, 0, 0);
        layout->addWidget(nameValue, 0, 1);
        layout->addWidget(specializationLabel, 1, 0);
        layout->addWidget(specializationValue, 1, 1);
        layout->addWidget(ageLabel, 2, 0);
        layout->addWidget(ageValue, 2, 1);
    } else {
        qDebug() << "DoctorProfileDialog: Doctor not found or query failed:" << query.lastError().text();
    }
}

AppointmentsWindow::AppointmentsWindow(const QString &patientId, QWidget *parent)
    : QWidget(parent), patientId(patientId)
{
    setWindowTitle("Appointments");
    setMinimumSize(600, 400);
    setupUI();
    loadAppointments();
}

void AppointmentsWindow::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Frame for the header section
    QFrame *headerFrame = new QFrame(this);
    headerFrame->setFrameShape(QFrame::Box);
    headerFrame->setStyleSheet(
        "QFrame {"
        "   background-color: #ffffff;"
        "   border: 1px solid #e0e0e0;"
        "   padding: 10px;"
        "}"
        );
    QHBoxLayout *headerLayout = new QHBoxLayout(headerFrame);
    headerLayout->setSpacing(10);

    QPushButton *backButton = new QPushButton("Back", headerFrame);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #e0e0e0;"
        "   color: #333333;"
        "   border: 1px solid #b0b0b0;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   font-size: 14px;"
        "   font-weight: normal;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d0d0d0;"
        "}"
        "QPushButton:focus {"
        "   outline: none;"
        "   border: 1px solid #888888;"
        "}"
        );
    connect(backButton, &QPushButton::clicked, this, &AppointmentsWindow::handleBackButtonClick);

    QLabel *titleLabel = new QLabel("Appointments", headerFrame);
    QFont titleFont("Sans Serif", 16, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "   color: #333333;"
        "   margin-bottom: 15px;"
        "}"
        );

    headerLayout->addWidget(backButton);
    headerLayout->addStretch();
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    appointmentsTable = new QTableWidget(this);
    appointmentsTable->setColumnCount(4);
    appointmentsTable->setHorizontalHeaderLabels({"Date", "Time", "Doctor", "View"});
    appointmentsTable->horizontalHeader()->setStretchLastSection(true);
    appointmentsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    appointmentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    appointmentsTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: #ffffff;"
        "   border: 1px solid #e0e0e0;"
        "   gridline-color: #e0e0e0;"
        "   font-size: 14px;"
        "   color: #333333;"
        "}"
        "QHeaderView::section {"
        "   background-color: #f5f5f5;"
        "   color: #333333;"
        "   padding: 6px;"
        "   border: 1px solid #e0e0e0;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "}"
        "QTableWidget::item {"
        "   padding: 5px;"
        "}"
        "QTableWidget::item:selected {"
        "   background-color: #e8e8e8;"
        "   color: #333333;"
        "}"
        );

    mainLayout->addWidget(headerFrame);
    mainLayout->addWidget(appointmentsTable);
}

void AppointmentsWindow::loadAppointments()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "AppointmentsWindow: Database connection failed.";
        return;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT DISTINCT a.date, a.appointment_time, d.name, d.id "
        "FROM appointments a "
        "JOIN doctors d ON a.doctor_id = d.id "
        "WHERE a.patient_id = :patientId "
        "ORDER BY a.date, a.appointment_time"
        );
    query.bindValue(":patientId", patientId);

    if (!query.exec()) {
        qDebug() << "AppointmentsWindow: Failed to load appointments:" << query.lastError().text();
        return;
    }

    // Log the raw query results to check for duplicates
    qDebug() << "AppointmentsWindow: Raw query results for patient" << patientId;
    int resultCount = 0;
    while (query.next()) {
        resultCount++;
        qDebug() << "Result" << resultCount << ": Date =" << query.value("date").toString()
                 << ", Time =" << query.value("appointment_time").toString()
                 << ", Doctor =" << query.value("name").toString()
                 << ", Doctor ID =" << query.value("id").toString();
    }
    query.seek(-1); // Reset the cursor to before the first row for the actual table population

    appointmentsTable->setRowCount(0);
    while (query.next()) {
        int row = appointmentsTable->rowCount();
        appointmentsTable->insertRow(row);

        QTableWidgetItem *dateItem = new QTableWidgetItem(query.value("date").toString());
        QTableWidgetItem *timeItem = new QTableWidgetItem(query.value("appointment_time").toString());
        QTableWidgetItem *doctorItem = new QTableWidgetItem(query.value("name").toString());

        QPushButton *viewButton = new QPushButton("View", this);
        viewButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #e0e0e0;"
            "   color: #333333;"
            "   border: 1px solid #b0b0b0;"
            "   border-radius: 5px;"
            "   padding: 5px;"
            "   font-size: 14px;"
            "}"
            "QPushButton:hover {"
            "   background-color: #d0d0d0;"
            "}"
            "QPushButton:focus {"
            "   outline: none;"
            "   border: 1px solid #888888;"
            "}"
            );

        QString doctorId = query.value("id").toString();
        connect(viewButton, &QPushButton::clicked, this, [=]() {
            DoctorProfileDialog *dialog = new DoctorProfileDialog(doctorId, this);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->exec();
        });

        appointmentsTable->setItem(row, 0, dateItem);
        appointmentsTable->setItem(row, 1, timeItem);
        appointmentsTable->setItem(row, 2, doctorItem);
        appointmentsTable->setCellWidget(row, 3, viewButton);
    }

    appointmentsTable->resizeColumnsToContents();
    qDebug() << "AppointmentsWindow: Loaded" << appointmentsTable->rowCount() << "unique appointments for patient" << patientId;
}

void AppointmentsWindow::handleBackButtonClick()
{
    close();
}
