#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include <QSoundEffect>
#include <QLabel>
#include "clickablelabel.h"

class HomePage : public QWidget {
    Q_OBJECT

public:
    explicit HomePage(QWidget *parent = nullptr);

    QPushButton *getNextButton() const;
    QPushButton *getExitButton() const;
    QLabel *getPlayerCountLabel() const;
    int getSelectedPlayerCount() const;  // <-- NEW getter for selected count

private:
    QLabel *logoLabel;
    QPushButton *nextButton;
    QPushButton *exitButton;
    QLabel *playerCountLabel;

    int selectedPlayerCount;  // default

    ClickableLabel* twoPlayersLabel;
    ClickableLabel* threePlayersLabel;
    ClickableLabel* fourPlayersLabel;

};
#endif
