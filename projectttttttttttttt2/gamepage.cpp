#include "gamepage.h"
#include "dice.h"
#include "board.h"

#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QRandomGenerator>
#include <QEvent>
#include <QWidget>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QTimer>

GamePage::GamePage(QWidget *parent) : QWidget(parent), currentPlayerIndex(0), totalPlayers(2) {
    setupUI();
    for (int i = 0; i < 4; ++i) {
        QLabel *nameLabel = new QLabel(this);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet("color: white; font-size: 16px;");
        nameLabel->setFixedWidth(50);
        nameLabel->hide();
        playerNameLabels.append(nameLabel);
    }

    diceRollSound.setSource(QUrl::fromLocalFile(":/assets/sounds/roll.wav"));
    snakeBiteSound.setSource(QUrl::fromLocalFile(":/assets/sounds/forsnake.wav"));
    ladderClimbSound.setSource(QUrl::fromLocalFile(":/assets/sounds/bhago.wav"));
    winSound.setSource(QUrl::fromLocalFile(":/assets/sounds/win.wav"));
    yesIs2.setSource(QUrl::fromLocalFile(":/assets/sounds/yesis2.wav"));

    diceRollSound.setVolume(0.5);
    snakeBiteSound.setVolume(0.5);
    ladderClimbSound.setVolume(0.5);
    winSound.setVolume(0.7);
}

