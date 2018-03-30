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

    connectableHardware(QString name, qreal width, qreal height, QString guid, QString graphicfile, QGraphicsItem *parent = Q_NULLPTR);

    QGraphicsLineItem *joiner = nullptr;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual QRectF boundingRect() const;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    double m_width, m_height;
    QString m_id, m_name;
    QPixmap *m_image = nullptr;

    int hardwareType;
    bool mainboard;

};

class connectableCable : public QGraphicsItem
{
public:

    connectableCable(QString cableName, QColor cableColor, int pins, int rows, QGraphicsItem *parent = Q_NULLPTR);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual QRectF boundingRect() const;

};

class HardwareLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareLayoutWidget(QWidget *parent = 0);
    ~HardwareLayoutWidget();

    bool LoadComponents(const QString filename);
    bool SaveComponents(const QString filename);

    bool addToScene(QString componentID, QString componentName, QString componentImageName, double componentWidth, double componentHeight, int pins, int rows);
    bool addCableToScene(QString cableName, QColor cableColor, int pins, int rows);

private slots:
    void on_PropertiestreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);
    void SelectionChanged();

private:
    Ui::HardwareLayoutWidget *ui;

    QGraphicsScene *scene = nullptr;

    int unitSystem;     // 0=mm, 1=100mil/0.1"

    // QList<QGraphicsItem*> components;

};

#endif // HARDWARELAYOUTWIDGET_H
