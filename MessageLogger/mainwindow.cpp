#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "indicator.h"
#include "connectionform.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QFileDialog>
#include <QFile>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Message Logger");
    client = new QMqttClient(this);
    connect(client, &QMqttClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(client, &QMqttClient::stateChanged, this, [&] (QMqttClient::ClientState state) {
        switch (state) {
        case QMqttClient::ClientState::Disconnected:
            ui->lineEdit->setEnabled(false);
            ui->startStopLogging->setEnabled(false);
            ui->actionConectar->setEnabled(true);
            ui->actionDisconnect->setEnabled(false);
            ui->actionPublish->setEnabled(false);

            ui->statusIndicator->setStatus(IndicatorStatus::DISCONNECTED);
            ui->statusLabel->setText("Disconnected");
            break;;
        case QMqttClient::ClientState::Connecting:
            ui->lineEdit->setEnabled(false);
            ui->startStopLogging->setEnabled(false);
            ui->actionConectar->setEnabled(true);
            ui->actionDisconnect->setEnabled(false);
            ui->actionPublish->setEnabled(false);

            ui->statusIndicator->setStatus(IndicatorStatus::DISCONNECTED);
            ui->statusLabel->setText("Connecting");
            break;
        case QMqttClient::ClientState::Connected:
            ui->lineEdit->setEnabled(true);
            ui->startStopLogging->setEnabled(true);
            ui->actionConectar->setEnabled(false);
            ui->actionDisconnect->setEnabled(true);
            ui->actionPublish->setEnabled(true);

            ui->statusIndicator->setStatus(IndicatorStatus::READY);
            ui->statusLabel->setText("Ready");
            break;
        }
    });

    connect(client, &QMqttClient::errorChanged, this, [&] (QMqttClient::ClientError error) {
        qDebug() << error;
        switch (error) {
        case QMqttClient::ClientError::BadUsernameOrPassword:
            QMessageBox::critical(this, "Error", "Incorrect client ID or API key");
            break;
        case QMqttClient::ClientError::IdRejected:
            QMessageBox::warning(this, "Error", "Client ID was rejected. Please try again in a few seconds");
            break;
        case QMqttClient::ClientError::InvalidProtocolVersion:
            QMessageBox::critical(this, "Error", "Invalid protocol version");
            break;
        case QMqttClient::ClientError::Mqtt5SpecificError:
            QMessageBox::critical(this, "Error", "MQTT 5 error");
            break;
        case QMqttClient::ClientError::NoError:
//            QMessageBox::critical(this, "Error", "");
            break;
        case QMqttClient::ClientError::NotAuthorized:
            QMessageBox::critical(this, "Error", "Incorrect client ID or API key");
            break;
        case QMqttClient::ClientError::ProtocolViolation:
            QMessageBox::critical(this, "Error", "Protocol violation");
            break;
        case QMqttClient::ClientError::ServerUnavailable:
            QMessageBox::critical(this, "Error", "Server unavailable");
            break;
        case QMqttClient::ClientError::TransportInvalid:
            QMessageBox::critical(this, "Error", "Invalid transport layer");
            break;
        case QMqttClient::ClientError::UnknownError:
//            QMessageBox::critical(this, "Error", "Unknown error");
            break;
        }
    });

    connectionForm = new ConnectionForm();

    connect(connectionForm, &ConnectionForm::connectionRequested, this, &MainWindow::onConnectionRequested);

    publisher = new PublisherWindow();

    connect(publisher, &PublisherWindow::messagePublished, this, [&](QString topic, QString message) {
        client->publish(topic, message.toLatin1());
    });
}

MainWindow::~MainWindow() {
    delete ui;
    delete connectionForm;
    delete publisher;
}


void MainWindow::on_startStopLogging_clicked() {
    if (ui->statusIndicator->getStatus() != IndicatorStatus::LOGGING) {
        if(ui->lineEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Error", "Could not subscribe. Topic is empty");
            return;
        }

        subscription = client->subscribe(QMqttTopicFilter(ui->lineEdit->text()), 2);
        if(!subscription) {
            QMessageBox::critical(this, "Error", "Could not subscribe. Is there a valid connection?");
            return;
        }

        ui->lineEdit->setEnabled(false);

        ui->startStopLogging->setText("Stop logging");
        ui->statusIndicator->setStatus(IndicatorStatus::LOGGING);
        ui->statusLabel->setText("Logging");
    } else {
        ui->lineEdit->setEnabled(true);
        client->unsubscribe(QMqttTopicFilter(ui->lineEdit->text()));

        ui->startStopLogging->setText("Start logging");
        ui->statusIndicator->setStatus(IndicatorStatus::READY);
        ui->statusLabel->setText("Ready");
    }
}

void MainWindow::on_actionConectar_triggered() {
    connectionForm->show();
}

void MainWindow::on_actionDisconnect_triggered() {
    client->disconnectFromHost();
}

void MainWindow::on_actionPublish_triggered() {
    publisher->show();
}

void MainWindow::onConnectionRequested(QString host, quint64 port, QString id, QString apiKey) {
    connectionForm->hide();

    client->setHostname(host);
    client->setPort(port);
    client->setUsername(id);
    client->setPassword(apiKey);

    client->connectToHost();
}

void MainWindow::onMessageReceived(const QByteArray &message, const QMqttTopicName &topic) {
    const QString content = "[" + QDateTime::currentDateTime().toString() + ", "
            + topic.name() + "]: "
            + message
            + QLatin1Char('\n');
    ui->plainTextEdit->insertPlainText(content);
    messages.append(QList<QString>() << QDateTime::currentDateTime().toString() << topic.name() << QString::fromLatin1(message));
}

void MainWindow::on_actionExport_to_CSV_triggered() {
    QString path;
    path = QFileDialog::getSaveFileName(this, "New file", QDir::homePath(), "*.csv");

    if(path.isEmpty()) {
        return;
    }

    QFile file(path);

    if(file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);

        stream << "Timestamp" << ", " << "Topic" << ", " << "Message" << "\n";
        foreach(QList<QString> data, messages) {
            stream << data.at(0) << ", " << data.at(1) << ", " << data.at(2) << "\n";
        }

    } else {
        QMessageBox::critical(this, "Could not create the file", file.errorString());
    }
}

void MainWindow::on_actionClear_triggered() {
    ui->plainTextEdit->clear();
    messages.clear();
}

void MainWindow::on_actionAbout_triggered() {
    QDesktopServices::openUrl ( QUrl("http:/imt-iotlabs.net/docs/logger/intro/") );
}

void MainWindow::on_actionAbout_Qt_triggered() {
    QMessageBox::aboutQt(this, "About Qt");
}
