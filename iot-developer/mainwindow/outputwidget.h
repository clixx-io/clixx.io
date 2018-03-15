#ifndef OUTPUTWIDGET_H
#define OUTPUTWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(MainWindow)

namespace Ui {
class OutputWidget;
}

class OutputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit OutputWidget(QWidget *parent = 0);
    ~OutputWidget();

    inline void setMainWindow(MainWindow *main) { mainwindow = main; }

private:
    Ui::OutputWidget *ui;

    MainWindow *mainwindow = nullptr;

};

#endif // OUTPUTWIDGET_H
