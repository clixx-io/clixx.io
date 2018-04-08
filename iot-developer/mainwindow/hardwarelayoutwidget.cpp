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

    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(Qt::red);
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
        foreach (connectableCable *cable, cables)
        {
            connectableHardware * hw1 = cable->getStartItem();
            connectableHardware * hw2 = cable->getEndItem();

            if (hw1 == this)
            {
                cable->setLine(pos().x(),pos().y(),hw2->x(),hw2->y());
            } else
            {
                cable->setLine(hw1->x(),hw1->y(),pos().x(),pos().y());
            }

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

void connectableHardware::addCableConnection(connectableCable *cable)
{
    cables.append(cable);
}

connectableCable::connectableCable(QString componentID, connectableHardware *startItem, connectableHardware *endItem, int wires, int rows, QColor cablecolor, QGraphicsItem *parent)
    : QGraphicsLineItem(startItem->x(),startItem->y(),endItem->x(),endItem->y(),parent),
      m_startItem(startItem),
      m_endItem(endItem),
      m_wires(wires),
      m_rows(rows),
      m_cablecolor(cablecolor),
      m_id(componentID)
{
    m_name = QObject::tr("Cable%1").arg(componentID);
}

void connectableCable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (option->state & QStyle::State_Selected)
    {
        painter->setPen(Qt::red);
        painter->drawRect(boundingRect());
    }

    /*
    painter->setPen(Qt::blue);

    QPainterPath path;
    path.moveTo(0, 0);
    path.lineTo(20, 30);
    path.cubicTo(80, 0, 50, 50, 80, 80);
    painter->drawPath(path);

    painter->setBrush(Qt::red);
    painter->drawRect(45, 45, 10, 10);
    */

    QGraphicsLineItem::paint(painter,option,widget);

}

void connectableCable::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Clicked:" << this->m_name;

    this->setSelected(!this->isSelected());

    return QGraphicsItem::mousePressEvent(event);

}

QVariant connectableCable::itemChange(GraphicsItemChange change, const QVariant &value)
{
    qDebug() << "Cable Change:" << change;

    if (change == GraphicsItemChange::ItemPositionChange)
    {
    } else if (change == GraphicsItemChange::ItemSelectedChange)
    {
        if (value.toBool())
            qDebug() << "Selected:" << this->m_name;
        else
            qDebug() << "unSelected:" << this->m_name;
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

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(SelectionChanged()));

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

}

HardwareLayoutWidget::~HardwareLayoutWidget()
{
    delete ui;
}

bool HardwareLayoutWidget::LoadComponents(const QString filename)
{

    qDebug() << "loading from :" << filename;

    QSettings boardfile(filename, QSettings::IniFormat);

    scene->items().clear();

    int count = boardfile.value("overview/items",0).toInt();
    for (int i=0; i < count; i++)
    {
        boardfile.beginGroup(tr("Item_%1").arg(i+1));

        QString compID = boardfile.value("id").toString().toLower();
        QString compName = boardfile.value("name").toString();
        QString compclass = boardfile.value("class").toString().toLower();

        int x = boardfile.value("x",0).toInt();
        int y = boardfile.value("y",0).toInt();
        int w = boardfile.value("width",0).toInt();
        int h = boardfile.value("height",0).toInt();

        if (compclass == "component")
        {
            qDebug() << tr(" - item:%1").arg(i+1);

            int p = boardfile.value("pins",0).toInt();
            int r = boardfile.value("rows",0).toInt();
            QString b = boardfile.value("board_file","").toString();
            QString i = boardfile.value("image_file","").toString();

            addToScene(compID,compName,x,y,b,i,w,h,p,r);

            ui->componentslistWidget->addItem(compName);

        }

        boardfile.endGroup();
    }

    for (int i=0; i < count; i++)
    {
        boardfile.beginGroup(tr("Item_%1").arg(i+1));

        QString compID = boardfile.value("id").toString().toLower();
        QString compName = boardfile.value("name").toString();
        QString compclass = boardfile.value("class").toString().toLower();

        int x = boardfile.value("x",0).toInt();
        int y = boardfile.value("y",0).toInt();
        int w = boardfile.value("width",0).toInt();
        int h = boardfile.value("height",0).toInt();

        if (compclass == "cable")
        {
            qDebug() << tr(" - item:%1").arg(i+1);

            QString si = boardfile.value("startitem","").toString();
            QString ei = boardfile.value("enditem","").toString();
            int wc = boardfile.value("wires",0).toInt();
            int rc = boardfile.value("rows",0).toInt();
            QString c = boardfile.value("color",0).toString();
            QColor cv(Qt::gray);

            connectableHardware *startitem = findByID(si);
            connectableHardware *enditem = findByID(ei);

            connectableCable *cb = new connectableCable(compID,startitem,enditem,wc,rc,cv);

            ui->componentslistWidget->addItem(compName);
        }

        boardfile.endGroup();
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
            boardfile.setValue("pins",hw->getPinCount());
            boardfile.setValue("rows",hw->getRowCount());

            boardfile.setValue("board_file",hw->getBoardFile());
            boardfile.setValue("image_file",hw->getImageFilename());

            boardfile.setValue("x",item->x());
            boardfile.setValue("y",item->y());

        }

        connectableCable *cbl = qgraphicsitem_cast<connectableCable *>(item);
        if (cbl)
        {
            boardfile.setValue("id",cbl->getID());
            boardfile.setValue("name",cbl->getName());
            boardfile.setValue("class","Cable");

            if (cbl->getStartItem())
                boardfile.setValue("startitem",cbl->getStartItem()->getID());
            else
                boardfile.setValue("startitem","");

            if (cbl->getEndItem())
                boardfile.setValue("enditem",cbl->getEndItem()->getID());
            else
                boardfile.setValue("enditem","");

            boardfile.setValue("type",cbl->getType());
            boardfile.setValue("color",cbl->getColor().name(QColor::HexRgb));
            boardfile.setValue("wirecount",cbl->getWireCount());
            boardfile.setValue("rows",cbl->getRows());
        }

        boardfile.endGroup();
    }

    return(false);
}

