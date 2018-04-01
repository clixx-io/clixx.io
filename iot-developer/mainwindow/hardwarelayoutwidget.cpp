#include <QObject>
#include <QString>
#include <QDebug>
#include <QInputDialog>
#include <QTreeWidgetItem>

#include "hardwarelayoutwidget.h"
#include "ui_hardwarelayoutwidget.h"
#include "hardwaregpio.h"

connectableHardware::connectableHardware(QString ID, QString name, QString boardfile, int pins, int rows, qreal width, qreal height, QString graphicfile, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_id(ID), m_name(name), m_boardfile(boardfile), hardwareType(0),
      m_pins(pins), m_rows(rows), m_width(width), m_height(height)
{
    if (graphicfile.length())
    {
        m_image = new QPixmap(graphicfile);
        m_imagefilename = graphicfile;
    }

    copyBoardFileProperties(boardfile);

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

    return QGraphicsItem::mousePressEvent(event);

}

QRectF connectableHardware::boundingRect() const
{
    return(QRectF(0,0,m_width, m_height));
}

void connectableHardware::copyBoardFileProperties(QString boardfilename)
{
    QSettings boardfile(boardfilename, QSettings::IniFormat);

    int gpiocount = boardfile.value("gpio/pins").toInt();
    QString pinname,keyname;
    for (int i=0; i < gpiocount; i++)
    {
        keyname = QObject::tr("gpio_assignments/pin%1_name").arg(i+1);
        pinname = boardfile.value(keyname,"nc").toString();
        m_gpiopin_names.append(pinname);
    }

}

connectableCable::connectableCable(QString startItem, QString endItem, QGraphicsItem *parent)
    : QGraphicsItem(parent),
      m_startItem(startItem),
      m_endItem(endItem)
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

    ui->graphicsView->setScene(scene);

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(SelectionChanged()));

    QBrush greenbrush(Qt::green);
    QPen blackpen(Qt::black);
    blackpen.setWidth(3);

    QGraphicsLineItem *joiner = scene->addLine(-50,-50,100,100,blackpen);
    joiner->setFlag(QGraphicsItem::ItemIsMovable);

    connectableHardware *item = new connectableHardware("5x","board","",30,2,90,40,":/res/res/mainboard-rpi3.PNG");
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    item->joiner = joiner;
    scene->addItem(item);

}

