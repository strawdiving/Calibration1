
/**
 * @file
 *   @brief Class VehicleManager. This class manages creation and destroy of vehicle
 *
 */
#ifndef VEHICLEMANAGER_H
#define VEHICLEMANAGER_H

#include "Toolbox.h"
#include "Vehicle.h"

class QuadApplication;
class LinkManager;
class SerialLink;
class PX4FirmwarePlugin;

class VehicleManager : public ToolBase
{
    Q_OBJECT
public:
    VehicleManager(QuadApplication* app);
    ~VehicleManager();

    virtual void setToolbox(Toolbox* toolbox);
    Vehicle* activeVehicle(void)     {
        if(_activeVehicle) {
            return _activeVehicle;
        }
        return NULL;
    }

signals:
    /// signal to MainWindow and SetupWidget to enable/disable monitor and calibration
    void activeVehicleChanged(Vehicle* vehicle);

public slots:
    /// create active vehicle
    void _vehicleHeartbeatInfo(SerialLink* link, int vehicleId);

private slots:
    /// deactivate vehicle
    void _deleteVehicle(Vehicle* vehicle);

private:
       LinkManager* _linkMgr;
       Vehicle* _activeVehicle; /// active vehicle
       PX4FirmwarePlugin* _firmwarePlugin;
};

#endif // VEHICLEMANAGER_H
