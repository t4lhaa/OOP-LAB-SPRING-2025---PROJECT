#include "mainwindow.h"
#include "board.h"
#include <QPainter>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), totalPlayers(2) {
    setupUI();
    setFixedSize(750, 750);
}

MainWindow::~MainWindow() {
    qDeleteAll(players);
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QPixmap background(":/assets/background1.png");
    painter.drawPixmap(0, 0, width(), height(), background);
}

void MainWindow::setupUI() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    homePage = new HomePage();
    setupPage = new SetupPage();
    gamePage = new GamePage();

    stack = new QStackedWidget(this);
    stack->addWidget(homePage);
    stack->addWidget(setupPage);
    stack->addWidget(gamePage);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->addWidget(stack);

    connect(homePage->getNextButton(), &QPushButton::clicked, this, [=]() {
        totalPlayers = homePage->getSelectedPlayerCount();
        if (totalPlayers == 0) {
            homePage->getPlayerCountLabel()->setText("Please select number of players");
            return;
        }
        setupPage->setTotalPlayers(totalPlayers);
        stack->setCurrentWidget(setupPage);
    });


    connect(setupPage, &SetupPage::setupComplete, this, [=]() {
        qDeleteAll(players);
        players.clear();

        for (const QString &name : setupPage->getPlayerNames()) {
            Player *player = new Player(name);
            players.append(player);
        }

        stack->setCurrentWidget(gamePage);
        Board::setBoardType(static_cast<Board::BoardType>(setupPage->getSelectedBoardIndex()));
        gamePage->startGame(totalPlayers, setupPage->getSelectedBoardPath(), players);
    });

    connect(setupPage, &SetupPage::goToHomePage, this, [=]() {
        stack->setCurrentWidget(homePage);
    });


    connect(homePage->getExitButton(), &QPushButton::clicked, this, &QMainWindow::close);

    connect(gamePage, &GamePage::returnToHome, this, [=]() {
        stack->setCurrentWidget(homePage);
        setupPage->reset();
        // qDeleteAll(players);
        // players.clear();
        gamePage->resetGame();
    });

}
