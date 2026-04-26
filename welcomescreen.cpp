#include "welcomescreen.h"
#include <QApplication>

WelcomeScreen::WelcomeScreen(QWidget *parent) : QDialog(parent)
{
    // Set to maximized window (with title bar)
    setWindowState(Qt::WindowMaximized);
    setWindowTitle("NED Hospital System");

    setupUI();
    setupStyles();
}

void WelcomeScreen::setupUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(50, 50, 50, 50);

    // Title label
    titleLabel = new QLabel("NED HOSPITAL SYSTEM", this);
    titleLabel->setAlignment(Qt::AlignCenter);

    // Subtitle label
    subtitleLabel = new QLabel("Management System", this);
    subtitleLabel->setAlignment(Qt::AlignCenter);

    // Continue button
    continueButton = new QPushButton("ENTER SYSTEM", this);
    continueButton->setMinimumHeight(60);
    continueButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // Add widgets with stretch for centering
    mainLayout->addStretch(1);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addStretch(1);
    mainLayout->addWidget(continueButton, 0, Qt::AlignCenter);
    mainLayout->addStretch(1);

    connect(continueButton, &QPushButton::clicked, this, &WelcomeScreen::handleContinue);
}

void WelcomeScreen::setupStyles()
{
    // Background color
    QPalette pal = palette();
    pal.setColor(QPalette::Window, QColor(240, 240, 240));  // Light gray
    setAutoFillBackground(true);
    setPalette(pal);

    // Title style
    QFont titleFont("Arial", 36, QFont::Bold);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #003366;");  // Dark blue

    // Subtitle style
    QFont subtitleFont("Arial", 24, QFont::Normal);
    subtitleLabel->setFont(subtitleFont);
    subtitleLabel->setStyleSheet("color: #555555;");

    // Button style
    continueButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #0066CC;"
        "   color: white;"
        "   border: 2px solid #004488;"
        "   border-radius: 10px;"
        "   padding: 15px 30px;"
        "   font-size: 18px;"
        "   min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0088FF;"
        "}"
        );
}

void WelcomeScreen::handleContinue()
{
    accept();
}