void HardwareLayoutWidget::SelectionChanged()
{
    qDebug() << tr("slot Selection Change, %1 items selected").arg(scene->selectedItems().count());

    QString itemName, itemWidth, itemHeight, itemImage, itemPinCount,  ItemColumns, ItemPosition(tr("<unassigned>")), itemOrientation(tr("Normal"));
    QStringList itemPinAssignments;

    if (scene->selectedItems().count()==1)
    {
        connectableHardware *h = qgraphicsitem_cast<connectableHardware *>(scene->selectedItems()[0]);
        if (h)
        {
            itemName = h->getName();
            itemWidth = tr("%1").arg(h->getWidth());
            itemHeight = tr("%1").arg(h->getHeight());
            itemImage = h->getImageFilename();
            itemPinCount = tr("%1").arg(h->getPinCount());
            ItemColumns =  tr("%1").arg(h->getRowCount());
            itemPinAssignments = h->getPinAssignments();
        }

    }

    QList<QTreeWidgetItem*> clist = ui->PropertiestreeWidget->findItems(tr("Name"), Qt::MatchContains|Qt::MatchRecursive, 0);
    foreach(QTreeWidgetItem* item, clist) {
        item->setText(1,itemName);
    }
    clist = ui->PropertiestreeWidget->findItems(tr("Width"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
        clist[0]->setText(1,itemWidth);

    clist = ui->PropertiestreeWidget->findItems(tr("Height"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
        clist[0]->setText(1,itemHeight);

    clist = ui->PropertiestreeWidget->findItems(tr("Image"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
        clist[0]->setText(1,itemImage);

    clist = ui->PropertiestreeWidget->findItems(tr("Pin Count"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
        clist[0]->setText(1,itemPinCount);

    clist = ui->PropertiestreeWidget->findItems(tr("Columns"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
        clist[0]->setText(1,ItemColumns);

    clist = ui->PropertiestreeWidget->findItems(tr("Position"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
        clist[0]->setText(1,ItemPosition);

    clist = ui->PropertiestreeWidget->findItems(tr("Orientation"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
        clist[0]->setText(1,itemOrientation);

    clist = ui->PropertiestreeWidget->findItems(tr("Pin Assignments"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
    {
        QString pinlabel;
        int pinnumber(0);

        clist[0]->takeChildren();

        foreach (QString pinvalue, itemPinAssignments)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem();

            pinlabel = tr("%1").arg(++pinnumber);

            item->setText(0,pinlabel);
            item->setText(1,pinvalue);

            clist[0]->addChild(item);

        }
        ui->PropertiestreeWidget->update();

    }

    SaveComponents("abc.txt");

}

HardwareLayoutWidget::~HardwareLayoutWidget()
{
    delete ui;
}

bool HardwareLayoutWidget::LoadComponents(const QString filename)
{

    qDebug() << "loading from :" << filename;

    QSettings boardfile(filename, QSettings::IniFormat);

    int count = boardfile.value("overview/items",0).toInt();
    for (int i=0; i < count; i++)
    {
        qDebug() << tr(" - item:%1").arg(i+1);
    }

    return(false);
}

bool HardwareLayoutWidget::SaveComponents(const QString filename)
{

    qDebug() << "Saving to :" << filename;

    QSettings boardfile(filename, QSettings::IniFormat);

    boardfile.setValue("overview/items",scene->items().count());
    for (int i=0; i < scene->items().count(); i++)
    {
        QGraphicsItem *item = scene->items()[i];

        boardfile.beginGroup(tr("Item_%1").arg(i+1));

        connectableHardware *hw = qgraphicsitem_cast<connectableHardware *>(item);
        if (hw)
        {
            boardfile.setValue("id",hw->getID());
            boardfile.setValue("name",hw->getName());
            boardfile.setValue("class","Component");
            boardfile.setValue("type",hw->getType());
            boardfile.setValue("width",hw->getWidth());
            boardfile.setValue("height",hw->getHeight());
        }
        connectableCable *cbl = qgraphicsitem_cast<connectableCable *>(item);
        if (cbl)
        {
            boardfile.setValue("id",hw->getID());
            boardfile.setValue("name",hw->getName());
            boardfile.setValue("class","Cable");
            boardfile.setValue("startitem",hw->getHeight());
            boardfile.setValue("enditem",hw->getHeight());
            boardfile.setValue("type",hw->getType());
            boardfile.setValue("color",hw->getHeight());
            boardfile.setValue("wirecount",hw->getHeight());
        }

        boardfile.setValue("x",item->x());
        boardfile.setValue("y",item->y());
        boardfile.endGroup();
    }

    return(false);
}

void HardwareLayoutWidget::on_PropertiestreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    bool ok;

    QString text = QInputDialog::getText((QWidget *) this->parentWidget(), QString("Property"),
                                         QString(tr("%1 Value:").arg(item->text(0))), QLineEdit::Normal,
                                         item->text(1), &ok);
    if (ok /* && !text.isEmpty() */)
    {
        item->setText(1,text);
    }

}

bool HardwareLayoutWidget::addToScene(QString componentID, QString componentName, QString componentBoardFile, QString componentImageName, double componentWidth, double componentHeight, int pins, int rows)
{
    connectableHardware *item = new connectableHardware(componentID,componentName,componentBoardFile,pins, rows, componentWidth,componentHeight,componentImageName);

    item->setFlag(QGraphicsItem::ItemIsSelectable);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    scene->addItem(item);

    return(false);
}

bool HardwareLayoutWidget::addCableToScene(QString cableName, QColor cableColor, int pins, int rows)
{
    connectableCable *item = new connectableCable("1","2");
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    scene->addItem(item);

    return(true);
}

// Zoom in works
// ui->graphicsView->scale(1.5,1.5);

// Move the viewport to the left works
// QRectF scenepos = ui->graphicsView->sceneRect();
// scenepos.setLeft(scenepos.x()-30);
// ui->graphicsView->setSceneRect(scenepos);

// Rotate the last item 90 degrees
/*
 * int i = scene->items().count()-1;
QGraphicsItem *item = scene->items()[i];
item->setRotation(item->rotation()+45);

qDebug() << "There are " << scene->items().count() << " items in the scene.";

SaveComponents("myfile.txt");
LoadComponents("myfile.txt");
*/
