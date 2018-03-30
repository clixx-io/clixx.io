#include <QDebug>
#include <QInputDialog>
#include <QTreeWidgetItem>

#include "hardwarelayoutwidget.h"
#include "ui_hardwarelayoutwidget.h"
#include "hardwaregpio.h"

connectableHardware::connectableHardware(QString name, qreal width, qreal height, QString guid, QString graphicfile, QGraphicsItem *parent)
    : QGraphicsItem(parent), hardwareType(0), m_width(width), m_height(height)
{
    if (graphicfile.length())
        m_image = new QPixmap(graphicfile);
}

void connectableHardware::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    if ((hardwareType == 0) && (m_image))
    {
        painter->drawImage(boundingRect(),m_image->toImage());
    }
    else
    {
        painter->setPen(Qt::yellow);
        painter->setBrush(Qt::gray);
        painter->drawRect(boundingRect());
    }
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
    } else if (change == GraphicsItemChange::ItemSelectedChange)
    {
        if (value.toBool())
            qDebug() << "Selected:" << this->m_name;
        else
            qDebug() << "unSelected:" << this->m_name;

    }


    return QGraphicsItem::itemChange(change, value);
}

void connectableHardware::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Clicked:" << this->m_name;

    this->setSelected(!this->isSelected());

}

QRectF connectableHardware::boundingRect() const
{
    return(QRectF(0,0,m_width, m_height));
}

connectableCable::connectableCable(QString cableName, QColor cableColor, int pins, int rows, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
}

void connectableCable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::yellow);
    painter->setBrush(Qt::gray);
    painter->drawRect(boundingRect());
}

QRectF connectableCable::boundingRect() const
{
}

HardwareLayoutWidget::HardwareLayoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareLayoutWidget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(SelectionChanged()));

    ui->graphicsView->setScene(scene);

    QBrush greenbrush(Qt::green);
    QPen blackpen(Qt::black);
    blackpen.setWidth(3);

    QGraphicsLineItem *joiner = scene->addLine(-100,-100,100,100,blackpen);
    joiner->setFlag(QGraphicsItem::ItemIsMovable);

    connectableHardware *item = new connectableHardware("board",90,40,"guid",":/res/res/mainboard-rpi3.PNG");
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    item->joiner = joiner;
    scene->addItem(item);

    QGraphicsRectItem *ioboard1 = scene->addRect(-150,-100,50,50,blackpen,greenbrush);
    ioboard1->setFlag(QGraphicsItem::ItemIsMovable);

}

void HardwareLayoutWidget::SelectionChanged()
{
    qDebug() << "slot Selection Change";
}

HardwareLayoutWidget::~HardwareLayoutWidget()
{
    delete ui;
}

bool HardwareLayoutWidget::LoadComponents(const QString filename)
{
    return(false);
}

bool HardwareLayoutWidget::SaveComponents(const QString filename)
{
    return(false);
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

bool HardwareLayoutWidget::addToScene(QString componentID, QString componentName, QString componentImageName, double componentWidth, double componentHeight, int pins, int rows)
{
    connectableHardware *item = new connectableHardware(componentName,componentWidth,componentHeight,componentID,componentImageName);

    item->setFlag(QGraphicsItem::ItemIsSelectable);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    scene->addItem(item);

    return(false);
}

bool HardwareLayoutWidget::addCableToScene(QString cableName, QColor cableColor, int pins, int rows)
{
    connectableCable *item = new connectableCable(cableName,cableColor,pins,rows);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    scene->addItem(item);

    return(true);
}



