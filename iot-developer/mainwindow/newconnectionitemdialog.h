#ifndef NEWCONNECTIONITEMDIALOG_H
#define NEWCONNECTIONITEMDIALOG_H

#include <QDialog>

namespace Ui {
class NewConnectionItemDialog;
}

class NewConnectionItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewConnectionItemDialog(QWidget *parent = 0);
    NewConnectionItemDialog(QWidget *parent, QJsonObject *results);
    ~NewConnectionItemDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::NewConnectionItemDialog *ui;

    void loadParameters(QJsonObject &parameters);

    QJsonObject *completed;

};

#endif // NEWCONNECTIONITEMDIALOG_H
