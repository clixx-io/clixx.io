#ifndef HARDWARELAYOUTWIDGET_H
#define HARDWARELAYOUTWIDGET_H

#include <QWidget>

namespace Ui {
class HardwareLayoutWidget;
}

class HardwareLayoutWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HardwareLayoutWidget(QWidget *parent = 0);
    ~HardwareLayoutWidget();

private:
    Ui::HardwareLayoutWidget *ui;
};

#endif // HARDWARELAYOUTWIDGET_H
