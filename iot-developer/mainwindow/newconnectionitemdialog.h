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

private:
    Ui::NewConnectionItemDialog *ui;
};

#endif // NEWCONNECTIONITEMDIALOG_H
