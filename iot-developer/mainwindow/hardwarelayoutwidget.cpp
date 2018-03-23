#include "hardwarelayoutwidget.h"
#include "ui_hardwarelayoutwidget.h"

HardwareLayoutWidget::HardwareLayoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareLayoutWidget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    QBrush greenbrush(Qt::green);
    QPen blackpen(Qt::black);
    blackpen.setWidth(3);

    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(":/res/res/mainboard-rpi3.PNG"));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(item);

    QGraphicsRectItem *ioboard;
    ioboard = scene->addRect(-100,-100,50,50,blackpen,greenbrush);
    ioboard->setFlag(QGraphicsItem::ItemIsMovable);

}

HardwareLayoutWidget::~HardwareLayoutWidget()
{
    delete ui;
}

bool HardwareLayoutWidget::LoadComponents(const QString filename)
{

}

bool HardwareLayoutWidget::SaveComponents(const QString filename)
{

}
