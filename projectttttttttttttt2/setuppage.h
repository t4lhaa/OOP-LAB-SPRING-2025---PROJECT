#ifndef SETUPPAGE_H
#define SETUPPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QVector>

class SetupPage : public QWidget {
    Q_OBJECT

public:
    explicit SetupPage(QWidget *parent = nullptr);
    QVector<QString> getPlayerNames() const;
    QString getSelectedBoardPath() const;

    void setTotalPlayers(int count);
    int getSelectedBoardIndex() const;

public slots:
    void reset();

signals:
    void setupComplete();
    void goToHomePage();

private slots:
    void moveLeft();
    void moveRight();
    void continueClicked();

private:
    int totalPlayers;
    void updateBoardImage();

    QVector<QLineEdit*> nameEdits;
    QVector<QLabel*> tokenIcons;
    QLabel *boardImage;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *continueButton;
    QPushButton *backToHomeButton;

    int currentBoardIndex;
    QVector<QString> boardPaths;
};

#endif
