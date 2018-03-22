#ifndef HARDWAREGPIO_H
#define HARDWAREGPIO_H

#include <QWidget>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QList>

namespace Ui {
class HardwareGPIO;
}

class gpioPIN : public QGraphicsItem
{
public:
    gpioPIN(QGraphicsItem *parent, qreal x, qreal y, int number, QString name, QGraphicsTextItem *label);

    int pinnumber;
    QString pinname;
    QGraphicsTextItem *pinlabel;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    QRectF boundingRect() const;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:

};

class HardwareGPIO : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareGPIO(QWidget *parent = 0);
    ~HardwareGPIO();

    bool LoadAssignments(const QString filename);
    bool SaveAssignments(const QString filename);
    void createDefaultPins(const int pinCount, const int rowCount);

    int itemCount(){if (!scene) return(0); else return(scene->items().length());}

private:
    Ui::HardwareGPIO *ui;

    QGraphicsScene *scene = nullptr;

};

#endif // HARDWAREGPIO_H

