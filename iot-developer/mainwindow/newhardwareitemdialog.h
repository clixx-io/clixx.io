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

    /*
    explicit NewHardwareItemDialog(QWidget *parent = 0, QString *boardname = nullptr, QString *boardfilename, QString *boardimagefilename,
                                   double *boardwidth, double *boardheight, int *pincount, int *rowcount);
    */

    ~NewHardwareItemDialog();

    QStringList loadBoardFiles();
    bool loadBoardList();

private slots:
    void on_toolButton_triggered(QAction *arg1);

    void on_buttonBox_accepted();

    void on_toolButton_clicked();

private:
    Ui::NewHardwareItemDialog *ui;

    QString name;
    QString boardfile;
    QString imagefilename;
    double width;
    double height;
    int pinCount;
    int rowCount;

    QJsonObject *completed;

};

#endif // NEWHARDWAREITEMDIALOG_H
