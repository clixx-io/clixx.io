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

QT_FORWARD_DECLARE_CLASS(connectableCable)

namespace Ui {
class HardwareLayoutWidget;
}

class connectableHardware : public QGraphicsItem
{
public:

    connectableHardware(QString ID, QString name, QString boardfile, int pins, int rows, qreal width, qreal height, QString graphicfile, QGraphicsItem *parent = Q_NULLPTR);

    QGraphicsLineItem *joiner = nullptr;
    QList <connectableCable *> cables;

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

    void addCableConnection(connectableCable *cable);

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
    QPixmap *m_image = nullptr;
    int m_pins, m_rows;

    QStringList m_gpiopin_names;

    int hardwareType;
    bool mainboard;

};

// class connectableCable : public QGraphicsPathItem
class connectableCable : public QGraphicsLineItem
{
public:

    connectableCable(QString componentID, connectableHardware *startItem, connectableHardware *endItem, int wires=-1, int rows=-1, QColor cablecolor=QColor(12,56,99), QGraphicsItem *parent = Q_NULLPTR);

    enum { Type = UserType + 2 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QString getID(){ return(m_id); }
    QString getName(){ return(m_name); }
    connectableHardware *getStartItem(){ return(m_startItem); }
    connectableHardware *getEndItem(){ return(m_endItem); }
    QString getType(){ return(m_type); }
    QColor getColor(){ return m_cablecolor; }
    int getWireCount(){ return m_wires; }
    int getRows(){ return(m_rows); }

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    /*
    virtual QRectF boundingRect() const;
    */

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    connectableHardware *m_startItem = nullptr, *m_endItem = nullptr;
    QString m_id, m_name, m_type;
    QColor m_cablecolor;
    int m_wires, m_rows;

};

class HardwareLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareLayoutWidget(QWidget *parent = 0);
    ~HardwareLayoutWidget();

    bool LoadComponents(const QString filename = "hardware.layout");
    bool SaveComponents(const QString filename = "hardware.layout");

    bool addToScene(QString componentID, QString componentName, double x, double y, QString componentBoardFile, QString componentImageName, double componentWidth, double componentHeight, int pins, int rows);
    bool addCableToScene(QString componentID, QString startItem, QString endItem, int wires, int rows, QColor cablecolor = QColor(255, 0, 0, 127));

    connectableHardware *findByID(QString componentID);
    connectableHardware *findByName(QString componentName);
    QString getNextID(){ return(QString::number(scene->items().count()+1));}
    QList <connectableHardware *> getHardwareComponents();

private slots:
    void on_PropertiestreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void SelectionChanged();

private:
    Ui::HardwareLayoutWidget *ui;

    QGraphicsScene *scene = nullptr;
    QString m_filename;

    int m_unitSystem;     // 0=mm, 1=100mil/0.1"

};

#endif // HARDWARELAYOUTWIDGET_H
