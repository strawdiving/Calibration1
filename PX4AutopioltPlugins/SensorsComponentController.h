#ifndef SENSORSCOMPONENTCONTROLLER_H
#define SENSORSCOMPONENTCONTROLLER_H

/**
 * Controller class. Interface to start and stop calibration
  **/

#include <QObject>
#include "ComponentController.h"

class SensorsComponentController  : public ComponentController
{
    Q_OBJECT
public:
    SensorsComponentController();
    ~SensorsComponentController();

    /// call UAS to start calibration of specific calibration type
    void calibrateGyro(void);
    void calibrateMag(void);
    void calibrateAccel(void);

signals:  
    /// signal to SensorsPage to handle calibration text messages
    void handleUASTextMessage(QString textMessage);

public slots:
    /// stop calibration and reload initial parameters from flight controller
    void _stopCalibration(void);

private slots:
    /// forward handleUASTextMessage signal to SensorsPage
    void _handleUASTextMessage(int compId, QString text);

private:   
};

#endif // SENSORSCOMPONENTCONTROLLER_H
