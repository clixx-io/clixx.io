#ifndef COMMUNICATORSERIALWIDGET_H
#define COMMUNICATORSERIALWIDGET_H

#include <QWidget>
#include <QStringList>

QT_FORWARD_DECLARE_CLASS(QSerialPort)
QT_FORWARD_DECLARE_CLASS(QTreeWidgetItem)
QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QByteArray)
QT_FORWARD_DECLARE_CLASS(MainWindow)

namespace Ui {
class CommunicatorSerialWidget;
}

class CommunicatorSerialWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommunicatorSerialWidget(QWidget *parent = 0);
    ~CommunicatorSerialWidget();

    QStringList ListSerialPorts();
    void refreshSerialPorts();
    bool openSerialPort(const QString serialportname);
    void closeSerialPort();

    void showStatusMessage(const QString &message);
    inline void setMainWindow(MainWindow *main) { mainwindow = main; }

    bool writewithEcho(const QString linetosend);
    void displayRxChar(char c);

private slots:
    void on_commandLinkButton_pressed();
    void on_sendButton_pressed();
    void on_CloseCommandLinkButton_pressed();
    void on_TimerEvent();

    void on_BaudrateSpeedDial_sliderMoved(int position);

    void on_portSelectiontreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_portSelectiontreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_portSelectiontreeWidget_itemPressed(QTreeWidgetItem *item, int column);

private:
    Ui::CommunicatorSerialWidget *ui;

    QSerialPort *serialPort = nullptr;
    QList <int> availableBaudRates;
    QString portName, portType;
    QTimer *serialTimer;
    QByteArray serialBuffer;

    MainWindow *mainwindow = nullptr;

};

#endif // COMMUNICATORSERIALWIDGET_H
