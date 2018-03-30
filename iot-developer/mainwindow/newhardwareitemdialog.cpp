#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QJsonObject>
#include <QFileDialog>

#include "newhardwareitemdialog.h"
#include "ui_newhardwareitemdialog.h"

NewHardwareItemDialog::NewHardwareItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewHardwareItemDialog)
{
    ui->setupUi(this);
}

NewHardwareItemDialog::NewHardwareItemDialog(QWidget *parent, QJsonObject *results) :
    QDialog(parent),
    ui(new Ui::NewHardwareItemDialog)
{
    ui->setupUi(this);

    completed = results;

    loadBoardList();
}

NewHardwareItemDialog::~NewHardwareItemDialog()
{
    delete ui;
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


void NewHardwareItemDialog::on_buttonBox_accepted()
{
    QJsonObject object
    {
        {"name", ui->NamelineEdit->text()},
        {"width", ui->WidthSpinBox->value()},
        {"height", ui->HeightSpinBox->value()},
        {"pins", ui->pinscomboBox->currentText().toInt()},
        {"rows", ui->rowscomboBox->currentText().toInt()},
        {"picturefilename", imagefilename}

    };

    if (ui->BoardNameslistWidget->selectedItems().count() > 0)
        object["type"] = QString(ui->BoardNameslistWidget->selectedItems()[0]->text());
    else
        object["type"] = "";

    *completed = object;
}

void NewHardwareItemDialog::on_toolButton_triggered(QAction *arg1)
{
}

void NewHardwareItemDialog::on_toolButton_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(
                            this,
                            "Select an Image to load",
                            "..",
                            "Images (*.png *.xpm *.jpg)");

    if (files.count()>0)
    {
        QPixmap pixmap(files[0]);

        ui->ComponentPicturelabel->setPixmap(pixmap);
        ui->ComponentPicturelabel->setScaledContents(true);
        ui->ComponentPicturelabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored );

        ui->ComponentPicturelabel->update();

        imagefilename = files[0];
    }

    qDebug() << files[0] << "loaded";

}
