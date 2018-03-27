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

class connectableHardware : public QGraphicsPixmapItem
{
public:

//    connectableHardware(QGraphicsItem *parent, qreal x, qreal y, QString name, QGraphicsTextItem *label);
    connectableHardware(const QPixmap &pixmap, QGraphicsItem *parent = Q_NULLPTR);

    QString pinname;
    QGraphicsTextItem *pinlabel;

    QGraphicsLineItem *joiner = nullptr;

    int hardwareType;
    bool mainboard;


protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:

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

private slots:
    void on_PropertiestreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::HardwareLayoutWidget *ui;

    QGraphicsScene *scene = nullptr;

    int unitSystem;     // 0=mm, 1=100mil/0.1"

    // QList<QGraphicsItem*> components;

};

#endif // HARDWARELAYOUTWIDGET_H
