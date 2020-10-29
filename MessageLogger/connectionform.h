#ifndef CONNECTIONFORM_H
#define CONNECTIONFORM_H

#include <QWidget>

namespace Ui {
class ConnectionForm;
}

class ConnectionForm : public QWidget {
    Q_OBJECT

public:
    explicit ConnectionForm(QWidget *parent = nullptr);
    ~ConnectionForm();

signals:
    void connectionRequested(QString host, quint64 port, QString id, QString apikey);

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ConnectionForm *ui;
};

#endif // CONNECTIONFORM_H
