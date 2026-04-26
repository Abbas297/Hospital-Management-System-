#include "patientdashboarddocfeedback.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTextEdit>
#include <QSpinBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

PatientDashboardDocFeedback::PatientDashboardDocFeedback(const QString &patientId, QWidget *parent)
    : QWidget(parent), patientId(patientId)
{
    setupUI();
    setupStyles();
    populateDoctors();

    setWindowTitle("Doctor Feedback");
    setMinimumSize(400, 500);
    resize(450, 550);
}

PatientDashboardDocFeedback::~PatientDashboardDocFeedback() {}

void PatientDashboardDocFeedback::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Single Frame for all content
    feedbackFrame = new QFrame(this);
    feedbackFrame->setFrameShape(QFrame::NoFrame);
    QVBoxLayout *contentLayout = new QVBoxLayout(feedbackFrame);
    contentLayout->setContentsMargins(15, 15, 15, 15);
    contentLayout->setSpacing(15);
    contentLayout->setAlignment(Qt::AlignCenter);

    // Title
    titleLabel = new QLabel("Doctor Feedback", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setToolTip("Provide feedback for a doctor");

    // Doctor Selection
    doctorLabel = new QLabel("Select Doctor:", this);
    doctorLabel->setAlignment(Qt::AlignLeft);
    doctorComboBox = new QComboBox(this);
    doctorComboBox->setToolTip("Choose a doctor to provide feedback for");

    // Rating
    ratingLabel = new QLabel("Rating (1-5):", this);
    ratingLabel->setAlignment(Qt::AlignLeft);
    ratingSpinBox = new QSpinBox(this);
    ratingSpinBox->setRange(1, 5);
    ratingSpinBox->setValue(3);
    ratingSpinBox->setToolTip("Rate the doctor from 1 (poor) to 5 (excellent)");

    // Comment
    commentLabel = new QLabel("Comment:", this);
    commentLabel->setAlignment(Qt::AlignLeft);
    commentTextEdit = new QTextEdit(this);
    commentTextEdit->setFixedHeight(100);
    commentTextEdit->setToolTip("Enter your feedback comments");

    // Confirmation Label (hidden initially)
    confirmationLabel = new QLabel("", this);
    confirmationLabel->setAlignment(Qt::AlignCenter);
    confirmationLabel->setVisible(false);

    // Buttons
    submitButton = new QPushButton("Submit", this);
    submitButton->setToolTip("Submit your feedback");
    backButton = new QPushButton("Back", this);
    backButton->setToolTip("Return to the previous screen");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(submitButton);
    buttonLayout->addWidget(backButton);

    // Add to content layout
    contentLayout->addWidget(titleLabel);
    contentLayout->addWidget(doctorLabel);
    contentLayout->addWidget(doctorComboBox);
    contentLayout->addWidget(ratingLabel);
    contentLayout->addWidget(ratingSpinBox);
    contentLayout->addWidget(commentLabel);
    contentLayout->addWidget(commentTextEdit);
    contentLayout->addWidget(confirmationLabel);
    contentLayout->addLayout(buttonLayout);

    mainLayout->addWidget(feedbackFrame);

    connect(submitButton, &QPushButton::clicked, this, &PatientDashboardDocFeedback::submitFeedback);
    connect(backButton, &QPushButton::clicked, this, &QWidget::close);
}

