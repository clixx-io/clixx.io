#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QWidget>

namespace Ui {
class OutputWidget;
}

class OutputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OutputWidget(QWidget *parent = 0);
    ~OutputWidget();

private:
    Ui::OutputWidget *ui;
};

#endif // OUTPUTWIDGET_H
