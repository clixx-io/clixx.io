#ifndef NEWHARDWAREITEMDIALOG_H
#define NEWHARDWAREITEMDIALOG_H

#include <QDialog>
#include "ui_newhardwareitemdialog.h"

namespace Ui {
class NewHardwareItemDialog;
}

class NewHardwareItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewHardwareItemDialog(QWidget *parent = 0);
    NewHardwareItemDialog(QWidget *parent, QJsonObject *results);
    ~NewHardwareItemDialog();

    QStringList loadBoardFiles();
    bool loadBoardList();

    void searchLibrary(QString searchString);

private slots:
    void on_buttonBox_accepted();

    void on_toolButton_clicked();

    void on_PastetoolButton_pressed();

    void on_BoardNameslistWidget_itemSelectionChanged();

    void on_searchlineEdit_textChanged(const QString &arg1);

private:
    Ui::NewHardwareItemDialog *ui;

    QString m_name;
    QString m_boardfile;
    QString m_imagefilename;

    double width;
    double height;
    int pinCount;
    int rowCount;

    QJsonObject *completed;

    bool havepastedimage,
         havesavedimage;

    QStringList componentfiles;

};

#endif // NEWHARDWAREITEMDIALOG_H
