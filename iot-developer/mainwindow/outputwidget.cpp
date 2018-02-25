#include "outputwidget.h"
#include "ui_outputwidget.h"

OutputWidget::OutputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OutputWidget)
{
    ui->setupUi(this);
}

OutputWidget::~OutputWidget()
{
    delete ui;
}
