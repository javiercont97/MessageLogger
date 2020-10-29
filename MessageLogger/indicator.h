#ifndef INDICATOR_H
#define INDICATOR_H

#include <QWidget>

enum class IndicatorStatus {
    DISCONNECTED,
    READY,
    LOGGING
};


class Indicator : public QWidget {
    Q_OBJECT
public:

    explicit Indicator(QWidget *parent = nullptr);

    IndicatorStatus getStatus() const;
    void setStatus(const IndicatorStatus &value);

signals:

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);

private:
    IndicatorStatus status = IndicatorStatus::DISCONNECTED;
};

#endif // INDICATOR_H
