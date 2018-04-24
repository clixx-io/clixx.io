#ifndef NEWGRAPHICITEMDIALOG_H
#define NEWGRAPHICITEMDIALOG_H

#include <QDialog>
#include <QFileDialog>
#include <QLabel>
#include <QJsonObject>

namespace Ui {
class NewGraphicItemDialog;
}

class NewGraphicItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewGraphicItemDialog(QWidget *parent = 0, QJsonObject *results = 0);
    ~NewGraphicItemDialog();

private slots:
    void on_PastetoolButton_clicked();

    void on_toolButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::NewGraphicItemDialog *ui;

    QJsonObject *completed;

    QString m_imagefilename;
};

class PreviewFileDialog : public QFileDialog
{
        Q_OBJECT
    public:
        explicit PreviewFileDialog(
            QWidget* parent = 0,
            const QString & caption = QString(),
            const QString & directory = QString(),
            const QString & filter = QString()
        );

    protected slots:
        void OnCurrentChanged(const QString & path);

    protected:
        QLabel* mpPreview;

};

#endif // NEWGRAPHICITEMDIALOG_H
