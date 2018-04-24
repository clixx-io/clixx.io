#include <QJsonObject>
#include <QFileDialog>
#include <QClipboard>
#include <QMimeData>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>

#include "newgraphicitemdialog.h"
#include "ui_newgraphicitemdialog.h"

NewGraphicItemDialog::NewGraphicItemDialog(QWidget *parent, QJsonObject *results) :
    QDialog(parent),
    ui(new Ui::NewGraphicItemDialog),
    completed(results)
{
    ui->setupUi(this);
}

NewGraphicItemDialog::~NewGraphicItemDialog()
{
    delete ui;
}

void NewGraphicItemDialog::on_PastetoolButton_clicked()
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

PreviewFileDialog::PreviewFileDialog(
    QWidget* parent,
    const QString & caption,
    const QString & directory,
    const QString & filter
) :
    QFileDialog(parent, caption, directory, filter)
{
    this->setOption(QFileDialog::DontUseNativeDialog,true);
    setObjectName("PreviewFileDialog");
    QVBoxLayout* box = new QVBoxLayout();

    mpPreview = new QLabel(tr("Preview"), this);
    mpPreview->setAlignment(Qt::AlignCenter);
    mpPreview->setObjectName("labelPreview");
    mpPreview->setMinimumSize(312,412);
    box->addWidget(mpPreview);
    box->addStretch();

    // add to QFileDialog layout
    QGridLayout *layout = static_cast<QGridLayout*>(this->layout());

    QList< QPair<QLayoutItem*, QList<int> > > movedItems;
    for(int i = 0; i < layout->count(); i++)
    {
      int row, column, rowSpan, columnSpan;
      layout->getItemPosition(i,&row,&column,&rowSpan,&columnSpan);
      if (row > 2)
      {
        QList<int> list;
        list << row << column << rowSpan << columnSpan;
        movedItems << qMakePair(layout->takeAt(i),list);
        i--;
      }
    }
    for(int i = 0; i < movedItems.count(); i++)
    {
      layout->addItem(movedItems[i].first,
                      movedItems[i].second[0],
                      movedItems[i].second[1],
                      movedItems[i].second[2],
                      movedItems[i].second[3]
              );
    }

    layout->addItem(box,1,3,1,1);
    connect(this, SIGNAL(currentChanged(const QString&)), this, SLOT(OnCurrentChanged(const QString&)));
}

void PreviewFileDialog::OnCurrentChanged(const QString & path)
{
    QPixmap pixmap = QPixmap(path);
    if (pixmap.isNull()) {
        mpPreview->setText("not an image");
    } else {
        mpPreview->setPixmap(pixmap.scaled(mpPreview->width(), mpPreview->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void NewGraphicItemDialog::on_toolButton_clicked()
{
    QFileDialog* mpOpenDialog = new PreviewFileDialog(this, tr("Open Graphic"), "", tr("Image Files (*.png *.jpg *.bmp *.tif);;"));
    mpOpenDialog->setAcceptMode(QFileDialog::AcceptOpen);
    if (mpOpenDialog->exec())
    {
        QStringList files = mpOpenDialog->selectedFiles();

        QPixmap pixmap(files[0]);

        qreal ar = (100 * pixmap.width()) / pixmap.height();

        ui->ComponentPicturelabel->setPixmap(pixmap);
        ui->ComponentPicturelabel->setScaledContents(true);
        ui->ComponentPicturelabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored );
        ui->ComponentPicturelabel->setFixedWidth(ui->ComponentPicturelabel->height() * (ar / 100));
        ui->ComponentPicturelabel->update();

        m_imagefilename = files[0];
    }

}

void NewGraphicItemDialog::on_buttonBox_accepted()
{
    QJsonObject object
    {
        {"width", ui->WidthSpinBox->value()},
        {"height", ui->HeightSpinBox->value()},
        {"picturefilename", m_imagefilename}

    };

    *completed = object;

}
