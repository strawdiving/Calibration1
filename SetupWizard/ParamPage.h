#ifndef PARAMPAGE_H
#define PARAMPAGE_H

/**
 * Main panel to show and set PID parameters, mainly used to adjust parameters by engineer
  **/

#include <QWidget>
#include "PX4AutopioltPlugins/ParamEditController.h"
#include <QMap>
#include "PIDParamSet.h"

class ParamPage : public QWidget
{
    Q_OBJECT

public:
    explicit ParamPage(QWidget *parent = 0);
    ~ParamPage();

    /// create ParamEditController instance to handle setting PID
    void initParamController(void);

signals:
    /// signal to ParamEditController to update PID params in flight controller
    void paramChanged(QString moduleName,QMap<QString,float> paramValue);

private slots:
    /// forward paramChanged signal with PID params
    void _paramChanged(QString moduleName,QMap<QString,float> paramValue);
    /// call widget to confirm PID params when pid_set_confirm message received
    void _PIDSetConfirm(mavlink_pid_set_confirm_t pid_set_confirm);

private:    
    ParamEditController* _paramEditController;

    /// Corresponds to the PID_NAME parameter value (enumeration type) of the pid_set_confirm message
    enum PID_NAME
    {
        PITCH,
        ROLL,
        YAW,
        X,
        Y,
        Z
    };

    /// widget for PID setting of pitch/yaw/roll/x/y/z
    PIDParamSet* _pitchPIDSetWgt;
    PIDParamSet* _rollPIDSetWgt;
    PIDParamSet* _yawPIDSetWgt;
    PIDParamSet* _xPIDSetWgt;
    PIDParamSet* _yPIDSetWgt;
    PIDParamSet* _zPIDSetWgt;

    /// list of 6 PID setting widgets
    QList<PIDParamSet*> _widgets;
};

#endif // PARAMPAGE_H
