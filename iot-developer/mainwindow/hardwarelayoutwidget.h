#ifndef HARDWARELAYOUTWIDGET_H
#define HARDWARELAYOUTWIDGET_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>

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

private slots:
    void on_AddcommandLinkButton_clicked();

private:
    Ui::HardwareLayoutWidget *ui;

    QGraphicsScene *scene = nullptr;

    // QList<QGraphicsItem> componentList;


};

#endif // HARDWARELAYOUTWIDGET_H
