#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player {
public:
    Player();
    Player(const QString &name = "");

    int getPosition() const;
    void setPosition(int position);
    void move(int steps);

    QString getName() const;
    void setName(const QString &name);

private:
    int position;
    QString playerName;
};

#endif
