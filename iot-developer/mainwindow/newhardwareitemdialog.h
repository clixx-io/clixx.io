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

    QString getName() { return(ui->NamelineEdit->text()); };
    QString getFile() { return(QString("")); };
    QString getImage() { return(QString("")); };
    qreal getWidth() { return(ui->WidthSpinBox->value()); };
    qreal getHeight() { return(ui->HeightSpinBox->value()); };

private slots:
    void on_toolButton_triggered(QAction *arg1);

private:
    Ui::NewHardwareItemDialog *ui;
};

#endif // NEWHARDWAREITEMDIALOG_H
