#include "setuppage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

SetupPage::SetupPage(QWidget *parent) : QWidget(parent), totalPlayers(4),currentBoardIndex(0) {
    leftButton = new QPushButton(this);
    leftButton->setFixedSize(45, 45);
    leftButton->setStyleSheet("border-image: url(:/assets/backward.png);");
    leftButton->move(235, 185);

    rightButton = new QPushButton(this);
    rightButton->setFixedSize(45, 45);
    rightButton->setStyleSheet("border-image: url(:/assets/forward.png);");
    rightButton->move(450, 185);

    boardImage = new QLabel(this);
    boardImage->setFixedSize(170, 170);
    boardImage->setScaledContents(true);
    boardImage->setStyleSheet("border: 5px solid #804113;");
    boardImage->setAlignment(Qt::AlignCenter);
    boardImage->move(280, 120);

    connect(leftButton, &QPushButton::clicked, this, &SetupPage::moveLeft);
    connect(rightButton, &QPushButton::clicked, this, &SetupPage::moveRight);

    boardPaths = {
        ":/assets/board1.png",
        ":/assets/board2.png",
        ":/assets/board3.png"
    };
    updateBoardImage();

    QStringList tokenImages = {
        ":/assets/red.png", ":/assets/blue.png",
        ":/assets/green.png", ":/assets/purple.png"
    };

    for (int i = 0; i < 4; ++i) {
        QLabel *token = new QLabel(this);
        token->setPixmap(QPixmap(tokenImages[i]).scaled(25, 25, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        token->setFixedSize(25, 25);
        token->move(210, 330 + i * 50);

        QLineEdit *edit = new QLineEdit(this);
        edit->setPlaceholderText(QString("Player %1 Name").arg(i + 1));
        edit->setFixedSize(300,40);
        edit->move(235, 322 + i * 50);

        QString backgroundPath = ":/assets/textbox" + QString::number(i+1) + ".png";  // Different background for each

        edit->setStyleSheet(
            QString("QLineEdit {"
                    "border-image: url(%1);"
                    // "background-repeat: no-repeat;"
                    // "background-position: center;"
                    // "background-size: cover;"
                    "border: 2px solid #804113;"
                    "border-radius: 10px;"
                    "font-size: 14px;"
                    "font-weight: bold;"
                    "color: white;"
                    // "padding-left: 83px;"
                    // "padding-left: 120px;"
                    "padding-bottom: 5px"
                    "}").arg(backgroundPath)
        );
        edit->setAlignment(Qt::AlignCenter);


        tokenIcons.append(token);
        nameEdits.append(edit);
    }

    continueButton = new QPushButton(this);
    continueButton->setFixedSize(175, 50);
    continueButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "border-image: url(:/assets/start.png);"
        "}"
        "QPushButton:hover {"
        "border-image: url(:/assets/start_hover.png);"
        "}"
        );
    continueButton->move(290, 525); // position of continue button

    connect(continueButton, &QPushButton::clicked, this, &SetupPage::continueClicked);

    backToHomeButton = new QPushButton(this);
    backToHomeButton->setFixedSize(170, 60);
    // backToHomeButton->setStyleSheet("border-image: url(:/assets/back.png);");
    backToHomeButton->setStyleSheet(
        "QPushButton {"
        "border: none;"
        "border-image: url(:/assets/back.png);"
        "}"
        "QPushButton:hover {"
        "border-image: url(:/assets/back_hover.png);"
        "}"
        );
    backToHomeButton->move(290, 570);

    connect(backToHomeButton, &QPushButton::clicked, this, [=]() {
        emit goToHomePage();
    });



}

void SetupPage::setTotalPlayers(int count) {
    totalPlayers = count;

    for (int i = 0; i < tokenIcons.size(); ++i) {
        if (i < totalPlayers) {
            tokenIcons[i]->show();
            nameEdits[i]->show();
        } else {
            tokenIcons[i]->hide();
            nameEdits[i]->hide();
        }
    }
}


void SetupPage::moveLeft() {
    currentBoardIndex = (currentBoardIndex - 1 + boardPaths.size()) % boardPaths.size();
    updateBoardImage();
}

void SetupPage::moveRight() {
    currentBoardIndex = (currentBoardIndex + 1) % boardPaths.size();
    updateBoardImage();
}

void SetupPage::updateBoardImage() {
    boardImage->setPixmap(QPixmap(boardPaths[currentBoardIndex]).scaled(300, 300, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void SetupPage::continueClicked() {
    emit setupComplete();
}

QVector<QString> SetupPage::getPlayerNames() const {
    QVector<QString> names;
    for (auto edit : nameEdits) {
        names.append(edit->text());
    }
    return names;
}

QString SetupPage::getSelectedBoardPath() const {
    return boardPaths[currentBoardIndex];
}

int SetupPage::getSelectedBoardIndex() const {
    return currentBoardIndex;
}

void SetupPage::reset() {
    for (int i = 0; i < 4; ++i) {
        nameEdits[i]->clear();
    }
}
