#ifndef PATIENTDASHBOARDFEEDBACK_H
#define PATIENTDASHBOARDFEEDBACK_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>

class QFrame;
class QPushButton;
class QLabel;
class QTextEdit;
class QSpinBox;

class PatientDashboardFeedback : public QWidget {
    Q_OBJECT

public:
    explicit PatientDashboardFeedback(const QString &patientId, QWidget *parent = nullptr);
    ~PatientDashboardFeedback();

private slots:
    void submitFeedback();

private:
    void setupUI();
    void setupStyles();
    void showConfirmation();

    QString patientId;
    QLabel *titleLabel;
    QLabel *ratingLabel;
    QSpinBox *ratingSpinBox;
    QLabel *commentLabel;
    QTextEdit *commentTextEdit;
    QPushButton *submitButton;
    QPushButton *backButton;
    QFrame *feedbackFrame;
    QLabel *confirmationLabel;
};

#endif // PATIENTDASHBOARDFEEDBACK_H
