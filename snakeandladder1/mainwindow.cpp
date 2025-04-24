#include "mainwindow.h"
#include <QPainter>
#include <QPixmap>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QGridLayout>
#include <QRandomGenerator>
#include<QPaintEvent>
#include <QResizeEvent>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentPlayerIndex(0), totalPlayers(2)
{
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    resize(800, 800);
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QStackedWidget *stack = new QStackedWidget(this);
    homePage = new QWidget();
    gamePage = new QWidget();
    stack->addWidget(homePage);
    stack->addWidget(gamePage);

    setupHomePage();
    setupGamePage();

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->addWidget(stack);

    connect(startButton, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(gamePage);
        startGame();
    });
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap background(":/assets/background1.png");
    painter.drawPixmap(0, 0, width(), height(), background);
}

void MainWindow::setupHomePage() {
    QVBoxLayout *layout = new QVBoxLayout(homePage);

    QLabel *title = new QLabel("Snake and Ladder", homePage);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");

    playerSlider = new QSlider(Qt::Horizontal);
    playerSlider->setMinimum(2);
    playerSlider->setMaximum(4);
    playerSlider->setValue(2);

    playerCountLabel = new QLabel("Players: 2");
    playerCountLabel->setStyleSheet("color: white;");

    startButton = new QPushButton("Start Game");

    layout->addWidget(title);
    layout->addWidget(playerSlider);
    layout->addWidget(playerCountLabel);
    layout->addWidget(startButton);

    connect(playerSlider, &QSlider::valueChanged, this, &MainWindow::updatePlayerCount);
}

