/**
 * @file
 *   @brief Class Vehicle. This class represents a vehicle with specific id, it handles received messages
 *  sent from LinkManager instance, decodes and parses messages and then dispatchs parsed valid information.
 *
 */
#ifndef VEHICLE_H
#define VEHICLE_H

#include <QObject>
#include <common/mavlink.h>
#include "SerialCommunication.h"
#include <QVariant>
#include <mavlink_types.h>
#include "FactMetaData.h"

class PX4FirmwarePlugin;
class PX4AutopilotPlugin;
class Vehicle;
class LinkManager;
class UAS;
class ParameterLoader;

class Vehicle : public QObject
{
    Q_OBJECT
public:
    explicit Vehicle(SerialLink* link, int vehicleId, PX4FirmwarePlugin* firmwarePlugin);
    ~Vehicle();

    /// all called by vihicle when used afterwards
    PX4AutopilotPlugin* autopilotPlugin(void) {return _autopilotPlugin;}
    UAS* uas(void) {return _uas;}
    ParameterLoader* parameterLoader(void) {return _parameterLoader;}
    PX4FirmwarePlugin* firmwarePlugin(void) {return _firmwarePlugin;}
    LinkManager* linkManager(void) { return _linkMgr;}

    int id(void) { return _vehicleId;}

private:
    QString _flightMode(uint8_t baseMode);  /// string of current flight mode
    void _addLink(SerialLink* link);
    void _handleHeartbeat(mavlink_message_t& message);  /// handle heartbeat message

    PX4FirmwarePlugin* _firmwarePlugin;
    LinkManager * _linkMgr;
    SerialLink* _link;

    /// all created by vehicle
    PX4AutopilotPlugin* _autopilotPlugin;
    UAS* _uas;
    ParameterLoader* _parameterLoader;

    int                     _vehicleId;  /// system id of current vehicle
    uint8_t             _base_mode;   /// custom flight modes, 2-manual, 3- altitude, 3-position

signals:
    /// signal to VehicleManager to handle link deletion
    void linkDeleted(Vehicle* vehicle);

    /// signal to mainWindow to change flight mode text
    void modeChanged(QString shortModeText);

    void PIDSetConfirm(mavlink_pid_set_confirm_t pid_set_confirm);

public slots:
    void _linkDeleted(SerialLink* link);
    /// Handle messages from LinkManager, parse status of vehicle and flight, call UAS to handle
    /// PARAM_VALUE and STATUS_TEXT messages
    void _mavlinkMessageReceived(SerialLink*link, mavlink_message_t &message);
};

#endif // VEHICLE_H
