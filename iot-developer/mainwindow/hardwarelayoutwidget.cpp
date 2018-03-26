#include <QDebug>
#include <QInputDialog>
#include <QTreeWidgetItem>

#include "hardwarelayoutwidget.h"
#include "ui_hardwarelayoutwidget.h"

/*
 * connectableHardware::connectableHardware(QGraphicsItem *parent, qreal x, qreal y, QString name, QGraphicsTextItem *label)
{
}
*/

connectableHardware::connectableHardware(const QPixmap &pixmap, QGraphicsItem *parent)
 : QGraphicsPixmapItem(pixmap,parent), hardwareType(0)
{

}


void connectableHardware::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if (hardwareType == 0)
        QGraphicsPixmapItem::paint(painter,option,widget);
    else
    {
        painter->setPen(Qt::yellow);
        painter->setBrush(Qt::gray);
        painter->drawRect(boundingRect());
    }
    /*
    */
}

void connectableHardware::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (hardwareType == 0)
        hardwareType = 1;
    else
        hardwareType = 0;

    update();
}

QVariant connectableHardware::itemChange(GraphicsItemChange change, const QVariant &value)
{
    qDebug() << "Change:" << change;

    if (change == GraphicsItemChange::ItemPositionChange)
    {
        if (joiner)
        {
            joiner->setLine(-75,-50,pos().x(),pos().y());
        }
    }

    return QGraphicsItem::itemChange(change, value);
}

HardwareLayoutWidget::HardwareLayoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareLayoutWidget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    QBrush greenbrush(Qt::green);
    QPen blackpen(Qt::black);
    blackpen.setWidth(3);

    QGraphicsLineItem *joiner = scene->addLine(-100,-100,100,100,blackpen);
    joiner->setFlag(QGraphicsItem::ItemIsMovable);

    connectableHardware *item = new connectableHardware(QPixmap(":/res/res/mainboard-rpi3.PNG"));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    item->joiner = joiner;
    scene->addItem(item);

    QGraphicsRectItem *ioboard = scene->addRect(-100,-100,50,50,blackpen,greenbrush);
    ioboard->setFlag(QGraphicsItem::ItemIsMovable);

    /*
    PropertiestreeWidget
    ui->PropertiestreeView->clear();
            ui->projectFileList->clear();
            for(int i=0; i< files.size(); i++)
            {
                QTreeWidgetItem * item = new QTreeWidgetItem();
                item->setText(0,files[i]);
                ui->projectFileList->addTopLevelItem(item);
            }
    */

}

HardwareLayoutWidget::~HardwareLayoutWidget()
{
    delete ui;
}

bool HardwareLayoutWidget::LoadComponents(const QString filename)
{

}

bool HardwareLayoutWidget::SaveComponents(const QString filename)
{

}

void HardwareLayoutWidget::on_PropertiestreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    bool ok;

    QString text = QInputDialog::getText((QWidget *) this->parentWidget(), QString("Property"),
                                         QString(tr("%1 Value:").arg(item->text(1))), QLineEdit::Normal,
                                         QString("value"), &ok);
    if (ok && !text.isEmpty())
    {
        item->setText(1,text);
    }

}
