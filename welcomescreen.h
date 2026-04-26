#ifndef WELCOMESCREEN_H
#define WELCOMESCREEN_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>
#include <QPalette>

class WelcomeScreen : public QDialog
{
    Q_OBJECT

public:
    explicit WelcomeScreen(QWidget *parent = nullptr);

private slots:
    void handleContinue();

private:
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QPushButton *continueButton;

    void setupUI();
    void setupStyles();
};

#endif // WELCOMESCREEN_H
