#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMqttClient>
#include <QMqttSubscription>
#include "connectionform.h"
#include "publisherwindow.h"
#include <QList>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startStopLogging_clicked();
    void on_actionConectar_triggered();
    void on_actionDisconnect_triggered();
    void on_actionPublish_triggered();

    void onConnectionRequested(QString host, quint64 port, QString id, QString apiKey);

    void onMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
    void on_actionExport_to_CSV_triggered();

    void on_actionClear_triggered();

    void on_actionAbout_triggered();

    void on_actionAbout_Qt_triggered();

private:
    Ui::MainWindow *ui;
    QMqttClient *client = nullptr;
    QMqttSubscription *subscription = nullptr;

    ConnectionForm *connectionForm;
    PublisherWindow *publisher;

    QList<QList<QString>> messages;
};
#endif // MAINWINDOW_H
