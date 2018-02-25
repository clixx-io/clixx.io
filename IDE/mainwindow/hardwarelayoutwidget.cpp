#include "hardwarelayoutwidget.h"
#include "ui_hardwarelayoutwidget.h"

HardwareLayoutWidget::HardwareLayoutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HardwareLayoutWidget)
{
    ui->setupUi(this);
}

HardwareLayoutWidget::~HardwareLayoutWidget()
{
    delete ui;
}
