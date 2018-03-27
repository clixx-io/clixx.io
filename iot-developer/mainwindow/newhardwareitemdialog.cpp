#include "newhardwareitemdialog.h"
#include "ui_newhardwareitemdialog.h"

#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>

NewHardwareItemDialog::NewHardwareItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewHardwareItemDialog)
{
    ui->setupUi(this);
    loadBoardList();
}

NewHardwareItemDialog::~NewHardwareItemDialog()
{
    delete ui;
}

void NewHardwareItemDialog::on_toolButton_triggered(QAction *arg1)
{

}

QStringList NewHardwareItemDialog::loadBoardFiles()
{
    QStringList results;

    QDir dir("../tests/boards");

    qDebug() << "Reading directory" << dir.absolutePath();

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        // qDebug() << dir.absoluteFilePath(fileInfo.fileName());

        if (fileInfo.fileName().endsWith(".board") && (!fileInfo.fileName().startsWith(".")))
            results += dir.absoluteFilePath(fileInfo.fileName());

    }

    return(results);
}

bool NewHardwareItemDialog::loadBoardList()
{
    QStringList filenames;

    filenames = loadBoardFiles();

    foreach (QString filename, filenames)
    {
        QSettings boardfile(filename, QSettings::IniFormat);

        qDebug() << boardfile.value("overview/name","").toString();

        QString name = boardfile.value("overview/name","").toString();

        ui->BoardNameslistWidget->addItem(name);

    }
}

