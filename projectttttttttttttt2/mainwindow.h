#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "homepage.h"
#include "setuppage.h"
#include "gamepage.h"
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setupUI();
    void updatePlayerCount(int count);

    HomePage *homePage;
    SetupPage *setupPage;
    GamePage *gamePage;
    QStackedWidget *stack;

    int totalPlayers;
    QVector<Player*> players;
};

#endif
