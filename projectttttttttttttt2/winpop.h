#ifndef WINPOP_H
#define WINPOP_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

class WinPop : public QWidget {
    Q_OBJECT

public:
    explicit WinPop(QWidget *parent = nullptr);
    void setWinnerName(const QString &name);

signals:
    void exitToHome();

private:
    QWidget *popupBox;
    QLabel *winnerLabel;
    QPushButton *exitButton;
};

#endif