void MainWindow::setupGamePage() {
    QVBoxLayout *layout = new QVBoxLayout(gamePage);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QGridLayout *grid = new QGridLayout();
    grid->setContentsMargins(0, 0, 0, 0);
    grid->setSpacing(0);

    QStringList iconPaths = {
        ":/assets/player1.png",
        ":/assets/player2.png",
        ":/assets/player3.png",
        ":/assets/player4.png"
    };

    for (int i = 0; i < 4; ++i) {
        QLabel *icon = new QLabel();
        icon->setPixmap(QPixmap(iconPaths[i]).scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        icon->setFixedSize(60, 60);
        icon->setStyleSheet("border: 2px solid transparent;");
        playerIcons.append(icon);
    }

    grid->addWidget(playerIcons[0], 0, 0, Qt::AlignTop | Qt::AlignLeft);
    grid->addWidget(playerIcons[1], 0, 2, Qt::AlignTop | Qt::AlignRight);
    grid->addWidget(playerIcons[2], 2, 0, Qt::AlignBottom | Qt::AlignLeft);
    grid->addWidget(playerIcons[3], 2, 2, Qt::AlignBottom | Qt::AlignRight);

    boardLabel = new QLabel();
    boardLabel->setPixmap(QPixmap(":/assets/board.png").scaled(500, 500, Qt::KeepAspectRatio));
    boardLabel->setFixedSize(500, 500);
    boardLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout *centerLayout = new QVBoxLayout();
    centerLayout->addWidget(boardLabel, 0, Qt::AlignCenter);

    diceLabel = new ClickableLabel(gamePage);
    diceLabel->setFixedSize(100, 100);
    diceLabel->setPixmap(QPixmap(":/assets/1.png").scaled(100, 100));
    diceLabel->setAlignment(Qt::AlignCenter);
    diceLabel->setCursor(Qt::PointingHandCursor);
    diceLabel->raise();  // Make sure it's on top
    diceLabel->show();

    grid->addLayout(centerLayout, 1, 1, Qt::AlignCenter);
    layout->addLayout(grid);

    playerTokens = {
        new QLabel(boardLabel), new QLabel(boardLabel),
        new QLabel(boardLabel), new QLabel(boardLabel)
    };

    QStringList playerImagePaths = {
        ":/assets/red.png",
        ":/assets/blue.png",
        ":/assets/green.png",
        ":/assets/purple.png"
    };

    for (int i = 0; i < playerTokens.size(); ++i) {
        QLabel* token = playerTokens[i];
        token->setPixmap(QPixmap(playerImagePaths[i]).scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        token->resize(30, 30);
        token->setAttribute(Qt::WA_TranslucentBackground);
        token->hide();
    }

    connect(diceLabel, &ClickableLabel::clicked, this, &MainWindow::rollDice);
    gamePage->installEventFilter(this);


}

void MainWindow::updatePlayerCount(int count) {
    totalPlayers = count;
    playerCountLabel->setText(QString("Players: %1").arg(count));
}

void MainWindow::startGame() {
    playerPositions = QVector<int>(totalPlayers, 1);
    currentPlayerIndex = 0;

    for (int i = 0; i < totalPlayers; ++i) {
        playerTokens[i]->move(getBoardCoordinates(1));
        playerTokens[i]->show();
    }
    for (int i = totalPlayers; i < playerTokens.size(); ++i) {
        playerTokens[i]->hide();
    }

    updateDiceImage(1);
    updatePlayerTurnIndicator();
}

void MainWindow::rollDice() {
    int dice = QRandomGenerator::global()->bounded(1, 7);
    updateDiceImage(dice);
    movePlayer(currentPlayerIndex, dice);

    if (playerPositions[currentPlayerIndex] >= 100) {
        diceLabel->setEnabled(false);
        diceLabel->setToolTip(QString("Player %1 wins!").arg(currentPlayerIndex + 1));
    }

    currentPlayerIndex = (currentPlayerIndex + 1) % totalPlayers;
    updatePlayerTurnIndicator();
}

void MainWindow::updateDiceImage(int value) {
    QString path = QString(":/assets/1-6/%1.png").arg(value);
    diceLabel->setPixmap(QPixmap(path).scaled(100, 100));
}

void MainWindow::movePlayer(int index, int steps) {
    int &pos = playerPositions[index];
    pos += steps;
    if (pos > 100) pos = 100;
    checkSnakesAndLadders(pos);
    updatePlayerToken(index);
}

void MainWindow::updatePlayerToken(int index) {
    QPoint pt = getBoardCoordinates(playerPositions[index]);
    int tokenSize = playerTokens[index]->width();
    pt.rx() -= tokenSize / 2;
    pt.ry() -= tokenSize / 2;

    int offset = 5;
    pt.rx() += (index % 2) * offset;
    pt.ry() += (index / 2) * offset;

    playerTokens[index]->move(pt);
}


void MainWindow::checkSnakesAndLadders(int &pos) {
    QMap<int, int> jumps = {
        {2, 23}, {17, 93}, {8, 12}, {32, 51}, {70, 89}, {75, 96},{39,80},{62,78},{29,54},
        {31, 14}, {41, 20}, {67, 50}, {99, 4}, {59, 37}, {92, 76},{82,61}
    };
    if (jumps.contains(pos)) pos = jumps[pos];
}

QPoint MainWindow::getBoardCoordinates(int position) {
    int row = (position - 1) / 10;
    int col = (row % 2 == 0) ? (position - 1) % 10 : 9 - ((position - 1) % 10);
    int tileSize = boardLabel->width() / 10;
    int x = col * tileSize + tileSize / 2;
    int y = (9 - row) * tileSize + tileSize / 2;
    return QPoint(x, y);
}


void MainWindow::updatePlayerTurnIndicator() {
    for (int i = 0; i < playerIcons.size(); ++i) {
        if (i == currentPlayerIndex) {
            playerIcons[i]->setStyleSheet("border: 3px solid yellow; border-radius: 10px;");
        } else {
            playerIcons[i]->setStyleSheet("border: 2px solid transparent;");
        }
    }
}
bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == gamePage && event->type() == QEvent::Resize) {
        if (diceLabel && boardLabel) {
            int x = (gamePage->width() - diceLabel->width()) / 2;
            int y = boardLabel->y() + boardLabel->height() + 10;
            diceLabel->move(x, y);
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

