#include "homepage.h"
#include "clickablelabel.h"

HomePage::HomePage(QWidget *parent) : QWidget(parent) {
    this->setFixedSize(750, 750);

    logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap(":/assets/logo.png").scaled(275, 275, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setFixedSize(275, 275);
    logoLabel->move(235, 60);
    logoLabel->setAlignment(Qt::AlignCenter);

    twoPlayersLabel = new ClickableLabel(this);
    threePlayersLabel = new ClickableLabel(this);
    fourPlayersLabel = new ClickableLabel(this);

    twoPlayersLabel->setFixedSize(95, 95);
    threePlayersLabel->setFixedSize(95, 95);
    fourPlayersLabel->setFixedSize(95, 95);

    twoPlayersLabel->setPixmap(QPixmap(":/assets/2playericon.png").scaled(85, 85, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    threePlayersLabel->setPixmap(QPixmap(":/assets/3playericon.png").scaled(85, 85, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    fourPlayersLabel->setPixmap(QPixmap(":/assets/4playericon.png").scaled(85, 85, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    twoPlayersLabel->move(170, 320);
    threePlayersLabel->move(335, 320);
    fourPlayersLabel->move(490, 320);

    twoPlayersLabel->setCursor(Qt::PointingHandCursor);
    threePlayersLabel->setCursor(Qt::PointingHandCursor);
    fourPlayersLabel->setCursor(Qt::PointingHandCursor);

    playerCountLabel = new QLabel("Select Number of Players",this);
    playerCountLabel->setStyleSheet("color: black; font-size: 15px;font-weight:bold");
    playerCountLabel->setFixedWidth(400);
    playerCountLabel->setWordWrap(true);
    playerCountLabel->setAlignment(Qt::AlignCenter);
    playerCountLabel->adjustSize();  // Resize to fit text
    playerCountLabel->move(180, 430);


    selectedPlayerCount = 0;

    auto highlightSelection = [=](ClickableLabel* selected) {
        twoPlayersLabel->setStyleSheet("");
        threePlayersLabel->setStyleSheet("");
        fourPlayersLabel->setStyleSheet("");
        selected->setStyleSheet(
            "border: 3px solid black;"
            "border-radius: 10px;"
            "padding-left:3px"
            );
    };

    connect(twoPlayersLabel, &ClickableLabel::clicked, this, [=]() {
        selectedPlayerCount = 2;
        playerCountLabel->setText("2 Players");
        highlightSelection(twoPlayersLabel);
    });

    connect(threePlayersLabel, &ClickableLabel::clicked, this, [=]() {
        selectedPlayerCount = 3;
        playerCountLabel->setText("3 Players");
        highlightSelection(threePlayersLabel);
    });

    connect(fourPlayersLabel, &ClickableLabel::clicked, this, [=]() {
        selectedPlayerCount = 4;
        playerCountLabel->setText("4 Players");
        highlightSelection(fourPlayersLabel);
    });


    nextButton = new QPushButton(this);
    nextButton->setFixedSize(180, 60);
    nextButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "border-image: url(:/assets/next.png);"
        "}"
        "QPushButton:hover {"
        "border-image: url(:/assets/next_hover.png);"
        "}"
        );
    nextButton->move(290, 510);
    nextButton->setCursor(Qt::PointingHandCursor);

    exitButton = new QPushButton(this);
    exitButton->setFixedSize(170, 57);
    exitButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "border-image: url(:/assets/exit.png);"
        "}"
        "QPushButton:hover {"
        "border-image: url(:/assets/exit_hover.png);"
        "}"
        );
    exitButton->move(295, 565);
    exitButton->setCursor(Qt::PointingHandCursor);

}

QPushButton* HomePage::getNextButton() const {
    return nextButton;
}

QPushButton* HomePage::getExitButton() const {
    return exitButton;
}

QLabel* HomePage::getPlayerCountLabel() const {
    return playerCountLabel;
}

int HomePage::getSelectedPlayerCount() const {
    return selectedPlayerCount;
}