QPixmap GamePage::getRoundedPixmap(const QString &path, int size, int radius) {
    QPixmap src(path);
    src = src.scaled(size, size, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPixmap rounded(size, size);
    rounded.fill(Qt::transparent);

    QPainter painter(&rounded);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath pathClip;
    pathClip.addRoundedRect(0, 0, size, size, radius, radius);
    painter.setClipPath(pathClip);
    painter.drawPixmap(0, 0, src);
    return rounded;
}

void GamePage::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QGridLayout *grid = new QGridLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);

    QStringList iconPaths = {
        ":/assets/player1.png", ":/assets/player2.png",
        ":/assets/player3.png", ":/assets/player4.png"
    };
    QStringList nameBackgrounds = {
        ":/assets/textbox1.png", ":/assets/textbox2.png",
        ":/assets/textbox3.png", ":/assets/textbox4.png"
    };

    for (const auto &path : iconPaths) {
        static int i=0;
        QWidget *playerWidget = new QWidget();
        QVBoxLayout *vbox = new QVBoxLayout(playerWidget);
        vbox->setAlignment(Qt::AlignCenter);
        vbox->setContentsMargins(0, 0, 0, 0);

        QLabel *icon = new QLabel();
        icon->setPixmap(QPixmap(iconPaths[i]).scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        icon->setFixedSize(75, 75);
        icon->setAlignment(Qt::AlignCenter);

        QLabel *nameLabel = new QLabel("Player", this);
        nameLabel->setStyleSheet(QString(
                                     "color: white;"
                                     "font-weight: bold;"
                                     "font-size: 13px;"
                                     "background-image: url(%1);"
                                     "background-repeat: no-repeat;"
                                     "background-position: center;"
                                     "background-size: contain;"
                                     "border-radius: 5px;"
                                     "padding: 5px;"
                                     ).arg(nameBackgrounds[i]));
        nameLabel->setAlignment(Qt::AlignCenter);

        if (i < 2) {
            vbox->addWidget(icon);
            vbox->addWidget(nameLabel);
        } else {
            vbox->addWidget(nameLabel);
            vbox->addWidget(icon);
        }

        playerIcons.append(icon);
        playerNameLabels.append(nameLabel);
        playerWidgets.append(playerWidget);
        i++;
    }


    grid->addWidget(playerWidgets[0], 0, 0, Qt::AlignTop | Qt::AlignLeft);
    grid->addWidget(playerWidgets[1], 0, 2, Qt::AlignTop | Qt::AlignRight);
    grid->addWidget(playerWidgets[2], 2, 0, Qt::AlignBottom | Qt::AlignLeft);
    grid->addWidget(playerWidgets[3], 2, 2, Qt::AlignBottom | Qt::AlignRight);


    boardLabel = new QLabel();
    boardLabel->setFixedSize(540, 540);
    boardLabel->setStyleSheet("border: 10px solid #804113; border-radius: 25%;");
    boardLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->addWidget(boardLabel, 0, Qt::AlignCenter);

    diceLabel = new ClickableLabel(this);
    diceLabel->setPixmap(QPixmap(":/assets/1.png").scaled(50, 50));
    diceLabel->setFixedSize(50, 50);
    diceLabel->setScaledContents(true);
    diceLabel->setAlignment(Qt::AlignCenter);
    diceLabel->setCursor(Qt::PointingHandCursor);
    diceLabel->raise();
    diceLabel->show();

    connect(diceLabel, &ClickableLabel::clicked, this, &GamePage::rollDice);

    QStringList tokenPaths = {
        ":/assets/red.png", ":/assets/blue.png",
        ":/assets/green.png", ":/assets/purple.png"
    };

    for (const auto &path : tokenPaths) {
        QLabel *token = new QLabel(boardLabel);
        token->setPixmap(QPixmap(path).scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        token->resize(40, 40);
        token->setScaledContents(true);
        token->setAttribute(Qt::WA_TranslucentBackground);
        token->hide();
        playerTokens.append(token);
    }

    grid->addLayout(centerLayout, 1, 1, Qt::AlignCenter);
    layout->addLayout(grid);

    winPopup = new WinPop(this);
    winPopup->hide();

    connect(winPopup, &WinPop::exitToHome, this, [=]() {
        emit returnToHome();
    });

    installEventFilter(this);

}


void GamePage::startGame(int count, const QString &boardImagePath, const QVector<Player*> &playerList) {
    totalPlayers = count;
    players.clear();
    for (auto p : playerList) {
        players.append(*p);
    }
    currentPlayerIndex = 0;

    boardLabel->setPixmap(getRoundedPixmap(boardImagePath, 540, 50));

    QPoint center = getBoardCoordinates(1);

    for (int i = 0; i < totalPlayers; ++i) {
        playerTokens[i]->move(center.x() - 15 + i * 20, center.y() - 15 + i * 20);
        playerTokens[i]->show();

        QString name = players[i].getName();
        if (name.trimmed().isEmpty()) {
            name = QString("Player %1").arg(i + 1);
            players[i].setName(name);
        }
        playerNameLabels[i]->setText(name);

        playerNameLabels[i]->move(center.x() - 25 + i * 20, center.y() + 20);
        playerNameLabels[i]->show();
    }

    for (int i = totalPlayers; i < playerTokens.size(); ++i) {
        playerTokens[i]->hide();
        playerNameLabels[i]->hide();
    }

    updateDiceImage(1);
    updatePlayerTurnIndicator();
}

void GamePage::rollDice() {
    diceRollSound.play();
    diceLabel->setEnabled(false);

    QTimer *timer = new QTimer(this);
    int rollCount = 10;
    int *currentRoll = new int(0);

    connect(timer, &QTimer::timeout, this, [=]() mutable {
        int fakeRoll = QRandomGenerator::global()->bounded(1, 7);
        updateDiceImage(fakeRoll);
        (*currentRoll)++;
        if (*currentRoll >= rollCount) {
            timer->stop();
            diceRollSound.stop();

            int finalRoll = Dice::roll();
            updateDiceImage(finalRoll);
            if(finalRoll==2){
                yesIs2.play();
            }
            delete currentRoll;

            int currentPos = players[currentPlayerIndex].getPosition();

            if (currentPos + finalRoll <= 100) {
                movePlayer(currentPlayerIndex, finalRoll);
            }

            currentPlayerIndex = (currentPlayerIndex + 1) % totalPlayers;
            updatePlayerTurnIndicator();
            diceLabel->setEnabled(true);
        }
    });

    timer->start(100);
}

void GamePage::updateDiceImage(int value) {
    diceLabel->setPixmap(QPixmap(Dice::getImagePath(value)).scaled(70, 70));
}


void GamePage::movePlayer(int index, int steps) {
    int startPos = players[index].getPosition();
    int endPos = startPos + steps;

    if (endPos > 100)
        endPos = 100;

    QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);
    for (int pos = startPos + 1; pos <= endPos; ++pos) {
        QPoint point = getBoardCoordinates(pos);
        int tokenSize = playerTokens[index]->width();
        point.rx() -= tokenSize / 2;
        point.ry() -= tokenSize / 2;

        int offset = 5;
        point.rx() += (index % 2) * offset;
        point.ry() += (index / 2) * offset;

        QPropertyAnimation *stepAnim = new QPropertyAnimation(playerTokens[index], "pos");
        stepAnim->setDuration(200);
        stepAnim->setEndValue(point);
        stepAnim->setEasingCurve(QEasingCurve::Linear);
        group->addAnimation(stepAnim);
    }

    connect(group, &QSequentialAnimationGroup::finished, this, [=]() {

        players[index].setPosition(endPos);

        if (endPos == 100) {
            players[index].setPosition(endPos);
            updatePlayerToken(index);
            winSound.play();
            winPopup->setWinnerName(players[currentPlayerIndex].getName());
            winPopup->show();
            diceLabel->setEnabled(false);
            return;
        }

        int finalPos = Board::getNewPosition(endPos);
        if (finalPos != endPos) {
            if (finalPos < endPos) {
                snakeBiteSound.play();
            } else {
                ladderClimbSound.play();
            }
            players[index].setPosition(finalPos);

            QPoint specialPoint = getBoardCoordinates(finalPos);
            int tokenSize = playerTokens[index]->width();
            specialPoint.rx() -= tokenSize / 2;
            specialPoint.ry() -= tokenSize / 2;
            int offset = 5;
            specialPoint.rx() += (index % 2) * offset;
            specialPoint.ry() += (index / 2) * offset;

            QPropertyAnimation *specialAnim = new QPropertyAnimation(playerTokens[index], "pos");
            specialAnim->setDuration(500);
            specialAnim->setEndValue(specialPoint);
            specialAnim->setEasingCurve(QEasingCurve::OutBounce);
            specialAnim->start(QAbstractAnimation::DeleteWhenStopped);
        }

    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}


void GamePage::updatePlayerToken(int index) {
    QPoint pt = getBoardCoordinates(players[index].getPosition());
    int tokenSize = playerTokens[index]->width();
    pt.rx() -= tokenSize / 2;
    pt.ry() -= tokenSize / 2;

    int offset = 5;
    pt.rx() += (index % 2) * offset;
    pt.ry() += (index / 2) * offset;

    playerTokens[index]->move(pt);
}

void GamePage::updatePlayerTurnIndicator() {
    for (int i = 0; i < playerIcons.size(); ++i) {
        if (i == currentPlayerIndex) {
            playerIcons[i]->setStyleSheet("border: 3px solid yellow; border-radius: 10px;");
        } else {
            playerIcons[i]->setStyleSheet("border: 2px solid transparent;");
        }
    }
}

QPoint GamePage::getBoardCoordinates(int position) {
    return Board::getBoardCoordinates(position, boardLabel->width());
}

bool GamePage::eventFilter(QObject *watched, QEvent *event) {
    if (watched == this && event->type() == QEvent::Resize) {
        if (diceLabel && boardLabel) {
            int x = (width() - diceLabel->width()) / 2;
            int y = boardLabel->y() + boardLabel->height() + 10;
            diceLabel->move(x, y);
        }
    }
    return QWidget::eventFilter(watched, event);
}

void GamePage::resetGame() {
    diceLabel->setEnabled(true);
    currentPlayerIndex = 0;
    for (Player &player : players) {
        player.setPosition(1);
    }
    winPopup->hide();
}
