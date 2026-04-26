#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFont>
#include "doctorlogindialog.h"
#include "patientlogindialog.h"
#include "adminlogindialog.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleDoctorClick();
    void handlePatientClick();
    void handleAdminClick();

private:
    void setupUI();
    void setupStyles();

    // UI Elements
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QPushButton *doctorBtn;
    QPushButton *patientBtn;
    QPushButton *adminBtn;
};

#endif // MAINWINDOW_H
