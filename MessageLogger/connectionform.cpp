#include "connectionform.h"
#include "ui_connectionform.h"

ConnectionForm::ConnectionForm(QWidget *parent): QWidget(parent), ui(new Ui::ConnectionForm) {
    ui->setupUi(this);
    this->setWindowTitle("Connect to Broker");
}

ConnectionForm::~ConnectionForm() {
    delete ui;
}

void ConnectionForm::on_pushButton_2_clicked() {
    emit connectionRequested(ui->hostLineEdit->text(), ui->spinBox->value(), ui->txt_appID->text(), ui->txt_apiKey->text());
}
