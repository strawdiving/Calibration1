#ifndef MESSAGEPANEL_H
#define MESSAGEPANEL_H

/** show important messages for calibration, with ui
  **/
#include <QWidget>
#include <QLabel>

class MessagePanel : public QWidget
{
    Q_OBJECT
public:
    explicit MessagePanel(QWidget *parent = 0);
    ~MessagePanel();

signals:

public slots:
    /// show messages from SetupWidget and PX4AutopilotPlugin
    void _showMessage(const QString text);
    /// show loading progress of initial params
    void _paramLoadProgress(int currentCount,int totalCount);

private:
    /// label to show messages
    QLabel* _messageText;
    const QString _armedVehicleText = "This operation cannot be performed while vehicle is armed";
    const QString _disconnectedVehicleText = QString(tr("连接飞机，地面站将会自动检测到飞机 "));

};

#endif // MESSAGEPANEL_H
