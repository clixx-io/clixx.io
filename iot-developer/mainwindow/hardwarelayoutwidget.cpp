#include <QDebug>

#include "hardwarelayoutwidget.h"
#include "ui_hardwarelayoutwidget.h"

/*
 * connectableHardware::connectableHardware(QGraphicsItem *parent, qreal x, qreal y, QString name, QGraphicsTextItem *label)
{
}
*/

connectableHardware::connectableHardware(const QPixmap &pixmap, QGraphicsItem *parent)
 : QGraphicsPixmapItem(pixmap,parent)
{
}


void connectableHardware::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter,option,widget);

    /*
    painter->setPen(Qt::yellow);
    painter->setBrush(Qt::red);
    painter->drawRect(0,0,50,50);
    */
}

void connectableHardware::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

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

void HardwareLayoutWidget::on_AddcommandLinkButton_clicked()
{
    connectableHardware *item = new connectableHardware(QPixmap(":/res/res/mainboard-rpi0.PNG"));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    scene->addItem(item);
}
