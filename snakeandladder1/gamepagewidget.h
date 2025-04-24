// gamepagewidget.h
#ifndef GAMEPAGEWIDGET_H
#define GAMEPAGEWIDGET_H

#include <QWidget>
#include <QPixmap>

class GamePageWidget : public QWidget {
    Q_OBJECT
public:
    explicit GamePageWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap background;
};

#endif // GAMEPAGEWIDGET_H

