#include "player.h"

Player::Player() : position(1), playerName("Unnamed") {}

Player::Player(const QString &name) : position(1), playerName(name) {}

int Player::getPosition() const {
    return position;
}

void Player::setPosition(int position) {
    this->position = position;
}

void Player::move(int steps) {
    if (position + steps <= 100) {
        position += steps;
    }
}

QString Player::getName() const {
    return playerName;
}

void Player::setName(const QString &name) {
    playerName = name;
}
