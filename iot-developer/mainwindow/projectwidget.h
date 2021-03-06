#ifndef PROJECTWIDGET_H
#define PROJECTWIDGET_H

#include <QWidget>

QT_FORWARD_DECLARE_CLASS(MainWindow)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
QT_FORWARD_DECLARE_CLASS(QProcess)

namespace Ui {
class ProjectWidget;
}

class ProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWidget(QWidget *parent = 0);
    ~ProjectWidget();

    bool loadProject(const QString dir);
    bool buildProject(const QString buildspecifier);

    inline void setMainWindow(MainWindow *main) { mainwindow = main; }

    const QString hardwareLayoutFilename = "hardware.layout";

public slots:
    void deployProject();
    void cleanProject();
    void checkProject();
    void runProject();

private slots:
    void on_projectFileList_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::ProjectWidget *ui;

    MainWindow *mainwindow = nullptr;

    QProcess *builder = nullptr;

};

#endif // PROJECTWIDGET_H
