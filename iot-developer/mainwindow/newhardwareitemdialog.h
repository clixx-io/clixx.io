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
    ~NewHardwareItemDialog();

    QString getName() { return(name); };
    QString getFile() { return(boardfile); };
    QString getImage() { return(imagefilename); };
    double getWidth() { return(width); };
    double getHeight() { return(height); };
    int getPinCount() { return(pinCount); };
    int getRowCount() { return(rowCount); };

    QStringList loadBoardFiles();
    bool loadBoardList();

private slots:
    void on_toolButton_triggered(QAction *arg1);

private:
    Ui::NewHardwareItemDialog *ui;

    QString name;
    QString boardfile;
    QString imagefilename;
    double width;
    double height;
    int pinCount;
    int rowCount;

};

#endif // NEWHARDWAREITEMDIALOG_H
