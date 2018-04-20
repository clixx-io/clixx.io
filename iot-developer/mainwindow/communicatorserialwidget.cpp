#include <QStringList>
#include <QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QDebug>

#include "communicatorserialwidget.h"
#include "ui_communicatorserialwidget.h"
#include "mainwindow.h"

CommunicatorSerialWidget::CommunicatorSerialWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommunicatorSerialWidget)
{
    ui->setupUi(this);

    refreshSerialPorts();

    // - A Timer is used to monitor for new serial ports (plugged in)
    //   and when a Port is open, that timer is used to read characters
    //   received.
    serialTimer = new QTimer(this);
    serialTimer->setInterval(3000);
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
        serialTimer->setInterval(200);
    }
}

void CommunicatorSerialWidget::on_sendButton_pressed()
{

    const char nl = '\r';
    QString terminatedOutput = ui->SendInput->currentText() + nl;

    // - Write the users input to the Serial Port
    serialPort->write(terminatedOutput.toLatin1());
    foreach (QChar c, terminatedOutput)
        serialBuffer.append(c);

    ui->SerialDisplaylistWidget->addItem(ui->SendInput->currentText());
    ui->SendInput->addItem(ui->SendInput->currentText());
    ui->SendInput->clearEditText();
    ui->SendInput->setFocus();

}

void CommunicatorSerialWidget::on_CloseCommandLinkButton_pressed()
{
    closeSerialPort();
    ui->stackedWidget->setCurrentIndex(0);
}

QStringList CommunicatorSerialWidget::ListSerialPorts()
{
    QStringList results;

    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {

        // Set the Dial range to the number of Baudrates that are available
        ui->BaudrateSpeedDial->setMaximum(serialPortInfo.standardBaudRates().length() - 1);

        availableBaudRates = serialPortInfo.standardBaudRates();

        results.append(serialPortInfo.portName());

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

       showStatusMessage(tr("Connected to %1 at %2 Baud.")
                          .arg(serialportname).arg(availableBaudRates[ui->BaudrateSpeedDial->value()]));

       QSettings settings("Clixx.io","IoT Developer");

       settings.beginGroup(serialportname);
       settings.setValue("baudrate",availableBaudRates[ui->BaudrateSpeedDial->value()]);
       settings.endGroup();

       // - Create a Timer for reading data
       serialTimer->start();

    } else {

        if (serialPort->errorString().contains(tr("Permission denied")))
        {
            QMessageBox::critical(this, tr("Permissions Error"), tr("<p>Insufficient Permissions to access your Serial Port.</p><p>Have you checked the UDEV permissions?</p>"
                                                                    "<p>See https://unix.stackexchange.com/questions/25258/ttyusb0-permission-changes-after-restart for example</p>"));

        } else
            QMessageBox::critical(this, tr("Error"), serialPort->errorString());

        showStatusMessage(tr("Open error %1").arg(serialPort->errorString()));
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

    showStatusMessage(tr("Disconnected"));
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
        qDebug() << "Item selected is " << item->text(0).toLatin1();

    refreshSerialPorts();

}

void CommunicatorSerialWidget::on_portSelectiontreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if (current)
    {
      portName = current->text(0);

      // - Check if the selected item has any settings
      QSettings settings("Clixx.io","IoT Developer");

      settings.beginGroup(portName);
      if (settings.allKeys().indexOf("baudrate") != -1)
      {
          ui->BaudrateSpeedDial->setValue(availableBaudRates.indexOf(settings.value("baudrate",9600).toInt()));
          ui->dialLabel->setText(tr("%1 Baud").arg(availableBaudRates[availableBaudRates.indexOf(settings.value("baudrate",9600).toInt())]));
      }
      settings.endGroup();
    }
}

void CommunicatorSerialWidget::on_TimerEvent()
{
    if (serialPort->isOpen())
    {
        const QByteArray data = serialPort->readAll();

        foreach (char c, data)
            displayRxChar(c);

    } else
        refreshSerialPorts();

}

void CommunicatorSerialWidget::refreshSerialPorts()
{
    // - Look for new or removed serial ports
    QStringList ports;
    QString portname;

    ports = ListSerialPorts();

    QList<QTreeWidgetItem*> displaylist = ui->portSelectiontreeWidget->findItems(tr("Serial"), Qt::MatchContains|Qt::MatchRecursive, 0);
    foreach(QTreeWidgetItem* item, displaylist)
    {

        // - scan the list of displayed ports
        for (int i=0;i<item->childCount();i++)
        {
            // -- check for deleted port
            if (!ports.contains(item->child(i)->text(0)))
            {
                qDebug() << "deleted port" << item->child(i)->text(0);
                QList<QTreeWidgetItem*> foundlist = ui->portSelectiontreeWidget->findItems(item->child(i)->text(0), Qt::MatchContains|Qt::MatchRecursive, 0);
                foreach (QTreeWidgetItem *widget, foundlist)
                {
                    delete widget;
                }
            }

        }

        foreach (portname, ports)
        {

            QList<QTreeWidgetItem*> foundlist = ui->portSelectiontreeWidget->findItems(portname, Qt::MatchContains|Qt::MatchRecursive, 0);
            if (!foundlist.length())
            {
                // - Add the new item
                QTreeWidgetItem * portitem = new QTreeWidgetItem();
                portitem->setText(0,portname);
                item->addChild(portitem);
                qDebug() << "add port" << portname;
            }

        }

    }
}

bool CommunicatorSerialWidget::writewithEcho(const QString linetosend)
{

    serialTimer->stop();

    foreach (QChar c, linetosend)
    {
        serialPort->write((const char *) &c,sizeof(c));
        serialPort->waitForBytesWritten();

        bool havechar(false);
        do
        {
            const QByteArray rcvdata = serialPort->readAll();
            foreach (char r, rcvdata)
            {
                if (r==c)
                {
                    havechar = true;
                    displayRxChar(r);
                }
                else
                    displayRxChar(r);
            }

        } while (!havechar);

    }

}

void CommunicatorSerialWidget::displayRxChar(char c)
{
    QListWidgetItem *lastline;
    const char nl = '\n';

    if (ui->SerialDisplaylistWidget->count() == 0)
    {
        lastline = new QListWidgetItem();
        ui->SerialDisplaylistWidget->addItem(lastline);
    } else
    {
        lastline = ui->SerialDisplaylistWidget->item(ui->SerialDisplaylistWidget->count()-1);
    }

    if ((serialBuffer[0]==c) && (serialBuffer.size()>0))
    {
        // No need to display echo characters
        serialBuffer.remove(0,1);
        return;
    }

    if (c == nl)
    {
        lastline = new QListWidgetItem();
        ui->SerialDisplaylistWidget->addItem(lastline);
    } else
        lastline->setText(lastline->text() + c);

}

void CommunicatorSerialWidget::showStatusMessage(const QString &message)
{
    mainwindow->showStatusMessage(message);
}

void CommunicatorSerialWidget::on_portSelectiontreeWidget_itemPressed(QTreeWidgetItem *item, int column)
{
    if (item->text(0) == tr("Serial"))
    {
        refreshSerialPorts();

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

