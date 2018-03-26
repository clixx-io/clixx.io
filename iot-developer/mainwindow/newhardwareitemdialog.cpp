#include "newhardwareitemdialog.h"
#include "ui_newhardwareitemdialog.h"

NewHardwareItemDialog::NewHardwareItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewHardwareItemDialog)
{
    ui->setupUi(this);
}

NewHardwareItemDialog::~NewHardwareItemDialog()
{
    delete ui;
}

void NewHardwareItemDialog::on_toolButton_triggered(QAction *arg1)
{

}
