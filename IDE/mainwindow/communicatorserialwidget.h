#ifndef COMMUNICATORSERIALWIDGET_H
#define COMMUNICATORSERIALWIDGET_H

#include <QWidget>

namespace Ui {
class CommunicatorSerialWidget;
}

class CommunicatorSerialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicatorSerialWidget(QWidget *parent = 0);
    ~CommunicatorSerialWidget();

private slots:
    void on_commandLinkButton_pressed();

    void on_sendButton_pressed();

    void on_CloseCommandLinkButton_pressed();

private:
    Ui::CommunicatorSerialWidget *ui;
};

#endif // COMMUNICATORSERIALWIDGET_H
