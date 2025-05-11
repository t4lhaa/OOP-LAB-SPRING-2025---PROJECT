#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSoundEffect>
#include "clickablelabel.h"
#include "player.h"
#include "winpop.h"


class GamePage : public QWidget {
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);

    void startGame(int count, const QString &boardImagePath, const QVector<Player*> &playerList);
    void resetGame();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void returnToHome();

private slots:
    void rollDice();

private:
    void setupUI();
    void updateDiceImage(int value);
    void movePlayer(int index, int steps);
    void updatePlayerToken(int index);
    void updatePlayerTurnIndicator();
    QPoint getBoardCoordinates(int position);


    WinPop *winPopup;

    QLabel *boardLabel;
    ClickableLabel *diceLabel;
    QVector<QWidget*> playerWidgets;
    QVector<QLabel*> playerTokens;
    QVector<QLabel*> playerIcons;
    QVector<QLabel*> playerNameLabels;
    QPixmap getRoundedPixmap(const QString &path, int size, int radius);

    QVector<Player> players;
    int currentPlayerIndex;
    int totalPlayers;

    QSoundEffect diceRollSound;
    QSoundEffect snakeBiteSound;
    QSoundEffect ladderClimbSound;
    QSoundEffect winSound;
    QSoundEffect yesIs2;
};

#endif
