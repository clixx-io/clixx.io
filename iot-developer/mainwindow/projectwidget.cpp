#include <QDockWidget>
#include <QTreeWidgetItem>

#include "mainwindow.h"

#include "clixxiotprojects.h"
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

void ProjectWidget::LoadProject(QString dir)
{
    if (!dir.isEmpty()) {

        mainwindow->currentProject->setProjectDir(dir);

        // currentProject->setProjectDir(dir);
        // QStringList projectfiles = currentProject->listfiles();
        QStringList files = mainwindow->currentProject->listfiles();

        ui->projectFileList->clear();
        for(int i=0; i< files.size(); i++)
        {
            QTreeWidgetItem * item = new QTreeWidgetItem();
            item->setText(0,files[i]);
            ui->projectFileList->addTopLevelItem(item);
        }
    }
}

void ProjectWidget::on_projectFileList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString filename(mainwindow->currentProject->getProjectDir() + "/" + item->text(column));

    mainwindow->LoadCodeSource(filename);

}