QList <connectableHardware *> HardwareLayoutWidget::getHardwareComponents()
{
    QList <connectableHardware *> results;

    for (int i=0; i < scene->items().count(); i++)
    {
        QGraphicsItem *item = scene->items()[i];

        connectableHardware *hw = qgraphicsitem_cast<connectableHardware *>(item);
        if (hw)
        {
            results.append(hw);
        }
    }

    return(results);
}

QGraphicsItem* HardwareLayoutWidget::findGraphicsItemByID(QString componentID)
{
    QGraphicsItem* result = nullptr;

    foreach (QGraphicsItem *item, scene->items())
    {
        connectableHardware *hw = qgraphicsitem_cast<connectableHardware *>(item);
        if (hw)
        {
            if (hw->getID() == componentID)
            {
                result = (QGraphicsItem *) hw;
                break;
            }
        }
        connectableCable *cbl = qgraphicsitem_cast<connectableCable *>(item);
        if (cbl)
        {
            if (cbl->getID() == componentID)
            {
                result = (QGraphicsItem *) cbl;
                break;
            }
        }
    }

    return(result);
}

connectableHardware* HardwareLayoutWidget::findByID(QString componentID)
{
    connectableHardware* result = nullptr;

    foreach (QGraphicsItem *item, scene->items())
    {
        connectableHardware *hw = qgraphicsitem_cast<connectableHardware *>(item);
        if (hw)
        {
            if (hw->getID() == componentID)
            {
                result = hw;
                break;
            }
        }
    }

    return(result);
}

connectableHardware* HardwareLayoutWidget::findByName(QString componentName)
{
    connectableHardware* result = nullptr;

    foreach (QGraphicsItem *item, scene->items())
    {
        connectableHardware *hw = qgraphicsitem_cast<connectableHardware *>(item);
        if (hw)
        {
            if (hw->getName() == componentName)
            {
                result = hw;
                break;
            }
        }
    }

    return(result);
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

QString HardwareLayoutWidget::getNextName(QString prefix)
{
    int itemcount(0);

    foreach (QGraphicsItem *item, scene->items())
    {
        connectableHardware *hw = qgraphicsitem_cast<connectableHardware *>(item);
        if (hw)
        {
            if (hw->getName().startsWith(prefix))
                itemcount++;
        }
        connectableCable *cbl = qgraphicsitem_cast<connectableCable *>(item);
        if (cbl)
        {
            if (cbl->getName().startsWith(prefix))
                itemcount++;
        }
    }

    return(prefix + QString::number(itemcount + 1));

}

bool HardwareLayoutWidget::addToScene(QString componentID, QString componentName, double x, double y, QString componentBoardFile, QString componentImageName, double componentWidth, double componentHeight, int pins, int rows)
{
    if (componentID.length() == 0)
        componentID = getNextID();

    connectableHardware *item = new connectableHardware(componentID,componentName,componentBoardFile,pins, rows, componentWidth,componentHeight,componentImageName);

    item->setX(x);
    item->setY(y);

    item->setFlag(QGraphicsItem::ItemIsSelectable);
    item->setFlag(QGraphicsItem::ItemIsMovable);
    item->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    scene->addItem(item);

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(item->getName());
    QVariant id(item->getID());
    newItem->setData(Qt::UserRole, id);
    ui->componentslistWidget->insertItem(ui->componentslistWidget->count(), newItem);

    return(false);
}

bool HardwareLayoutWidget::addCableToScene(QString componentID, QString startItem, QString endItem, int wires, int rows, QColor cablecolor)
{

    if (componentID.length() == 0)
        componentID = getNextID();

    connectableHardware *c1 = findByID(startItem);
    connectableHardware *c2 = findByID(endItem);

    connectableCable *cable = new connectableCable(componentID, c1, c2, wires, rows, cablecolor);

    c1->addCableConnection(cable);
    c2->addCableConnection(cable);

    cable->setFlag(QGraphicsItem::ItemIsSelectable);
    cable->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    scene->addItem(cable);

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(cable->getName());
    QVariant id(cable->getID());
    newItem->setData(Qt::UserRole, id);
    ui->componentslistWidget->insertItem(ui->componentslistWidget->count(), newItem);

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

*/


void HardwareLayoutWidget::on_componentslistWidget_itemClicked(QListWidgetItem *item)
{
    scene->clearSelection();

    QVariant data = item->data(Qt::UserRole);
    QString id = data.toString();

    QGraphicsItem *sceneitem = findGraphicsItemByID(id);
    if (sceneitem)
        sceneitem->setSelected(true);

}
