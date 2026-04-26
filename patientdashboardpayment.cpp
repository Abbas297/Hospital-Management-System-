#include "patientdashboardpayment.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

PatientDashboardPayment::PatientDashboardPayment(const QString &patientId, QWidget *parent)
    : QWidget(parent), patientId(patientId)
{
    setupUI();
    setupStyles();
    fetchPaymentDetails();

    setWindowTitle("Payments");
    setMinimumSize(400, 300);
    resize(450, 350);
}

PatientDashboardPayment::~PatientDashboardPayment() {}

void PatientDashboardPayment::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Single Frame for all content
    paymentFrame = new QFrame(this);
    paymentFrame->setFrameShape(QFrame::NoFrame);
    QVBoxLayout *contentLayout = new QVBoxLayout(paymentFrame);
    contentLayout->setContentsMargins(15, 15, 15, 15);
    contentLayout->setSpacing(10);
    contentLayout->setAlignment(Qt::AlignCenter);

    // Title
    titleLabel = new QLabel("Payment Details", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setToolTip("Patient payment information");

    // Payment Details
    amountPaidLabel = new QLabel("Amount Paid: Checking...", this);
    amountPaidLabel->setAlignment(Qt::AlignCenter);
    amountPaidLabel->setToolTip("Total amount paid");

    amountRemainingLabel = new QLabel("Amount Remaining: Checking...", this);
    amountRemainingLabel->setAlignment(Qt::AlignCenter);
    amountRemainingLabel->setToolTip("Remaining balance");

    lastPaymentDateLabel = new QLabel("Last Payment Date: Checking...", this);
    lastPaymentDateLabel->setAlignment(Qt::AlignCenter);
    lastPaymentDateLabel->setToolTip("Date of last payment");

    nextDueDateLabel = new QLabel("Next Due Date: Checking...", this);
    nextDueDateLabel->setAlignment(Qt::AlignCenter);
    nextDueDateLabel->setToolTip("Due date for next payment");

    // Back Button
    backButton = new QPushButton("Back", this);
    backButton->setToolTip("Return to the previous screen");

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(backButton);

    // Add to content layout
    contentLayout->addWidget(titleLabel);
    contentLayout->addWidget(amountPaidLabel);
    contentLayout->addWidget(amountRemainingLabel);
    contentLayout->addWidget(lastPaymentDateLabel);
    contentLayout->addWidget(nextDueDateLabel);
    contentLayout->addLayout(buttonLayout);

    mainLayout->addWidget(paymentFrame);

    connect(backButton, &QPushButton::clicked, this, &QWidget::close);
}

void PatientDashboardPayment::setupStyles()
{
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
    amountPaidLabel->setStyleSheet(labelStyle);
    amountRemainingLabel->setStyleSheet(labelStyle);
    lastPaymentDateLabel->setStyleSheet(labelStyle);
    nextDueDateLabel->setStyleSheet(labelStyle);

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

void PatientDashboardPayment::fetchPaymentDetails()
{
    QSqlDatabase db = QSqlDatabase::database("hospitalConnection");
    if (!db.isOpen()) {
        qDebug() << "Database connection failed.";
        amountPaidLabel->setText("Amount Paid: Error");
        amountRemainingLabel->setText("Amount Remaining: Error");
        lastPaymentDateLabel->setText("Last Payment Date: Error");
        nextDueDateLabel->setText("Next Due Date: Error");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT amount_paid, amount_remaining, last_payment_date, next_due_date "
                  "FROM payments WHERE patient_id = :patientId LIMIT 1");
    query.bindValue(":patientId", patientId);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError().text();
        amountPaidLabel->setText("Amount Paid: Error");
        amountRemainingLabel->setText("Amount Remaining: Error");
        lastPaymentDateLabel->setText("Last Payment Date: Error");
        nextDueDateLabel->setText("Next Due Date: Error");
        return;
    }

    if (query.next()) {
        double amountPaid = query.value("amount_paid").toDouble();
        double amountRemaining = query.value("amount_remaining").toDouble();
        QString lastPaymentDate = query.value("last_payment_date").toString();
        QString nextDueDate = query.value("next_due_date").toString();

        amountPaidLabel->setText(QString("Amount Paid: $%1").arg(amountPaid, 0, 'f', 2));
        amountRemainingLabel->setText(QString("Amount Remaining: $%1").arg(amountRemaining, 0, 'f', 2));
        lastPaymentDateLabel->setText("Last Payment Date: " + (lastPaymentDate.isEmpty() ? "N/A" : lastPaymentDate));
        nextDueDateLabel->setText("Next Due Date: " + (nextDueDate.isEmpty() ? "N/A" : nextDueDate));
    } else {
        amountPaidLabel->setText("Amount Paid: $0.00");
        amountRemainingLabel->setText("Amount Remaining: $0.00");
        lastPaymentDateLabel->setText("Last Payment Date: N/A");
        nextDueDateLabel->setText("Next Due Date: N/A");
    }
}
