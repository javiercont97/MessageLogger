#ifndef PUBLISHERWINDOW_H
#define PUBLISHERWINDOW_H

#include <QWidget>

namespace Ui {
class PublisherWindow;
}

class PublisherWindow : public QWidget {
    Q_OBJECT

public:
    explicit PublisherWindow(QWidget *parent = nullptr);
    ~PublisherWindow();

private:
    Ui::PublisherWindow *ui;

signals:
    void messagePublished(QString topic, QString message);
private slots:
    void on_pushButton_clicked();
};

#endif // PUBLISHERWINDOW_H
