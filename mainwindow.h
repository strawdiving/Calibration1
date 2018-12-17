#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#define M_PI (3.14159265358979323846)

#include <QMainWindow>
#include <QSerialPort>
#include<QTimer>
#include <QElapsedTimer>
#include "SerialCommunication.h"
#include "common/mavlink.h"
#include "ParameterLoader.h"
#include "SetupWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow*  _create();
    ~MainWindow();

    MainWindow* instance(void);
    //MessagePanel* messagePanel(void) {return _messagePanel;}

    void paintEvent(QPaintEvent* event);
    void closeEvent(QCloseEvent* event);
    void init();
    //void enableAllDataTransmission(SerialLink *link);//pack message, and send message to FC to request param list

    int    getSystemId() {return systemId;}

private slots:
    void _updateState();

public slots:
    /// Signals from LinkManager
    void _modeChange(QString shortModeText);
    void on_pushButton_Connect_clicked();

    void _activeVehicleChanged(Vehicle* vehicle);

protected:

private:
    Ui::MainWindow *ui;
    QPalette w_palette; //warning
    QPalette c_palette; //common
    bool                   port_open;
    bool                   link_connected;
    bool                   connectionLost;

    // TIMEOUT CONSTANTS
    static const unsigned int timeoutIntervalHeartbeat = 6; ///< Heartbeat timeout is 3.5 seconds
    SerialConfiguration* _config;
    LinkManager* _linkMgr;//manage serial links
    SerialLink* _currentLink;//currently connected link
    Vehicle* _vehicle;

    SetupWidget* _calWidget;

    QTimer             _portListTimer;//time to search for valid port
    QTimer             _statusTimer;
    int systemId;
};

#endif // MAINWINDOW_H
