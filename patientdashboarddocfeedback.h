#ifndef PATIENTDASHBOARDDOCFEEDBACK_H
#define PATIENTDASHBOARDDOCFEEDBACK_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>

class QFrame;
class QPushButton;
class QLabel;
class QComboBox;
class QTextEdit;
class QSpinBox;

class PatientDashboardDocFeedback : public QWidget {
    Q_OBJECT

public:
    explicit PatientDashboardDocFeedback(const QString &patientId, QWidget *parent = nullptr);
    ~PatientDashboardDocFeedback();

private slots:
    void submitFeedback();

private:
    void setupUI();
    void setupStyles();
    void populateDoctors();
    void showConfirmation();

    QString patientId;
    QLabel *titleLabel;
    QLabel *doctorLabel;
    QComboBox *doctorComboBox;
    QLabel *ratingLabel;
    QSpinBox *ratingSpinBox;
    QLabel *commentLabel;
    QTextEdit *commentTextEdit;
    QPushButton *submitButton;
    QPushButton *backButton;
    QFrame *feedbackFrame;
    QLabel *confirmationLabel;
};

#endif // PATIENTDASHBOARDDOCFEEDBACK_H
