#include "eventplaybackwidget.h"
#include "ui_eventplaybackwidget.h"

EventPlaybackWidget::EventPlaybackWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventPlaybackWidget)
{
    ui->setupUi(this);
}

EventPlaybackWidget::~EventPlaybackWidget()
{
    delete ui;
}
