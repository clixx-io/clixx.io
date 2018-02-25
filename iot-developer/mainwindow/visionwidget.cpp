#include "visionwidget.h"
#include "ui_visionwidget.h"

VisionWidget::VisionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VisionWidget)
{
    ui->setupUi(this);
}

VisionWidget::~VisionWidget()
{
    delete ui;
}
