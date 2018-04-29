#include <QObject>
#include <QString>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QShortcut>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>

#include "hardwarelayoutwidget.h"
#include "ui_hardwarelayoutwidget.h"
#include "hardwaregpio.h"

connectableHardware::connectableHardware(QString ID, QString name, QString boardfile, int pins, int rows, qreal width, qreal height, QString graphicfile, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_id(ID), m_name(name), m_boardfile(boardfile), hardwareType(0),
      m_pins(pins), m_rows(rows), m_width(width), m_height(height), m_connectionpoint(0)
{
    if (graphicfile.length())
    {
        m_image = new QPixmap(graphicfile);
        m_imagefilename = graphicfile;
    }

    copyBoardFileProperties(boardfile);

}

connectableHardware::~connectableHardware()
{
    if (m_image)
        delete m_image;

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
            QGraphicsItem * g1 = cable->getStartItem();
            QGraphicsItem * g2 = cable->getEndItem();

            if (g1 == this)
            {
                connectableHardware *hw2 = qgraphicsitem_cast<connectableHardware *>(g2);
                if (hw2)
                    cable->setLine(pos().x()+getPrimaryConnectionPoint().x(), pos().y()+getPrimaryConnectionPoint().y(),
                                   hw2->x()+hw2->getPrimaryConnectionPoint().x(),hw2->y()+hw2->getPrimaryConnectionPoint().y());

                connectableGraphic *gf2 = qgraphicsitem_cast<connectableGraphic *>(g2);
                if (gf2)
                    cable->setLine(pos().x()+getPrimaryConnectionPoint().x(), pos().y()+getPrimaryConnectionPoint().y(),
                                   gf2->x()+gf2->getPrimaryConnectionPoint().x(),gf2->y()+gf2->getPrimaryConnectionPoint().y());

            } else
            {
                connectableHardware *hw1 = qgraphicsitem_cast<connectableHardware *>(g1);
                if (hw1)
                    cable->setLine(hw1->x()+hw1->getPrimaryConnectionPoint().x(),hw1->y()+hw1->getPrimaryConnectionPoint().y(),
                                   pos().x()+getPrimaryConnectionPoint().x(), pos().y()+getPrimaryConnectionPoint().y());

                connectableGraphic *gf1 = qgraphicsitem_cast<connectableGraphic *>(g1);
                if (gf1)
                    cable->setLine(gf1->x()+gf1->getPrimaryConnectionPoint().x(),gf1->y()+getPrimaryConnectionPoint().y(),
                                   pos().x()+getPrimaryConnectionPoint().x(), pos().y()+getPrimaryConnectionPoint().y());

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

connectableCable::connectableCable(QString componentID, QString componentName, QGraphicsItem *startItem, QGraphicsItem *endItem, int wires, int rows, QColor cablecolor, QGraphicsItem *parent)
    : QGraphicsLineItem(startItem->x(),startItem->y(),endItem->x(),endItem->y(),parent),
      m_startItem(startItem),
      m_endItem(endItem),
      m_wires(wires),
      m_rows(rows),
      m_cablecolor(cablecolor),
      m_id(componentID),
      m_name(componentName)
{
    if (m_name.size()==0)
        m_name = QObject::tr("Cable%1").arg(componentID);

    // Copy over the cable color
    QPen pen;
    pen.setColor(cablecolor);
    setPen(pen);

    // Adjust the starting position
    QPoint startPos(startItem->x(),startItem->y());
    connectableHardware *h = qgraphicsitem_cast<connectableHardware *>(startItem);
    if (h)
    {
        h->addCableConnection(this);
        startPos += h->getPrimaryConnectionPoint();
    }
    connectableGraphic *g = qgraphicsitem_cast<connectableGraphic *>(startItem);
    if (g)
    {
        g->addCableConnection(this);
        startPos += g->getPrimaryConnectionPoint();
    }

    // Adjust the ending position
    QPoint endPos(endItem->x(),endItem->y());
    h = qgraphicsitem_cast<connectableHardware *>(endItem);
    if (h)
    {
        h->addCableConnection(this);
        endPos += h->getPrimaryConnectionPoint();
    }
    g = qgraphicsitem_cast<connectableGraphic *>(endItem);
    if (g)
    {
        g->addCableConnection(this);
        endPos += g->getPrimaryConnectionPoint();
    }

    // Now use this line
    setLine(startPos.x(),startPos.y(),endPos.x(),endPos.y());
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

HardwareLayoutWidget::HardwareLayoutWidget(QGraphicsScene *existingScene, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareLayoutWidget),
    scene(existingScene)
{
    ui->setupUi(this);

    if (!scene)
        scene = new QGraphicsScene(this);
    else
        loadComponentlist(ui->componentslistWidget);

    ui->graphicsView->setScene(scene);

    connect(scene, SIGNAL(selectionChanged()), this, SLOT(SelectionChanged()));

    // Shortcuts for deleting items
    QShortcut* delshortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->componentslistWidget);
    connect(delshortcut, SIGNAL(activated()), this, SLOT(deleteItem()));

    // Shortcuts for zoomin + zoomout
    QShortcut* zoominshortcut = new QShortcut(QKeySequence(Qt::Key_Plus), ui->graphicsView);
    connect(zoominshortcut, SIGNAL(activated()), this, SLOT(zoomin()));

    QShortcut* zoomoutshortcut = new QShortcut(QKeySequence(Qt::Key_Minus), ui->graphicsView);
    connect(zoomoutshortcut, SIGNAL(activated()), this, SLOT(zoomout()));

    // Shortcuts for cursor keys
    QShortcut* leftshortcut = new QShortcut(QKeySequence(Qt::Key_Left), ui->graphicsView);
    connect(leftshortcut, SIGNAL(activated()), this, SLOT(panleft()));
    QShortcut* rightshortcut = new QShortcut(QKeySequence(Qt::Key_Right), ui->graphicsView);
    connect(rightshortcut, SIGNAL(activated()), this, SLOT(panright()));
    QShortcut* upshortcut = new QShortcut(QKeySequence(Qt::Key_Up), ui->graphicsView);
    connect(upshortcut, SIGNAL(activated()), this, SLOT(panup()));
    QShortcut* downshortcut = new QShortcut(QKeySequence(Qt::Key_Down), ui->graphicsView);
    connect(downshortcut, SIGNAL(activated()), this, SLOT(pandown()));

}

void HardwareLayoutWidget::loadComponentlist(QListWidget *widget)
{
    QString itemName,itemID;

    widget->clear();

    foreach (QGraphicsItem *item,scene->items())
    {
        connectableHardware *h = qgraphicsitem_cast<connectableHardware *>(item);
        if (h)
        {
            itemID = h->getID();
            itemName = h->getName();
        }

        connectableCable *c = qgraphicsitem_cast<connectableCable *>(item);
        if (c)
        {
            itemID = c->getID();
            itemName = c->getName();
        }

        connectableGraphic *g = qgraphicsitem_cast<connectableGraphic *>(item);
        if (g)
        {
            itemID = g->getID();
            itemName = g->getName();
        }

        QListWidgetItem *newItem = new QListWidgetItem;
        newItem->setText(itemName);
        QVariant id(itemID);
        newItem->setData(Qt::UserRole, id);
        ui->componentslistWidget->insertItem(ui->componentslistWidget->count(), newItem);

    }
}

void HardwareLayoutWidget::SelectionChanged()
{
    qDebug() << tr("slot Selection Change, %1 items selected").arg(scene->selectedItems().count());

    QString itemName, itemWidth, itemHeight, itemImage, itemPinCount,  ItemColumns, ItemPosition(tr("<unassigned>")), itemOrientation(tr("Normal")),
            itemConnectionPoint(tr("Top Right"));

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
            itemConnectionPoint = getConnectionPointNames()[h->getPrimaryConnectionIndex()];
        }

        connectableCable *cbl = qgraphicsitem_cast<connectableCable *>(scene->selectedItems()[0]);
        if (cbl)
        {
            itemName = cbl->getName();
            itemPinCount = tr("%1").arg(cbl->getWireCount());
            ItemColumns =  tr("%1").arg(cbl->getRows());
            // itemPinAssignments = cbl->getPinAssignments();
        }

        connectableGraphic *gfx = qgraphicsitem_cast<connectableGraphic *>(scene->selectedItems()[0]);
        if (gfx)
        {
            itemName = gfx->getName();
            itemImage = gfx->getImageFilename();
            itemWidth = tr("%1").arg(gfx->getWidth());
            itemHeight = tr("%1").arg(gfx->getHeight());
            itemConnectionPoint = getConnectionPointNames()[gfx->getPrimaryConnectionIndex()];
        }

        ui->componentslistWidget->clearSelection();
        QList<QListWidgetItem *> founditems = ui->componentslistWidget->findItems(itemName, Qt::MatchExactly);
        if (founditems.size() > 0) {
            // An item found
            founditems[0]->setSelected(true);
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

    clist = ui->PropertiestreeWidget->findItems(tr("Connects"), Qt::MatchContains|Qt::MatchRecursive, 0);
    if (clist.count()==1)
        clist[0]->setText(1,itemConnectionPoint);

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
    QStringList cpnames = getConnectionPointNames();

    scene->items().clear();

    int count = boardfile.value("overview/items",0).toInt();
    for (int i=0; i < count; i++)
    {
        boardfile.beginGroup(tr("Item_%1").arg(i+1));

        QString compID = boardfile.value("id").toString().toLower();
        QString compName = boardfile.value("name").toString();
        QString comptype = boardfile.value("type").toString().toLower();
        QString conctpnt = boardfile.value("connection_point").toString();
        int connectindex = cpnames.indexOf(conctpnt);

        qDebug() << "Connectionpoint:" << conctpnt;

        int x = boardfile.value("x",0).toInt();
        int y = boardfile.value("y",0).toInt();
        double w = boardfile.value("width",0).toFloat();
        double h = boardfile.value("height",0).toFloat();

        if (comptype == "hardware")
        {
            int p = boardfile.value("pins",0).toInt();
            int r = boardfile.value("rows",0).toInt();
            QString b = boardfile.value("board_file","").toString();
            QString i = boardfile.value("image_file","").toString();

            connectableHardware *hw = addToScene(compID,compName,x,y,b,i,w,h,p,r);

            hw->setPrimaryConnectionIndex(connectindex);

        }
        if (comptype == "graphic")
        {
            QString i = boardfile.value("image_file","").toString();

            connectableGraphic *g = addGraphicToScene(compID,compName,x,y,i,w,h);

            g->setPrimaryConnectionIndex(connectindex);

        }

        boardfile.endGroup();
    }

    for (int i=0; i < count; i++)
    {
        boardfile.beginGroup(tr("Item_%1").arg(i+1));

        QString compID = boardfile.value("id").toString().toLower();
        QString compName = boardfile.value("name").toString();
        QString comptype = boardfile.value("type").toString().toLower();

        if (comptype == "cable")
        {
            qDebug() << tr(" - cable item:%1").arg(i+1);

            QString si = boardfile.value("startitem","").toString();
            QString ei = boardfile.value("enditem","").toString();
            int wc = boardfile.value("wires",0).toInt();
            int rc = boardfile.value("rows",0).toInt();
            QString c = boardfile.value("color",0).toString();
            QColor cv(Qt::gray);

            addCableToScene(compID,compName,si,ei,wc,rc,cv);

        }

        boardfile.endGroup();
    }

    loadComponentlist(ui->componentslistWidget);

    return(false);
}

bool HardwareLayoutWidget::SaveComponents(QString filename)
{

    QSettings boardfile(filename, QSettings::IniFormat);
    QStringList cpnames = getConnectionPointNames();

    boardfile.setValue("overview/items",scene->items().count());

    int i(0);
    foreach (QGraphicsItem *item, scene->items())
    {

        boardfile.beginGroup(tr("Item_%1").arg(i+1));
        boardfile.remove("");

        connectableHardware *h = qgraphicsitem_cast<connectableHardware *>(item);
        if (h)
        {
            boardfile.setValue("id",h->getID());
            boardfile.setValue("name",h->getName());
            boardfile.setValue("type","Hardware");
            boardfile.setValue("width",h->getWidth());
            boardfile.setValue("height",h->getHeight());
            boardfile.setValue("pins",h->getPinCount());
            boardfile.setValue("rows",h->getRowCount());

            boardfile.setValue("board_file",h->getBoardFile());
            boardfile.setValue("image_file",h->getImageFilename());

            boardfile.setValue("x",item->x());
            boardfile.setValue("y",item->y());

            boardfile.setValue("connection_point",cpnames[h->getPrimaryConnectionIndex()]);

        }

        connectableCable *c = qgraphicsitem_cast<connectableCable *>(item);
        if (c)
        {
            boardfile.setValue("id",c->getID());
            boardfile.setValue("name",c->getName());
            boardfile.setValue("type","Cable");

            QGraphicsItem *startitem = c->getStartItem();
            QGraphicsItem *enditem = c->getEndItem();

            connectableHardware *sc = qgraphicsitem_cast<connectableHardware *>(startitem);
            if (sc)
                boardfile.setValue("startitem",sc->getID());

            connectableGraphic *sg = qgraphicsitem_cast<connectableGraphic *>(startitem);
            if (sg)
                boardfile.setValue("startitem",sg->getID());

            connectableHardware *ec = qgraphicsitem_cast<connectableHardware *>(enditem);
            if (ec)
                boardfile.setValue("enditem",ec->getID());

            connectableGraphic *eg = qgraphicsitem_cast<connectableGraphic *>(enditem);
            if (eg)
                boardfile.setValue("enditem",eg->getID());

            boardfile.setValue("color",c->getColor().name(QColor::HexRgb));
            boardfile.setValue("wirecount",c->getWireCount());
            boardfile.setValue("rows",c->getRows());

        }

        connectableGraphic *g = qgraphicsitem_cast<connectableGraphic *>(item);
        if (g)
        {
            boardfile.setValue("id",g->getID());
            boardfile.setValue("name",g->getName());
            boardfile.setValue("type","Graphic");
            boardfile.setValue("width",g->getWidth());
            boardfile.setValue("height",g->getHeight());
            boardfile.setValue("image_file",g->getImageFilename());

            boardfile.setValue("x",item->x());
            boardfile.setValue("y",item->y());

            boardfile.setValue("connection_point",cpnames[g->getPrimaryConnectionIndex()]);

        }

        boardfile.endGroup();

        i++;

    }

    qDebug() << "Saved";

    return(false);
}

void HardwareLayoutWidget::deleteComponent(QString ID)
{
    QGraphicsItem *item = findByID(ID);

    qDebug() << "Removing Item";

    if (item)
    {
        scene->clearSelection();
        scene->removeItem(item);
    }
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

QList <connectableGraphic *> HardwareLayoutWidget::getGraphicComponents()
{
    QList <connectableGraphic *> results;

    for (int i=0; i < scene->items().count(); i++)
    {
        QGraphicsItem *item = scene->items()[i];

        connectableGraphic *gfx = qgraphicsitem_cast<connectableGraphic *>(item);
        if (gfx)
        {
            results.append(gfx);
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

QGraphicsItem* HardwareLayoutWidget::findByID(QString componentID)
{
    QGraphicsItem* result = nullptr;

    foreach (QGraphicsItem *item, scene->items())
    {
        connectableHardware *h = qgraphicsitem_cast<connectableHardware *>(item);
        if (h)
        {
            if (h->getID() == componentID)
            {
                result = h;
                break;
            }
        }
        connectableGraphic *g = qgraphicsitem_cast<connectableGraphic *>(item);
        if (g)
        {
            if (g->getID() == componentID)
            {
                result = g;
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

QPoint connectableHardware::getPrimaryConnectionPoint()
{
    QPoint result(0.0,0.0);

    if (m_connectionpoints.size())
        return(m_connectionpoints[0]);
    else
        return(result);
}

void connectableHardware::setPrimaryConnectionPoint(QPoint point)
{
    m_connectionpoints.append(point);
}

void connectableHardware::setPrimaryConnectionIndex(int index)
{
    QPoint cpoint;

    switch (index)
    {
        case 0:     // Top Left
                    cpoint.setX(0);
                    cpoint.setY(0);
                    break;

        case 1:     // Top centre
                    cpoint.setX(getWidth() / 2);
                    cpoint.setY(0);
                    break;

        case 2:     // Top right
                    cpoint.setX(getWidth());
                    cpoint.setY(0);
                    break;

        case 3:     // middle left
                    cpoint.setX(0);
                    cpoint.setY(getHeight() / 2);
                    break;

        case 4:     // centre
                    cpoint.setX(getWidth() / 2);
                    cpoint.setY(getHeight() / 2);
                    break;

        case 5:     // middle right
                    cpoint.setX(getWidth());
                    cpoint.setY(getHeight() / 2);
                    break;

        case 6:     // bottom left
                    cpoint.setX(0);
                    cpoint.setY(getHeight());
                    break;

        default:
        case 7:     // bottom centre
                    cpoint.setX(getWidth() / 2);
                    cpoint.setY(getHeight());
                    break;

        case 8:     // bottom right
                    cpoint.setX(getWidth());
                    cpoint.setY(getHeight());
                    break;

    }

    m_connectionpoint = index;

    // Only one connection point is supported
    if (m_connectionpoints.size() == 0)
        m_connectionpoints.append(cpoint);
    else
        m_connectionpoints[0] = cpoint;
}

QStringList HardwareLayoutWidget::getConnectionPointNames()
{
    QStringList items;
    items << tr("Top Left") << tr("Top Centre") << tr("Top Right");
    items << tr("Left Centre") << tr("Centre") << tr("Right Centre");
    items << tr("Bottom Left") << tr("Bottom Centre") << tr("Bottom Right");
    return(items);
}

void HardwareLayoutWidget::on_PropertiestreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    bool ok;

    QString propertyname(item->text(0)),text;

    if (propertyname == tr("Connects"))
    {

        int selectedindex;
        bool ok;
        QStringList items = getConnectionPointNames();

        connectableHardware *hw = qgraphicsitem_cast<connectableHardware *>(scene->selectedItems()[0]);
        if (hw)
            selectedindex = hw->getPrimaryConnectionIndex();

        QString itemselected = QInputDialog::getItem(this, tr("Cable Connects to.."),
                                             tr("Connect to"), items, selectedindex, false, &ok);
        if (ok && !itemselected.isEmpty())
        {
            text = itemselected;
        }

        if (ok)
        {
            item->setText(1,text);
            if (hw)
                hw->setPrimaryConnectionIndex(items.indexOf(itemselected));

            connectableGraphic *gfx = qgraphicsitem_cast<connectableGraphic *>(scene->selectedItems()[0]);
            if (gfx)
                gfx->setPrimaryConnectionIndex(items.indexOf(itemselected));
        }
    }
    else if (propertyname == tr("Name"))
    {
        text = QInputDialog::getText((QWidget *) this->parentWidget(), QString("Property"),
                                     QString(tr("%1 :").arg(item->text(0))), QLineEdit::Normal,
                                     item->text(1), &ok);

        if (ok)
        {
            item->setText(1,text);
            ui->componentslistWidget->selectedItems()[0]->setText(text);

            connectableHardware *hw = qgraphicsitem_cast<connectableHardware *>(scene->selectedItems()[0]);
            if (hw)
                hw->setName(text);

            connectableCable *cbl = qgraphicsitem_cast<connectableCable *>(scene->selectedItems()[0]);
            if (cbl)
                cbl->setName(text);

            connectableGraphic *gfx = qgraphicsitem_cast<connectableGraphic *>(scene->selectedItems()[0]);
            if (gfx)
                gfx->setName(text);
        }
    }
    else if (propertyname == tr("Width"))
    {
        double width;
        QGraphicsItem *selected = scene->selectedItems()[0];

        connectableHardware *h = qgraphicsitem_cast<connectableHardware *>(selected);
        if (h)
            width = h->getWidth();

        connectableGraphic *g = qgraphicsitem_cast<connectableGraphic *>(scene->selectedItems()[0]);
        if (g)
            width = g->getWidth();

        double d = QInputDialog::getDouble(this, tr("Property"),
                                           tr("Width:"), width, 0, 5000, 2, &ok);
        if (ok)
        {
            item->setText(1,QString::number(d));

            if (h)
                h->setWidth(d);
            if (g)
                g->setWidth(d);

        }
    }
    else if (propertyname == tr("Height"))
    {
        double height;
        QGraphicsItem *selected = scene->selectedItems()[0];

        connectableHardware *h = qgraphicsitem_cast<connectableHardware *>(selected);
        if (h)
            height = h->getWidth();

        connectableGraphic *g = qgraphicsitem_cast<connectableGraphic *>(scene->selectedItems()[0]);
        if (g)
            height = g->getWidth();

        double d = QInputDialog::getDouble(this, tr("Property"),
                                           tr("Height:"), height, 0, 5000, 2, &ok);
        if (ok)
        {
            item->setText(1,QString::number(d));

            if (h)
                h->setHeight(d);
            if (g)
                g->setHeight(d);

        }
    }
    else
    {
        text = QInputDialog::getText((QWidget *) this->parentWidget(), QString("Property"),
                                     QString(tr("%1 Value:").arg(item->text(0))), QLineEdit::Normal,
                                     item->text(1), &ok);

        if (ok)
        {
            item->setText(1,text);
        }
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

connectableHardware * HardwareLayoutWidget::addToScene(QString componentID, QString componentName, double x, double y, QString componentBoardFile, QString componentImageName, double componentWidth, double componentHeight, int pins, int rows)
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

    return(item);
}

connectableCable * HardwareLayoutWidget::addCableToScene(QString componentID, QString componentName, QString startItem, QString endItem, int wires, int rows, QColor cablecolor)
{

    if (componentID.length() == 0)
        componentID = getNextID();

    QGraphicsItem *c1 = findByID(startItem);
    QGraphicsItem *c2 = findByID(endItem);

    connectableCable *cable = new connectableCable(componentID, componentName, c1, c2, wires, rows, cablecolor);

    cable->setFlag(QGraphicsItem::ItemIsSelectable);
    cable->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    scene->addItem(cable);

    QListWidgetItem *newItem = new QListWidgetItem;
    newItem->setText(cable->getName());
    QVariant id(cable->getID());
    newItem->setData(Qt::UserRole, id);
    ui->componentslistWidget->insertItem(ui->componentslistWidget->count(), newItem);

    qDebug() << "Add to Scene Complete";

    return(cable);
}

connectableGraphic::connectableGraphic(QString ID, QString name, qreal width, qreal height, QString graphicfile, QGraphicsItem *parent)
    : QGraphicsItem(parent),
    m_id(ID), m_name(name),
    m_width(width), m_height(height),
    m_connectionpoint(0)
{
    if (graphicfile.length())
    {
        m_image = new QPixmap(graphicfile);
        m_imagefilename = graphicfile;
    }
}


void connectableGraphic::setPrimaryConnectionIndex(int index)
{
    QPoint cpoint;

    switch (index)
    {
        case 0:     // Top Left
                    cpoint.setX(0);
                    cpoint.setY(0);
                    break;

        case 1:     // Top centre
                    cpoint.setX(getWidth() / 2);
                    cpoint.setY(0);
                    break;

        case 2:     // Top right
                    cpoint.setX(getWidth());
                    cpoint.setY(0);
                    break;

        case 3:     // middle left
                    cpoint.setX(0);
                    cpoint.setY(getHeight() / 2);
                    break;

        case 4:     // centre
                    cpoint.setX(getWidth() / 2);
                    cpoint.setY(getHeight() / 2);
                    break;

        case 5:     // middle right
                    cpoint.setX(getWidth());
                    cpoint.setY(getHeight() / 2);
                    break;

        case 6:     // bottom left
                    cpoint.setX(0);
                    cpoint.setY(getHeight());
                    break;

        default:
        case 7:     // bottom centre
                    cpoint.setX(getWidth() / 2);
                    cpoint.setY(getHeight());
                    break;

        case 8:     // bottom right
                    cpoint.setX(getWidth());
                    cpoint.setY(getHeight());
                    break;

    }

    m_connectionpoint = index;

    // Only one connection point is supported
    if (m_connectionpoints.size() == 0)
        m_connectionpoints.append(cpoint);
    else
        m_connectionpoints[0] = cpoint;
}

connectableGraphic * HardwareLayoutWidget::addGraphicToScene(QString componentID, QString componentName, double x, double y, QString componentImageName, double componentWidth, double componentHeight)
{
    if (componentID.length() == 0)
        componentID = getNextID();

    connectableGraphic *item = new connectableGraphic(componentID,componentName,componentWidth,componentHeight,componentImageName);

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

    return(item);
}

void connectableGraphic::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(boundingRect(),m_image->toImage());
}

QRectF connectableGraphic::boundingRect() const
{
    return(QRectF(0,0,m_width, m_height));
}

QVariant connectableGraphic::itemChange(GraphicsItemChange change, const QVariant &value)
{
    qDebug() << "Change:" << change;

    if (change == GraphicsItemChange::ItemPositionChange)
    {
        foreach (connectableCable *cable, cables)
        {
            QGraphicsItem * g1 = cable->getStartItem();
            QGraphicsItem * g2 = cable->getEndItem();

            if (g1 == this)
            {
                connectableHardware *hw2 = qgraphicsitem_cast<connectableHardware *>(g2);
                if (hw2)
                {
                    cable->setLine(pos().x(),pos().y(),hw2->x(),hw2->y());
                }

                connectableGraphic *gf2 = qgraphicsitem_cast<connectableGraphic *>(g2);
                if (gf2)
                {
                    cable->setLine(pos().x(),pos().y(),gf2->x(),gf2->y());
                }

            } else
            {
                connectableHardware *hw1 = qgraphicsitem_cast<connectableHardware *>(g1);
                if (hw1)
                {
                    cable->setLine(hw1->x(),hw1->y(),pos().x(),pos().y());
                }

                connectableGraphic *gf1 = qgraphicsitem_cast<connectableGraphic *>(g1);
                if (gf1)
                {
                    cable->setLine(gf1->x(),gf1->y(),pos().x(),pos().y());
                }

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

void connectableGraphic::addCableConnection(connectableCable *cable)
{
    cables.append(cable);
}

QPoint connectableGraphic::getPrimaryConnectionPoint()
{
    QPoint result;

    if (m_connectionpoints.size())
        return(m_connectionpoints[0]);
    else
        return(result);
}

void connectableGraphic::setPrimaryConnectionPoint(QPoint point)
{
    m_connectionpoints.append(point);
}

void connectableGraphic::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "Clicked:" << this->m_name;

    this->setSelected(!this->isSelected());

    return QGraphicsItem::mousePressEvent(event);

}

void HardwareLayoutWidget::on_componentslistWidget_itemClicked(QListWidgetItem *item)
{
    scene->clearSelection();

    QVariant data = item->data(Qt::UserRole);
    QString id = data.toString();

    QGraphicsItem *sceneitem = findGraphicsItemByID(id);
    if (sceneitem)
        sceneitem->setSelected(true);

}

void HardwareLayoutWidget::print()
{

    QPrinter printer;
    if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::Antialiasing);
        scene->render(&painter);
    }

}

void HardwareLayoutWidget::printPreview()
{
    QPrinter printer;
    if (QPrintPreviewDialog(&printer).exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::Antialiasing);
        scene->render(&painter);
    }
}

void HardwareLayoutWidget::deleteItem()
{
    QMessageBox msgBox;

    if (ui->componentslistWidget->selectedItems().size() == 0)
        return;
    else if (ui->componentslistWidget->selectedItems().size() > 1)
    {
        msgBox.setText(tr("Please confirm that you wish to delete these items."));
        msgBox.setInformativeText(tr("Do you really wish to delete these items?"));
    } else
    {
        msgBox.setText(tr("Please confirm that you wish to delete this item."));
        msgBox.setInformativeText(tr("Do you really wish to delete this item?"));
    }

    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if (ret == QMessageBox::Ok)
    {

        foreach (QListWidgetItem *item, ui->componentslistWidget->selectedItems())
        {
            QVariant data = item->data(Qt::UserRole);
            QString id = data.toString();

            deleteComponent(id);

            delete item;
        }
    }
}

void HardwareLayoutWidget::zoomin()
{
    ui->graphicsView->scale(1.2,1.2);
}

void HardwareLayoutWidget::zoomout()
{
    ui->graphicsView->scale(0.8,0.8);
}

void HardwareLayoutWidget::panleft()
{
    ui->graphicsView->setSceneRect(ui->graphicsView->sceneRect().x()-10,
                                   ui->graphicsView->sceneRect().y(),
                                   ui->graphicsView->sceneRect().width(),
                                   ui->graphicsView->sceneRect().height());
}

void HardwareLayoutWidget::panright()
{
    ui->graphicsView->setSceneRect(ui->graphicsView->sceneRect().x()+10,
                                   ui->graphicsView->sceneRect().y(),
                                   ui->graphicsView->sceneRect().width(),
                                   ui->graphicsView->sceneRect().height());
}

void HardwareLayoutWidget::panup()
{
    ui->graphicsView->setSceneRect(ui->graphicsView->sceneRect().x(),
                                   ui->graphicsView->sceneRect().y()-10,
                                   ui->graphicsView->sceneRect().width(),
                                   ui->graphicsView->sceneRect().height());
}

void HardwareLayoutWidget::pandown()
{
    ui->graphicsView->setSceneRect(ui->graphicsView->sceneRect().x(),
                                   ui->graphicsView->sceneRect().y()+10,
                                   ui->graphicsView->sceneRect().width(),
                                   ui->graphicsView->sceneRect().height());
}

