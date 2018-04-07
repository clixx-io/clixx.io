#include <QJsonObject>
#include <QDebug>

#include "newconnectionitemdialog.h"
#include "ui_newconnectionitemdialog.h"

NewConnectionItemDialog::NewConnectionItemDialog(QWidget *parent, QJsonObject *results) :
    QDialog(parent),
    ui(new Ui::NewConnectionItemDialog),
    completed(results)
{
    ui->setupUi(this);

    loadParameters(*results);

}

NewConnectionItemDialog::~NewConnectionItemDialog()
{
    delete ui;
}

void NewConnectionItemDialog::loadParameters(QJsonObject &parameters)
{
    if (parameters.contains("component_count") && parameters["component_count"].isDouble())
    {
        QString ckeyid, ckeyname;

        for (int i=0; i<parameters["component_count"].toInt(); i++)
        {
            ckeyid = QObject::tr("component_%1_id").arg(i+1);
            ckeyname = QObject::tr("component_%1_name").arg(i+1);

            qDebug() << parameters[ckeyname].toString();

            ui->fromcomboBox->addItem(parameters[ckeyname].toString(),parameters[ckeyid].toString());
            ui->tocomboBox->addItem(parameters[ckeyname].toString(),parameters[ckeyid].toString());

        }
    }
}

void NewConnectionItemDialog::on_buttonBox_accepted()
{
    completed->insert("startitem", ui->fromcomboBox->currentData().toString());
    completed->insert("enditem", ui->tocomboBox->currentData().toString());
}
