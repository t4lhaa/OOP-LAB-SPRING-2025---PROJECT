#include "board.h"
#include <QMap>

Board::BoardType Board::currentBoardType = Board::Easy;

void Board::setBoardType(BoardType type) {
    Board::currentBoardType = type;
}

int Board::applySnakesAndLadders(int position) {
    static QMap<int, int> easy = {
        {5, 58}, {14, 49}, {42, 60}, {53, 72}, {64, 83}, {75, 94},
        {38, 20}, {45, 7}, {51, 10}, {65, 54}, {91, 73}, {97, 61}
    };

    static QMap<int, int> medium = {
        {2, 23}, {17, 93}, {8, 12}, {32, 51}, {70, 89}, {75, 96}, {39, 80}, {62, 78}, {29, 54},
        {31, 14}, {41, 20}, {67, 50}, {99, 4}, {59, 37}, {92, 76}, {82, 61}
    };

    static QMap<int, int> difficult = {
        {3, 20}, {6, 14}, {11, 28}, {15, 34}, {17, 74}, {22, 37}, {38, 59}, {49, 67}, {57, 76}, {61, 78}, {73, 86}, {81, 98}, {88, 91},
        {8, 4}, {18, 1}, {26, 10}, {39, 5}, {51, 6}, {54, 36}, {56, 1}, {60, 23}, {75, 28}, {83, 45}, {85, 59}, {90, 48}, {92, 25}, {97, 87}, {99, 63}
    };

    switch (currentBoardType) {
    case Easy:
        return easy.value(position, position);
    case Medium:
        return medium.value(position, position);
    case Difficult:
        return difficult.value(position, position);
    }
}

QPoint Board::getBoardCoordinates(int position, int boardWidth) {
    int row = (position - 1) / 10;
    int col = (row % 2 == 0) ? (position - 1) % 10 : 9 - ((position - 1) % 10);
    int tileSize = boardWidth / 10;
    int x = col * tileSize + tileSize / 2;
    int y = (9 - row) * tileSize + tileSize / 2;
    return QPoint(x, y);
}
int Board::getNewPosition(int position) {
    return applySnakesAndLadders(position);
}

