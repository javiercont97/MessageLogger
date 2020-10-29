#include "indicator.h"
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>

Indicator::Indicator(QWidget *parent): QWidget(parent) {
    status = IndicatorStatus::DISCONNECTED;
}

void Indicator::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    switch (status) {
    case IndicatorStatus::DISCONNECTED:
        painter.setBrush(Qt::red);
        break;
    case IndicatorStatus::READY:
        painter.setBrush(Qt::yellow);
        break;
    case IndicatorStatus::LOGGING:
        painter.setBrush(Qt::green);
        break;
    }

    painter.translate(width()/2, height()/2);
    painter.drawEllipse(QRectF(-5, -5, 10, 10));
}

IndicatorStatus Indicator::getStatus() const {
    return status;
}

void Indicator::setStatus(const IndicatorStatus &value) {
    status = value;
    update();
}
