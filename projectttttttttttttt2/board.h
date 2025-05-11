#ifndef BOARD_H
#define BOARD_H

#include <QPoint>

class Board {
public:
    enum BoardType { Easy, Medium, Difficult };
    static void setBoardType(BoardType type);
    static int applySnakesAndLadders(int position);
    static QPoint getBoardCoordinates(int position, int boardWidth);
    static int getNewPosition(int position);

private:
    static BoardType currentBoardType;
};

#endif
