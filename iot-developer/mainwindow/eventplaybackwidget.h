#ifndef EVENTPLAYBACKWIDGET_H
#define EVENTPLAYBACKWIDGET_H

#include <QWidget>

namespace Ui {
class EventPlaybackWidget;
}

class EventPlaybackWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventPlaybackWidget(QWidget *parent = 0);
    ~EventPlaybackWidget();

private:
    Ui::EventPlaybackWidget *ui;
};

#endif // EVENTPLAYBACKWIDGET_H
