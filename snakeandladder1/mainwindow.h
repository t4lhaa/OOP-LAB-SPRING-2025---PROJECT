#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QEvent>
#include <QSlider>
#include <QVector>
#include<QPaintEvent>
#include <QWidget>
#include <QResizeEvent>
#include "clickablelabel.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;


private slots:
    void startGame();
    void rollDice();
    void updatePlayerCount(int count);

private:
    QWidget *homePage;
    QWidget *gamePage;

    QVector<QLabel*> playerIcons;

    QSlider *playerSlider;
    QLabel *playerCountLabel;
    QPushButton *startButton;
    ClickableLabel *diceLabel;
    QLabel *boardLabel;

    QVector<QLabel*> playerTokens;
    QVector<int> playerPositions;
    int currentPlayerIndex;
    int totalPlayers;

    void setupUI();
    void setupHomePage();
    void setupGamePage();
    void initializeGame();
    void paintEvent(QPaintEvent *event);
    void movePlayer(int index, int steps);
    void updatePlayerToken(int index);
    void updateDiceImage(int value);
    void updatePlayerTurnIndicator();
    void checkSnakesAndLadders(int &position);
    QPoint getBoardCoordinates(int position);
};

#endif
