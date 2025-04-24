// gamepagewidget.cpp
#include "gamepagewidget.h"
#include <QPainter>
#include<QWidget>

GamePageWidget::GamePageWidget(QWidget *parent)
    : QWidget(parent), background(":/assets/background.png") {}

void GamePageWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(rect(), background);
}
