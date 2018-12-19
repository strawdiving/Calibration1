#ifndef PIDPARAMSET_H
#define PIDPARAMSET_H

/**
 * Widget for PID setting of pitch/yaw/roll/x/y/z
  **/

#include <QWidget>
#include <QMap>

namespace Ui {
class PIDParamSet;
}

class PIDParamSet : public QWidget
{
    Q_OBJECT

public:
    PIDParamSet(QString name);
    ~PIDParamSet();
    QString name() {return _name;}
    /// initialize the initial values , or confirm the setting values
    void paramConfirmed(float P,float rate_P,float rate_I,float rate_D);

signals:
    /// signal to ParamPage to inform the change of PID params
    void paramChanged(QString moduleName,QMap<QString,float> paramValue);

private slots:
     /// called when settings applied, store settings in PID.ini (_storeSettings()) and send paramChanged signal
    void on_pushButton_Apply_clicked();

    /// called when settings reset, load settings stored in PID.ini (_loadSettings())
    void on_pushButton_Reset_clicked();

private:
    void _storeSettings();
    void _loadSettings();

    /// store the initial value sent back by the flight controller in initPID.ini
    void _storeInitSettings();

    Ui::PIDParamSet *ui;
    QString _name;

    /// key: param name,PITCH/ROLL/RAW/X/Y/Z
    /// value: value
    QMap<QString,float> _paramName2Value;
};

#endif // PIDPARAMSET_H
