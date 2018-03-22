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

class HardwareLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareLayoutWidget(QWidget *parent = 0);
    ~HardwareLayoutWidget();

    bool LoadComponents(const QString filename);
    bool SaveComponents(const QString filename);

private:
    Ui::HardwareLayoutWidget *ui;

    QGraphicsScene *scene = nullptr;

    // QList<QGraphicsItem> componentList;


};

#endif // HARDWARELAYOUTWIDGET_H
