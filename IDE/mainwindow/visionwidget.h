#ifndef VISIONWIDGET_H
#define VISIONWIDGET_H

#include <QWidget>

namespace Ui {
class VisionWidget;
}

class VisionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VisionWidget(QWidget *parent = 0);
    ~VisionWidget();

private:
    Ui::VisionWidget *ui;
};

#endif // VISIONWIDGET_H
