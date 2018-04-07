#include <QDockWidget>
#include <QTreeWidgetItem>
#include <QProcess>
#include <QDebug>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>

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

bool ProjectWidget::loadProject(const QString dir)
{
    bool cleanOption(false), allOption(false), transferOption(false), checkOption(false);

    if (!dir.isEmpty()) {

        mainwindow->currentProject->setProjectDir(dir);

        QStringList files = mainwindow->currentProject->listfiles();

        ui->projectFileList->clear();
        for(int i=0; i< files.size(); i++)
        {
            QTreeWidgetItem * item = new QTreeWidgetItem();
            item->setText(0,files[i]);
            ui->projectFileList->addTopLevelItem(item);
        }

        // -- From the list of files, see if we have something that we
        //    can recognise like a Makefile
        if (files.contains("Makefile"))
        {
            QStringList makefile;
            QString filename(mainwindow->currentProject->getProjectDir() + "/Makefile");
            QFile file(filename);

            if (file.open(QIODevice::ReadOnly))
            {
                while(!file.atEnd())
                    makefile.append(file.readLine());

                file.close();
            }
            else
            {
                mainwindow->showStatusMessage(tr("Error opening %1").arg(filename));
                return(false);
            }

            mainwindow->clearStatusMessages();

            // - Check the Makefile for all, clean, check
            foreach (QString line, makefile)
            {
                if (line.startsWith("all:"))
                {
                    allOption = true;
                }
                else if (line.startsWith("clean:"))
                {
                    cleanOption = true;
                }
                else if (line.startsWith("deploy") || (line.startsWith("transfer")))
                {
                    transferOption = true;
                }
                else if (line.startsWith("check:"))
                {
                    checkOption = true;
                }
            }

            QString makeoptions;
            if (allOption)
                makeoptions += "all,";
            if (cleanOption)
                makeoptions += "clean,";
            if (checkOption)
                makeoptions += "check";
            if (makeoptions.endsWith(','))
                makeoptions = makeoptions.left(makeoptions.length()-1);

            mainwindow->showStatusMessage(tr("GNU style makefile found with %1 options.").arg(makeoptions));

        }

        if (files.contains(hardwareLayoutFilename))
        {
            mainwindow->showStatusMessage(tr("Hardware Layout file found."));
            mainwindow->loadDesignDiagram();
        }

        if (QDir::setCurrent(mainwindow->currentProject->getProjectDir()))
        {
            mainwindow->showStatusMessage(tr("Using Project directory %1.").arg(QDir::currentPath()));
        } else
        {
            mainwindow->showStatusMessage(tr("Failed to change to %1 directory.").arg(mainwindow->currentProject->getProjectDir()));
            return(false);
        }

        mainwindow->setBuildButtonToggles(allOption,cleanOption,transferOption, checkOption);

    }

    return(true);
}

bool ProjectWidget::buildProject(const QString buildspecifier)
{

#ifdef Q_OS_WIN32
    // TODO : fix this
    QString make("mingw32-make.exe");
#else
    QString make("make");
#endif

    QStringList makeparams;
    makeparams << "-f" << "Makefile" << buildspecifier;

    if (!builder)
        builder = new QProcess(this);

    builder->setProcessChannelMode(QProcess::MergedChannels);
    builder->setWorkingDirectory(mainwindow->currentProject->getProjectDir());
    builder->start(make,makeparams);

    if (!builder->waitForFinished())
    {
        mainwindow->showStatusMessage(tr("Build failed - %1").arg(builder->errorString()));

    } else
    {
        QString processOutput(builder->readAll());

        mainwindow->showStatusMessage(tr("Build succeeded - %1").arg(processOutput));
    }

}

void ProjectWidget::deployProject()
{
    QMessageBox msgBox(QMessageBox::Critical, tr("Problem"), tr("Not yet implemented"),QMessageBox::Ok);
    msgBox.exec();

    return;
}

void ProjectWidget::cleanProject()
{
    return;
}

void ProjectWidget::checkProject()
{
    return;
}

void ProjectWidget::runProject()
{
    return;
}

void ProjectWidget::on_projectFileList_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString filename(mainwindow->currentProject->getProjectDir() + "/" + item->text(column));

    mainwindow->LoadCodeSource(filename);

}
