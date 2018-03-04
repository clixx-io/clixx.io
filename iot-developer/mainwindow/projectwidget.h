#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(MainWindow)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)

namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidget(QWidget *parent = 0);
    ~ProjectWidget();

    void LoadProject(QString dir);
    inline void setMainWindow(MainWindow *main) { mainwindow = main; }

private slots:
    void on_projectFileList_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::ProjectWidget *ui;

    MainWindow *mainwindow;
};

#endif // PROJECTWIDGET_H
