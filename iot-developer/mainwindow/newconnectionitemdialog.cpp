#include <QJsonObject>

#include "newconnectionitemdialog.h"
#include "ui_newconnectionitemdialog.h"

NewConnectionItemDialog::NewConnectionItemDialog(QWidget *parent, QJsonObject *results) :
    QDialog(parent),
    ui(new Ui::NewConnectionItemDialog)
{
    ui->setupUi(this);
}

NewConnectionItemDialog::~NewConnectionItemDialog()
{
    delete ui;
}
