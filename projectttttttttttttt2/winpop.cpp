#include "winpop.h"

WinPop::WinPop(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("background-color: rgba(0, 0, 0, 150);");
    setFixedSize(750,750);
    move(0, 0);

    popupBox = new QWidget(this);
    popupBox->setFixedSize(400, 250);
    popupBox->setStyleSheet("border: 5px solid #804113;background-color: #FFFDD0; border-radius: 20px;");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(15);
    shadow->setOffset(0);
    popupBox->setGraphicsEffect(shadow);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(popupBox, 0, Qt::AlignCenter);

    QVBoxLayout *boxLayout = new QVBoxLayout(popupBox);
    boxLayout->setAlignment(Qt::AlignCenter);

    winnerLabel = new QLabel("PLAYER 1 WINSSSS!!!!!", popupBox);
    winnerLabel->setStyleSheet("border: none; font-size: 20px; font-weight: bold; color: black;");
    winnerLabel->setAlignment(Qt::AlignCenter);
    boxLayout->addWidget(winnerLabel);

    exitButton = new QPushButton(popupBox);
    exitButton->setFixedSize(55, 55);
    exitButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "border-image: url(:/assets/exitButton.png);"
        "}"
        );
    exitButton->setCursor(Qt::PointingHandCursor);

    connect(exitButton, &QPushButton::clicked, this, &WinPop::exitToHome);

    boxLayout->addSpacing(40);
    boxLayout->addWidget(exitButton, 0, Qt::AlignCenter);
}

void WinPop::setWinnerName(const QString &name) {
    winnerLabel->setText(name + " WINSSSS!!!!!");
}
