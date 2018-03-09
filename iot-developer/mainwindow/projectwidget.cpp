#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QProcess>
#include <QDebug>
#include <QStringList>

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

    if (builder)
        delete builder;

}

void ProjectWidget::LoadProject(const QString dir)
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

void ProjectWidget::BuildProject(const QString buildspecifier)
{
    QString make("C:\\Qt\\Tools\\mingw530_32\\bin\\mingw32-make.exe");
    QStringList makeparams;

    makeparams << "-f" << "Makefile";

    if (!builder)
        builder = new QProcess(this);

    builder->setProcessChannelMode(QProcess::MergedChannels);
    builder->setWorkingDirectory(mainwindow->currentProject->getProjectDir());
    builder->start(make,makeparams);

    if (!builder->waitForFinished())
    {
        qDebug() << "Build failed" << builder->errorString();

    } else
    {
        qDebug() << "Build Succeeded" << builder->readAll();
    }

}

void ProjectWidget::on_projectFileList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString filename(mainwindow->currentProject->getProjectDir() + "/" + item->text(column));

    mainwindow->LoadCodeSource(filename);

}
