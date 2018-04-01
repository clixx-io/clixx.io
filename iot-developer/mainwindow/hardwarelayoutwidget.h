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

namespace Ui {
class HardwareLayoutWidget;
}

class connectableHardware : public QGraphicsItem
{
public:

    connectableHardware(QString ID, QString name, QString boardfile, int pins, int rows, qreal width, qreal height, QString graphicfile, QGraphicsItem *parent = Q_NULLPTR);

    QGraphicsLineItem *joiner = nullptr;

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

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void copyBoardFileProperties(QString boardfilename);

private:
    QString m_id, m_name, m_boardfile,m_type,m_imagefilename;
    double m_width, m_height;
    QPixmap *m_image = nullptr;
    int m_pins, m_rows;

    QStringList m_gpiopin_names;

    int hardwareType;
    bool mainboard;

};

class connectableCable : public QGraphicsItem
{
public:

    connectableCable(QString startItem, QString endItem, QGraphicsItem *parent = Q_NULLPTR);

    enum { Type = UserType + 2 };
    int type() const
    {
        // Enable the use of qgraphicsitem_cast with this item.
        return Type;
    }

    QString getID(){ return(m_id); }
    QString getName(){ return(m_name); }
    QString getStartItem(){ return(m_startItem); }
    QString getEndItem(){ return(m_endItem); }
    QString getType(){ return(m_type); }
    QColor getColor(){ return m_cablecolor; }
    int getWireCount();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

private:
    QString m_startItem, m_endItem;
    QString m_id, m_name, m_type;
    QColor m_cablecolor;
    int m_pins, m_rows;

};

class HardwareLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareLayoutWidget(QWidget *parent = 0);
    ~HardwareLayoutWidget();

    bool LoadComponents(const QString filename);
    bool SaveComponents(const QString filename);

    bool addToScene(QString componentID, QString componentName, QString componentBoardFile, QString componentImageName, double componentWidth, double componentHeight, int pins, int rows);
    bool addCableToScene(QString cableName, QColor cableColor, int pins, int rows);

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
