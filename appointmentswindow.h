#ifndef APPOINTMENTSWINDOW_H
#define APPOINTMENTSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSqlDatabase>
#include <QLabel>
#include <QDialog>

class DoctorProfileDialog : public QDialog
{
    Q_OBJECT
public:
    DoctorProfileDialog(const QString &doctorId, QWidget *parent = nullptr);
private:
    void setupUI();
    void loadDoctorData(const QString &doctorId, QGridLayout *layout);
};

class AppointmentsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit AppointmentsWindow(const QString &patientId, QWidget *parent = nullptr);
private:
    void setupUI();
    void loadAppointments();
    QString patientId;
    QTableWidget *appointmentsTable;

private slots:
    void handleBackButtonClick();
};

#endif // APPOINTMENTSWINDOW_H
