#include "publisherwindow.h"
#include "ui_publisherwindow.h"

PublisherWindow::PublisherWindow(QWidget *parent): QWidget(parent), ui(new Ui::PublisherWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Publisher");
    connect(ui->lineEdit_2, &QLineEdit::returnPressed, this, [&]() {
        ui->pushButton->click();
        ui->lineEdit_2->clear();
    });
}

PublisherWindow::~PublisherWindow() {
    delete ui;
}

void PublisherWindow::on_pushButton_clicked() {
    messagePublished(ui->lineEdit->text(), ui->lineEdit_2->text());
}
