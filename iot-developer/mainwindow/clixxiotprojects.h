#ifndef CLIXXIOTPROJECTS_H
#define CLIXXIOTPROJECTS_H

#include <QStringList>
#include <QSettings>

class ClixxIoTProjects
{

public:
    ClixxIoTProjects();

    QString getProjectsDir();
    QStringList list();

};

class ClixxIoTProject
{
    QString name,
            projectpath;

protected:
    QSettings *settings;

public:
    ClixxIoTProject(QString projectname = "");

    QString getProjectDir();
    QString getprojectconfigpath();
    void setProjectDir(QString dirname);

    QStringList listfiles();

    int Open(QString projectname = "");
    int Close();

};

#endif // CLIXXIOTPROJECTS_H
