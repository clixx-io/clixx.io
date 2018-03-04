#include <QStringList>
#include <QtSerialPort/QSerialPortInfo>

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
