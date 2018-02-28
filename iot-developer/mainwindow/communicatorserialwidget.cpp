#include "communicatorserialwidget.h"
#include "ui_communicatorserialwidget.h"

CommunicatorSerialWidget::CommunicatorSerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicatorSerialWidget)
{
    ui->setupUi(this);


    QLineEdit *x = ui->SendInput->lineEdit();
}

CommunicatorSerialWidget::~CommunicatorSerialWidget()
{
    delete ui;
}

void CommunicatorSerialWidget::on_commandLinkButton_pressed()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->SendInput->setFocus();
}

void CommunicatorSerialWidget::on_sendButton_pressed()
{
    ui->SerialDisplaylistWidget->addItem(ui->SendInput->currentText());
    ui->SendInput->addItem(ui->SendInput->currentText());
    ui->SendInput->clearEditText();
    ui->SendInput->setFocus();

}

void CommunicatorSerialWidget::on_CloseCommandLinkButton_pressed()
{
    ui->stackedWidget->setCurrentIndex(0);
}
