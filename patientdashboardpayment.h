#ifndef PATIENTDASHBOARDPAYMENT_H
#define PATIENTDASHBOARDPAYMENT_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>

class QFrame;
class QPushButton;
class QLabel;

class PatientDashboardPayment : public QWidget {
    Q_OBJECT

public:
    explicit PatientDashboardPayment(const QString &patientId, QWidget *parent = nullptr);
    ~PatientDashboardPayment();

private:
    void setupUI();
    void setupStyles();
    void fetchPaymentDetails();

    QString patientId;
    QLabel *titleLabel;
    QLabel *amountPaidLabel;
    QLabel *amountRemainingLabel;
    QLabel *lastPaymentDateLabel;
    QLabel *nextDueDateLabel;
    QFrame *paymentFrame;
    QPushButton *backButton;
};

#endif // PATIENTDASHBOARDPAYMENT_H
