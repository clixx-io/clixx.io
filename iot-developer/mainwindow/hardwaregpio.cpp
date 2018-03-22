#include <QDebug>
#include <QInputDialog>

#include "hardwaregpio.h"
#include "ui_hardwaregpio.h"


HardwareGPIO::HardwareGPIO(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareGPIO)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

}

HardwareGPIO::~HardwareGPIO()
{
    if (scene)
        delete scene;

    delete ui;
}

void HardwareGPIO::createDefaultPins(const int pinCount, const int rowCount)
{
    QBrush pinbrush(Qt::cyan);
    QPen blackpen(Qt::black);

    scene->clear();

    int pinWidth = 10;
    int startX = -25;
    int startY = -20;
    int labelX = -70;
    int labelY = -25;
    int labelHeight = 10;
    int labelColWidth = 80;
    int pinsPerRow = pinCount / rowCount;

    for (int r=0; r<rowCount; r++ )
    {
        for (int c=0; c<pinsPerRow; c++)
        {

            QGraphicsTextItem *newlabel = scene->addText(tr("Pin %1").arg((r*pinsPerRow)+c+1));
            newlabel->setX(labelX + (r * labelColWidth));
            newlabel->setY(labelY + (c * labelHeight));

            gpioPIN *mypin = new gpioPIN(nullptr,
                                         startX + (r * pinWidth),
                                         startY + (c * pinWidth),
                                         (r*pinsPerRow)+c+1,
                                         tr("Pin %1").arg((r*pinsPerRow)+c+1),
                                         newlabel);

            scene->addItem(mypin);

        }
    }

}

bool HardwareGPIO::LoadAssignments(const QString filename)
{

    return(true);

}

bool HardwareGPIO::SaveAssignments(const QString filename)
{
    foreach (QGraphicsItem *i, scene->items())
    {
        qDebug() << "x:" << i->x() << "," << i->y();
    }

    return(true);
}

gpioPIN::gpioPIN(QGraphicsItem *parent, qreal x, qreal y, int number, QString name, QGraphicsTextItem *label)
 : pinname(name),pinnumber(number),pinlabel(label)
{
    setX(x);
    setY(y);
}

void gpioPIN::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::black);

    if (pinname.toLower() == "vcc")
        painter->setBrush(Qt::red);
    else if (pinname.toLower() == "gnd")
        painter->setBrush(Qt::black);
    else if (pinname.toLower() == "nc")
        painter->setBrush(Qt::gray);
    else if ((pinname.toLower() == "sda") || (pinname.toLower() == "scl"))
        painter->setBrush(Qt::blue);
    else
        painter->setBrush(Qt::cyan);

    painter->drawRect(0,0,10,10);
}

QRectF gpioPIN::boundingRect() const
{
    return(QRectF(0,0,10,10));
}

void gpioPIN::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{

    bool ok;
    QString text = QInputDialog::getText((QWidget *) this->parentWidget(), QString("QInputDialog::getText()"),
                                         QString("Pin name:"), QLineEdit::Normal,
                                         pinname, &ok);
    if (ok && !text.isEmpty())
    {
        pinname = text;
        pinlabel->setPlainText(text);
    }

}
