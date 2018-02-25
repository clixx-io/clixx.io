#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>

namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidget(QWidget *parent = 0);
    ~ProjectWidget();

private:
    Ui::ProjectWidget *ui;
};

#endif // PROJECTWIDGET_H
