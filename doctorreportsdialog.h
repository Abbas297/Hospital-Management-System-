#ifndef DOCTORREPORTSDIALOG_H
#define DOCTORREPORTSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DoctorReportsDialog : public QDialog
{
    Q_OBJECT

public:
    DoctorReportsDialog(const QString &doctorId, QWidget *parent = nullptr);
    ~DoctorReportsDialog();

private:
    void setupUi();
    void checkReports();

    QString m_doctorId;
    QVBoxLayout *mainLayout;
    QLabel *headerLabel;
    QLabel *statusLabel;
    QPushButton *closeButton;
};

#endif // DOCTORREPORTSDIALOG_H
