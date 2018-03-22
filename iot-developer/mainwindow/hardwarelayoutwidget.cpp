#include "hardwarelayoutwidget.h"
#include "ui_hardwarelayoutwidget.h"

HardwareLayoutWidget::HardwareLayoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareLayoutWidget)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    QGraphicsEllipseItem *elipse;
    QGraphicsRectItem *mainboard;

    QBrush redbrush(Qt::red);
    QBrush greenbrush(Qt::green);
    QPen blackpen(Qt::black);
    blackpen.setWidth(3);

    // QPixmap::fromImage(image)
    QGraphicsPixmapItem *item = new QGraphicsPixmapItem(QPixmap(":/res/res/device-32.png"));
    item->setFlag(QGraphicsItem::ItemIsMovable);
    scene->addItem(item);

//    elipse = scene->addEllipse(50,10,100,100,blackpen,redbrush);
//    elipse->setFlag(QGraphicsItem::ItemIsMovable);

    mainboard = scene->addRect(-100,-100,50,50,blackpen,greenbrush);
    mainboard->setFlag(QGraphicsItem::ItemIsMovable);

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
