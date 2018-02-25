#include "projectwidget.h"
#include "ui_projectwidget.h"

ProjectWidget::ProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWidget)
{
    ui->setupUi(this);
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}
