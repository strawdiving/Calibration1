#ifndef SENSORSCOMPONENTCONTROLLER_H
#define SENSORSCOMPONENTCONTROLLER_H

/** Interface to start and stop calibration
  **/

#include <QObject>
#include "ComponentController.h"

class SensorsComponentController  : public ComponentController
{
    Q_OBJECT
public:
    SensorsComponentController();
    ~SensorsComponentController();

    void calibrateGyro(void);
    void calibrateMag(void);
    void calibrateAccel(void);

signals:  
    void handleUASTextMessage(QString textMessage);

public slots:
    void _stopCalibration(void);

private slots:
    void _handleUASTextMessage(int compId, QString text);

private:   
};

#endif // SENSORSCOMPONENTCONTROLLER_H
