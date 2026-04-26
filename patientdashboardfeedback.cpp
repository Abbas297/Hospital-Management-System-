#include "patientdashboardfeedback.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QSpinBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>

PatientDashboardFeedback::PatientDashboardFeedback(const QString &patientId, QWidget *parent)
    : QWidget(parent), patientId(patientId)
{
    setupUI();
    setupStyles();

    setWindowTitle("General Feedback");
    setMinimumSize(400, 500);
    resize(450, 550);
}

PatientDashboardFeedback::~PatientDashboardFeedback() {}

void PatientDashboardFeedback::setupUI()
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
    titleLabel = new QLabel("General Feedback", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setToolTip("Provide feedback about hospital services and staff");

    // Rating
    ratingLabel = new QLabel("Overall Rating (1-5):", this);
    ratingLabel->setAlignment(Qt::AlignLeft);
    ratingSpinBox = new QSpinBox(this);
    ratingSpinBox->setRange(1, 5);
    ratingSpinBox->setValue(3);
    ratingSpinBox->setToolTip("Rate your overall experience from 1 (poor) to 5 (excellent)");

    // Comment
    commentLabel = new QLabel("Comments:", this);
    commentLabel->setAlignment(Qt::AlignLeft);
    commentTextEdit = new QTextEdit(this);
    commentTextEdit->setFixedHeight(100);
    commentTextEdit->setToolTip("Enter your feedback about hospital staff or services");

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
    contentLayout->addWidget(ratingLabel);
    contentLayout->addWidget(ratingSpinBox);
    contentLayout->addWidget(commentLabel);
    contentLayout->addWidget(commentTextEdit);
    contentLayout->addWidget(confirmationLabel);
    contentLayout->addLayout(buttonLayout);

    mainLayout->addWidget(feedbackFrame);

    connect(submitButton, &QPushButton::clicked, this, &PatientDashboardFeedback::submitFeedback);
    connect(backButton, &QPushButton::clicked, this, &QWidget::close);
}

void PatientDashboardFeedback::setupStyles()
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

void PatientDashboardFeedback::submitFeedback()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        confirmationLabel->setText("Error: Database connection failed");
        confirmationLabel->setStyleSheet("QLabel { color: #d32f2f; font-size: 14px; font-weight: medium; padding: 5px; }");
        confirmationLabel->setVisible(true);
        return;
    }

    int rating = ratingSpinBox->value();
    QString comment = commentTextEdit->toPlainText();
    QString submissionDate = QDate::currentDate().toString("yyyy-MM-dd");

    QSqlQuery query(db);
    query.prepare("INSERT INTO general_feedback (patient_id, rating, comment, submission_date) "
                  "VALUES (:patientId, :rating, :comment, :submissionDate)");
    query.bindValue(":patientId", patientId);
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
    }
    confirmationLabel->setVisible(true);
}

void PatientDashboardFeedback::showConfirmation()
{
    confirmationLabel->setVisible(true);
}
