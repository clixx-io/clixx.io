#ifndef HARDWARELAYOUTWIDGET_H
#define HARDWARELAYOUTWIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QListWidgetItem>

QT_FORWARD_DECLARE_CLASS(connectableCable)

namespace Ui {
class HardwareLayoutWidget;
}

class connectableHardware : public QGraphicsItem
{
public:

    connectableHardware(QString ID, QString name, QString boardfile, int pins, int rows, qreal width, qreal height, QString graphicfile, QGraphicsItem *parent = Q_NULLPTR);
    ~connectableHardware();

    enum { Type = UserType + 1 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QString getID(){ return(m_id); }
    QString getName(){ return(m_name); }
    QString getType(){ return(m_type); }
    int getPinCount(){ return(m_pins); }
    int getRowCount(){ return(m_rows); }
    QString getBoardFile(){ return(m_boardfile); }
    QString getImageFilename(){ return(m_imagefilename); }
    double getWidth(){ return(m_width); }
    double getHeight(){ return(m_height); }
    QStringList getPinAssignments(){ return(m_gpiopin_names); }

    void setName(QString name){ m_name = name; }

    void addCableConnection(connectableCable *cable);

    QPoint getPrimaryConnectionPoint();
    void setPrimaryConnectionPoint(QPoint point);
    int getPrimaryConnectionIndex(){ return(m_connectionpoint);}
    void setPrimaryConnectionIndex(int index);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void copyBoardFileProperties(QString boardfilename);

private:
    QString m_id, m_name, m_boardfile, m_type, m_imagefilename;
    double m_width, m_height;

    QList <QPoint> m_connectionpoints;
    int m_connectionpoint;

    QPixmap *m_image = nullptr;
    int m_pins, m_rows;

    QStringList m_gpiopin_names;
    QList <connectableCable *> cables;

    int hardwareType;
    bool mainboard;

};

class connectableCable : public QGraphicsLineItem
{
public:

    connectableCable(QString componentID, QGraphicsItem *startItem, QGraphicsItem *endItem, int wires=-1, int rows=-1, QColor cablecolor=QColor(12,56,99), QGraphicsItem *parent = Q_NULLPTR);

    enum { Type = UserType + 2 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QString getID(){ return(m_id); }
    QString getName(){ return(m_name); }
    QGraphicsItem *getStartItem(){ return(m_startItem); }
    QGraphicsItem *getEndItem(){ return(m_endItem); }
    QString getType(){ return(m_type); }
    QColor getColor(){ return m_cablecolor; }
    int getWireCount(){ return m_wires; }
    int getRows(){ return(m_rows); }

    void setName(QString name){ m_name = name; }

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    QGraphicsItem *m_startItem = nullptr, *m_endItem = nullptr;
    QString m_id, m_name, m_type;
    QColor m_cablecolor;
    int m_wires, m_rows;

};

class connectableGraphic : public QGraphicsItem
{
public:

    connectableGraphic(QString ID, QString name, qreal width, qreal height, QString graphicfile, QGraphicsItem *parent = Q_NULLPTR);
//    connectableGraphic(QString ID, QGraphicsItem *parent = Q_NULLPTR);

    enum { Type = UserType + 3 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QString getID(){ return(m_id); }
    QString getName(){ return(m_name); }
    QString getImageFilename(){ return(m_imagefilename); }
    double getWidth(){ return(m_width); }
    double getHeight(){ return(m_height); }

    void setName(QString name){ m_name = name; }

    void addCableConnection(connectableCable *cable);

    QPoint getPrimaryConnectionPoint();
    void setPrimaryConnectionPoint(QPoint point);

    int getPrimaryConnectionIndex(){ return(m_connectionpoint);}
    void setPrimaryConnectionIndex(int index);

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    /*
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    */

private:
    QString m_id, m_name, m_imagefilename;
    double m_width, m_height;

    QList <QPoint> m_connectionpoints;
    int m_connectionpoint;

    QPixmap *m_image = nullptr;

    QList <connectableCable *> cables;

};

class HardwareLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareLayoutWidget(QWidget *parent = 0);
    ~HardwareLayoutWidget();

    bool LoadComponents(const QString filename = "hardware.layout");
    bool SaveComponents(const QString filename = "hardware.layout");

    connectableHardware *addToScene(QString componentID, QString componentName, double x, double y, QString componentBoardFile, QString componentImageName, double componentWidth, double componentHeight, int pins, int rows);
    connectableCable *addCableToScene(QString componentID, QString startItem, QString endItem, int wires, int rows, QColor cablecolor = QColor(255, 0, 0, 127));
    connectableGraphic * addGraphicToScene(QString componentID, QString componentName, double x, double y, QString componentImageName, double componentWidth, double componentHeight);

    QGraphicsItem *findByID(QString componentID);
    connectableHardware *findByName(QString componentName);
    QGraphicsItem* findGraphicsItemByID(QString componentID);
    QString getNextID(){ return(QString::number(scene->items().count()+1));}
    QString getNextName(QString prefix);
    QList <connectableHardware *> getHardwareComponents();
    QList <connectableGraphic *> getGraphicComponents();

    QStringList getConnectionPointNames();

    void deleteComponent(QString ID);

    void print();
    void printPreview();

private slots:
    void on_PropertiestreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void SelectionChanged();

    void on_componentslistWidget_itemClicked(QListWidgetItem *item);

    void deleteItem();
    void zoomin();
    void zoomout();
    void panleft();
    void panright();
    void panup();
    void pandown();

private:
    Ui::HardwareLayoutWidget *ui;

    QGraphicsScene *scene = nullptr;
    QString m_filename;

    int m_unitSystem;     // 0=mm, 1=100mil/0.1"

};

#endif // HARDWARELAYOUTWIDGET_H
