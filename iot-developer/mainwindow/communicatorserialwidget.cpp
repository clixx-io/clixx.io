#include <QStringList>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QDebug>

#include "communicatorserialwidget.h"
#include "ui_communicatorserialwidget.h"

CommunicatorSerialWidget::CommunicatorSerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicatorSerialWidget)
{
    ui->setupUi(this);

    ListSerialPorts();

    // - A Timer is used to monitor for new serial ports (plugged in)
    //   and when a Port is open, that timer is used to read characters
    //   received.
    serialTimer = new QTimer(this);
    serialTimer->setInterval(200);
    serialTimer->setSingleShot(false);
    connect(serialTimer, SIGNAL(timeout()),this,SLOT(on_TimerEvent()));

}

CommunicatorSerialWidget::~CommunicatorSerialWidget()
{
    delete ui;
}

void CommunicatorSerialWidget::on_commandLinkButton_pressed()
{

    if (openSerialPort(portName))
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->SendInput->setFocus();
    }

}

void CommunicatorSerialWidget::on_sendButton_pressed()
{

    const char nl = '\r';
    QString terminatedOutput = ui->SendInput->currentText() + nl;

    // - Write the users input to the Serial Port
    serialPort->write(terminatedOutput.toLatin1());

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

            // Set the Dial range to the number of Baudrates that are available
            ui->BaudrateSpeedDial->setMaximum(serialPortInfo.standardBaudRates().length() - 1);

            availableBaudRates = serialPortInfo.standardBaudRates();

        }
    }

    return(results);
}

bool CommunicatorSerialWidget::openSerialPort(const QString serialportname)
{
    bool retval(true);

    if (!serialPort)
        serialPort = new QSerialPort();
    else
        serialPort->close();

    serialPort->setPortName(serialportname);
    serialPort->setBaudRate(availableBaudRates[ui->BaudrateSpeedDial->value()]);
    serialPort->setDataBits(QSerialPort::Data8);

    // - Parity
    if (ui->parityNRadioButton->isChecked())
        serialPort->setParity(QSerialPort::NoParity);
    else if (ui->parityERadioButton->isChecked())
        serialPort->setParity(QSerialPort::EvenParity);
    else if (ui->parityORadioButton->isChecked())
        serialPort->setParity(QSerialPort::OddParity);

    // - Stop Bits
    if (ui->stop1bitsRadioButton->isChecked())
        serialPort->setStopBits(QSerialPort::OneStop);
    else
        serialPort->setStopBits(QSerialPort::TwoStop);

    if (serialPort->open(QIODevice::ReadWrite)) {

       // - Create a Timer for reading data
       serialTimer -> start();

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
    //showStatusMessage(tr("Disconnected"));
}

void CommunicatorSerialWidget::on_BaudrateSpeedDial_sliderMoved(int position)
{
    if (position >= 0)
        ui->dialLabel->setText(tr("%1 Baud").arg(availableBaudRates[position]));

    qDebug() << "Slider position " << position << " baudrate" << availableBaudRates[position];
}

void CommunicatorSerialWidget::on_portSelectiontreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->isSelected())
        qDebug("Item is selected item");

    QString msg = item->text(0);

}

void CommunicatorSerialWidget::on_portSelectiontreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    portName = current->text(0);
}

void CommunicatorSerialWidget::on_TimerEvent()
{
    const QByteArray data = serialPort->readAll();
    const char nl = '\n';

    if (data.indexOf(nl) != -1)
    {
        serialBuffer += data.left(data.indexOf(nl)-1);

        ui->SerialDisplaylistWidget->addItem(serialBuffer);

        serialBuffer = data.right(data.length()-serialBuffer.length());

    } else

        serialBuffer += data;

}

void CommunicatorSerialWidget::showStatusMessage(const QString &message)
{

}

void CommunicatorSerialWidget::on_portSelectiontreeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    if (item->text(0) == tr("Serial"))
    {
        ui->TargetstackedWidget->setCurrentIndex(0);
    }
    else if (item->text(0) == tr("TCP"))
    {
        ui->TargetstackedWidget->setCurrentIndex(1);
    }
    else if (item->text(0) == tr("UDP"))
    {
        ui->TargetstackedWidget->setCurrentIndex(1);
    }
    else if (item->text(0) == tr("UDP"))
    {
        ui->TargetstackedWidget->setCurrentIndex(1);
    }
    else if (item->text(0) == tr("I2C"))
    {
        ui->TargetstackedWidget->setCurrentIndex(1);
    }
    else if (item->text(0) == tr("MQTT"))
    {
        ui->TargetstackedWidget->setCurrentIndex(1);
    }
    else if (item->text(0) == tr("IFTTT"))
    {
        ui->TargetstackedWidget->setCurrentIndex(1);
    }

}
