#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include <QJsonObject>
#include <QFileDialog>
#include <QClipboard>
#include <QMimeData>
#include <QStandardPaths>

#include "mainwindow.h"
#include "newhardwareitemdialog.h"
#include "ui_newhardwareitemdialog.h"

NewHardwareItemDialog::NewHardwareItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewHardwareItemDialog),
    havepastedimage(false),
    havesavedimage(false)
{
    ui->setupUi(this);
}

NewHardwareItemDialog::NewHardwareItemDialog(QWidget *parent, QJsonObject *results) :
    QDialog(parent),
    ui(new Ui::NewHardwareItemDialog),
    havepastedimage(false),
    havesavedimage(false)
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
    QSettings settings;
    QString dirname = settings.value("directories/board_library",QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/boardlibrary").toString();
    QDir dir(dirname);

    QFileInfoList list = dir.entryInfoList();
    if (list.count()==0)
    {
        MainWindow *mw = (MainWindow*) getMainWindow();
        if (mw)
            mw->showStatusMessage(tr("Warning: No boards found in directory \"%1\"").arg(dir.absolutePath()));
    }

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        if (fileInfo.fileName().endsWith(".board") && (!fileInfo.fileName().startsWith(".")))
            results += dir.absoluteFilePath(fileInfo.fileName());

    }

    return(results);
}

bool NewHardwareItemDialog::loadBoardList()
{
    componentfiles = loadBoardFiles();

    foreach (QString filename, componentfiles)
    {
        QSettings boardfile(filename, QSettings::IniFormat);

        qDebug() << boardfile.value("overview/name","").toString();

        QString name = boardfile.value("overview/name","").toString();

        QListWidgetItem *newItem = new QListWidgetItem;
        QString fullFilePath(filename);
        QVariant fullFilePathData(fullFilePath);
        newItem->setData(Qt::UserRole, fullFilePathData);
        newItem->setText(name);

        ui->BoardNameslistWidget->addItem(newItem);

    }
}

void NewHardwareItemDialog::on_buttonBox_accepted()
{

    QJsonObject object
    {
        {"name", m_name },
        {"boardfile", m_boardfile},
        {"width", ui->WidthSpinBox->value()},
        {"height", ui->HeightSpinBox->value()},
        {"pins", ui->pinscomboBox->currentText().toInt()},
        {"rows", ui->rowscomboBox->currentText().toInt()},
        {"picturefilename", m_imagefilename}

    };

    if (ui->BoardNameslistWidget->selectedItems().count() > 0)
    {
        object["name"] = QString(ui->BoardNameslistWidget->selectedItems()[0]->text());
        object["type"] = QString(ui->BoardNameslistWidget->selectedItems()[0]->text());
    }
    else
        object["type"] = "";

    *completed = object;
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

        m_imagefilename = files[0];
    }

    qDebug() << files[0] << "loaded";

}

void NewHardwareItemDialog::on_PastetoolButton_pressed()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if (mimeData->hasImage()) {
        ui->ComponentPicturelabel->setPixmap(qvariant_cast<QPixmap>(mimeData->imageData()));
    }
    else
    {
        qDebug() << tr("Cannot display data");
    }
}

void NewHardwareItemDialog::on_NamelineEdit_textChanged(const QString &arg1)
{
    searchLibrary(arg1);
}

void NewHardwareItemDialog::searchLibrary(QString searchString)
{
    ui->BoardNameslistWidget->clear();

    foreach (QString filename, componentfiles)
    {
        QSettings boardfile(filename, QSettings::IniFormat);

        QString name = boardfile.value("overview/name","").toString();

        if (name.toLower().contains(searchString.toLower()))
        {
            QListWidgetItem *newItem = new QListWidgetItem;
            QString fullFilePath(filename);
            QVariant fullFilePathData(fullFilePath);
            newItem->setData(Qt::UserRole, fullFilePathData);
            newItem->setText(name);

            ui->BoardNameslistWidget->addItem(newItem);

        }

    }

}

void NewHardwareItemDialog::on_BoardNameslistWidget_itemPressed(QListWidgetItem *item)
{

    QSettings settings;
    QString dirname = settings.value("directories/board_library",QStandardPaths::standardLocations(QStandardPaths::AppDataLocation)[0] + "/boardlibrary").toString();
    QDir imagesDir(dirname);

    QVariant data = item->data(Qt::UserRole);
    QString fullFilePath = data.toString();

    qDebug() << "Activated:" << item->text() << ", " << fullFilePath;

    m_name = item->text();
    m_boardfile = fullFilePath;

    QSettings boardfile(fullFilePath, QSettings::IniFormat);

    QString imageFileName = imagesDir.absolutePath() + "/" + boardfile.value("bitmap/file","").toString();
    QPixmap pixmap(imageFileName);
    ui->ComponentPicturelabel->setPixmap(pixmap);
    ui->ComponentPicturelabel->setScaledContents(true);
    ui->ComponentPicturelabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored );
    ui->ComponentPicturelabel->update();
    m_imagefilename = imageFileName;

    ui->WidthSpinBox->setValue(boardfile.value("overview/width",ui->WidthSpinBox->value()).toDouble());
    ui->HeightSpinBox->setValue(boardfile.value("overview/height",ui->HeightSpinBox->value()).toDouble());

    qDebug() << "Activated:" << item->text() << ", " << fullFilePath;

}