void PatientDashboardDocFeedback::setupStyles()
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
    doctorLabel->setStyleSheet(labelStyle);
    ratingLabel->setStyleSheet(labelStyle);
    commentLabel->setStyleSheet(labelStyle);

    confirmationLabel->setStyleSheet(
        "QLabel {"
        "   color: #2e7d32;"
        "   font-size: 14px;"
        "   font-weight: medium;"
        "   padding: 5px;"
        "}"
        );

    doctorComboBox->setStyleSheet(
        "QComboBox {"
        "   background-color: #ffffff;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 6px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "   min-width: 200px;"
        "}"
        "QComboBox:hover {"
        "   border: 1px solid #1e90ff;"
        "}"
        "QComboBox:focus {"
        "   border: 1px solid #1e90ff;"
        "   outline: none;"
        "}"
        );

    ratingSpinBox->setStyleSheet(
        "QSpinBox {"
        "   background-color: #ffffff;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 6px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "   min-width: 60px;"
        "}"
        "QSpinBox:hover {"
        "   border: 1px solid #1e90ff;"
        "}"
        "QSpinBox:focus {"
        "   border: 1px solid #1e90ff;"
        "   outline: none;"
        "}"
        );

    commentTextEdit->setStyleSheet(
        "QTextEdit {"
        "   background-color: #ffffff;"
        "   border: 1px solid #d1d5db;"
        "   border-radius: 6px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "}"
        "QTextEdit:hover {"
        "   border: 1px solid #1e90ff;"
        "}"
        "QTextEdit:focus {"
        "   border: 1px solid #1e90ff;"
        "   outline: none;"
        "}"
        );

    submitButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #1e90ff;"
        "   color: #ffffff;"
        "   border: none;"
        "   border-radius: 6px;"
        "   padding: 8px 20px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 80px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #1565c0;"
        "}"
        "QPushButton:focus {"
        "   outline: none;"
        "   border: 2px solid #63b3ed;"
        "}"
        );

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

void PatientDashboardDocFeedback::populateDoctors()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection failed.";
        doctorComboBox->addItem("Error: Database connection failed");
        doctorComboBox->setEnabled(false);
        submitButton->setEnabled(false);
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id, name FROM doctors")) {
        qDebug() << "Query failed:" << query.lastError().text();
        doctorComboBox->addItem("Error: Unable to fetch doctors");
        doctorComboBox->setEnabled(false);
        submitButton->setEnabled(false);
        return;
    }

    while (query.next()) {
        QString doctorId = query.value("id").toString();
        QString doctorName = query.value("name").toString();
        doctorComboBox->addItem(doctorName, doctorId);
    }

    if (doctorComboBox->count() == 0) {
        doctorComboBox->addItem("No doctors available");
        doctorComboBox->setEnabled(false);
        submitButton->setEnabled(false);
    }
}

void PatientDashboardDocFeedback::submitFeedback()
{
    if (doctorComboBox->count() == 0 || !doctorComboBox->isEnabled()) {
        confirmationLabel->setText("Error: Cannot submit feedback");
        confirmationLabel->setStyleSheet("QLabel { color: #d32f2f; font-size: 14px; font-weight: medium; padding: 5px; }");
        confirmationLabel->setVisible(true);
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        confirmationLabel->setText("Error: Database connection failed");
        confirmationLabel->setStyleSheet("QLabel { color: #d32f2f; font-size: 14px; font-weight: medium; padding: 5px; }");
        confirmationLabel->setVisible(true);
        return;
    }

    QString doctorId = doctorComboBox->currentData().toString();
    int rating = ratingSpinBox->value();
    QString comment = commentTextEdit->toPlainText();
    QString submissionDate = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query(db);
    query.prepare("INSERT INTO doctor_feedback (patient_id, doctor_id, rating, comment, submission_date) "
                  "VALUES (:patientId, :doctorId, :rating, :comment, :submissionDate)");
    query.bindValue(":patientId", patientId);
    query.bindValue(":doctorId", doctorId);
    query.bindValue(":rating", rating);
    query.bindValue(":comment", comment);
    query.bindValue(":submissionDate", submissionDate);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        confirmationLabel->setText("Error: Failed to submit feedback");
        confirmationLabel->setStyleSheet("QLabel { color: #d32f2f; font-size: 14px; font-weight: medium; padding: 5px; }");
    } else {
        confirmationLabel->setText("Feedback submitted successfully!");
        confirmationLabel->setStyleSheet("QLabel { color: #2e7d32; font-size: 14px; font-weight: medium; padding: 5px; }");
        submitButton->setEnabled(false);
        commentTextEdit->setReadOnly(true);
        ratingSpinBox->setReadOnly(true);
        doctorComboBox->setEnabled(false);
    }
    confirmationLabel->setVisible(true);
}

void PatientDashboardDocFeedback::showConfirmation()
{
    confirmationLabel->setVisible(true);
}
