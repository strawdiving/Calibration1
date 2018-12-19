#ifndef PARAMEDITCONTROLLER_H
#define PARAMEDITCONTROLLER_H

/**
 *  Controller class. Interface to load and set PID params
  **/
#include "ComponentController.h"

class ParamEditController: public ComponentController
{
    Q_OBJECT
public:
    ParamEditController();
    ~ParamEditController();

signals:
    /// signal to ParamPage to initialize or confirm PID params
    void PIDSetConfirm(mavlink_pid_set_confirm_t pid_set_confirm);

public slots:
    /// call UAS to write PID params to flight controller
    void _paramChanged(QString moduleName,QMap<QString,float> paramValue);
    /// forward pid_set_confirm message to ParamPage
    void _PIDSetConfirm(mavlink_pid_set_confirm_t pid_set_confirm);
};

#endif // PARAMEDITCONTROLLER_H
