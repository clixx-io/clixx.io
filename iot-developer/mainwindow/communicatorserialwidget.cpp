#include <QStringList>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>

#include "communicatorserialwidget.h"
#include "ui_communicatorserialwidget.h"

CommunicatorSerialWidget::CommunicatorSerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicatorSerialWidget)
{
    ui->setupUi(this);

    ListSerialPorts();

    // QLineEdit *x = ui->SendInput->lineEdit();
}

CommunicatorSerialWidget::~CommunicatorSerialWidget()
{
    delete ui;
}

void CommunicatorSerialWidget::on_commandLinkButton_pressed()
{
    if (openSerialPort())
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->SendInput->setFocus();
    }

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

QStringList CommunicatorSerialWidget::ListSerialPorts()
{
    QStringList results;

    QList<QTreeWidgetItem*> clist = ui->portSelectiontreeWidget->findItems(tr("Serial"), Qt::MatchContains|Qt::MatchRecursive, 0);
    foreach(QTreeWidgetItem* item, clist)
    {
        foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
        {
            QTreeWidgetItem * portitem = new QTreeWidgetItem();
            portitem->setText(0,serialPortInfo.portName());

            item->addChild(portitem);
        }
    }

    return(results);
}

bool CommunicatorSerialWidget::openSerialPort()
{
    bool retval(true);

    ui->BaudrateSpeedDial->setMaximum(115200);

    if (!serialPort)
        serialPort = new QSerialPort();
    else
        serialPort->close();

    serialPort->setPortName("COM4:");
    serialPort->setBaudRate(9600);
    serialPort->setDataBits(QSerialPort::Data8);

    // Parity
    if (ui->parityNRadioButton->isChecked())
        serialPort->setParity(QSerialPort::NoParity);
    else if (ui->parityERadioButton->isChecked())
        serialPort->setParity(QSerialPort::EvenParity);
    else if (ui->parityORadioButton->isChecked())
        serialPort->setParity(QSerialPort::OddParity);

    if (ui->stop1bitsRadioButton->isChecked())
        serialPort->setStopBits(QSerialPort::OneStop);
    else
        serialPort->setStopBits(QSerialPort::TwoStop);

    if (serialPort->open(QIODevice::ReadWrite)) {
        //m_console->setEnabled(true);
        //m_console->setLocalEchoEnabled(p.localEchoEnabled);
        //m_ui->actionConnect->setEnabled(false);
        //m_ui->actionDisconnect->setEnabled(true);
        //m_ui->actionConfigure->setEnabled(false);
        //showStatusMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
        //                  .arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
        //                  .arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));
    } else {
        QMessageBox::critical(this, tr("Error"), serialPort->errorString());

        //showStatusMessage(tr("Open error"));
        retval = false;
    }

    return(retval);
}
//! [4]

//! [5]
void CommunicatorSerialWidget::closeSerialPort()
{
    if (serialPort->isOpen())
        serialPort->close();
    //m_console->setEnabled(false);
    //m_ui->actionConnect->setEnabled(true);
    //m_ui->actionDisconnect->setEnabled(false);
    //m_ui->actionConfigure->setEnabled(true);
    //showStatusMessage(tr("Disconnected"));
}
