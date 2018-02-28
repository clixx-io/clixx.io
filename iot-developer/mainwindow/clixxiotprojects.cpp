#include "clixxiotprojects.h"

#include <QDir>

ClixxIoTProjects::ClixxIoTProjects()
{

}

QStringList ClixxIoTProjects::list()
{
    QStringList results;
    QSettings settings("clixx.io", "IoT Framework");

    settings.beginGroup("projects");
    QString workdir = settings.value("directory").toString();
    settings.endGroup();

    QDir dir(workdir);
    dir.setFilter(QDir::Dirs);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        if (!fileInfo.fileName().endsWith(".ini"))
            results += fileInfo.fileName();

    }

    return(results);

}

ClixxIoTProject::ClixxIoTProject(QString projectname)
{
    if (!projectname.length())
        return;

}

QString ClixxIoTProject::getprojectdirectory()
{
    return(projectpath);
}

QString ClixxIoTProject::getprojectconfigpath()
{
    return(projectpath + "/" + name + ".ini");
}

int ClixxIoTProject::Open(QString projectname)
{
    settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "clixx.io", projectname);

    return(0);
}

QStringList ClixxIoTProject::listfiles()
{

    QDir dir(projectpath);
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList results;

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        if (!fileInfo.fileName().endsWith(".ini"))
            results += fileInfo.fileName();

    }

    return(results);
}

int ClixxIoTProject::Close()
{
    return(0);
}

