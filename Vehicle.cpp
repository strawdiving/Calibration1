#include "Vehicle.h"
#include "PX4FirmwarePlugin.h"
#include "PX4AutopioltPlugins/PX4AutopilotPlugin.h"

#include "ParameterLoader.h"
#include "QuadApplication.h"
#include "linkmanager.h"
#include "UAS.h"
#include <QDebug>

Vehicle::Vehicle(SerialLink* link, int vehicleId,PX4FirmwarePlugin *firmwarePlugin)
    : _autopilotPlugin(NULL)
    , _vehicleId(vehicleId)
    , _link(NULL)
    , _uas(NULL)
    , _parameterLoader(NULL)
    , _base_mode(0)
    , _system_status(-1)
    , _firmwarePlugin(firmwarePlugin)
{
    //qDebug()<<"vehicle";
    _addLink(link);

    _autopilotPlugin = new PX4AutopilotPlugin(this,this);

    _linkMgr = qgcApp()->toolbox()->linkManager();
    connect(_linkMgr,&LinkManager::messageReceived,this,&Vehicle::_mavlinkMessageReceived);

    _uas = new UAS(this);

    _parameterLoader = new ParameterLoader(this,this);
    connect(_parameterLoader,&ParameterLoader::paramsReady,_autopilotPlugin,&PX4AutopilotPlugin:: _parametersReadyChanged);
}

Vehicle::~Vehicle()
{
delete _parameterLoader;
}

 void Vehicle::_addLink(SerialLink* link)
 {
     if(!_link) {
         _link = link;
         connect(qgcApp()->toolbox()->linkManager(),&LinkManager::linkDisconnected,this,&Vehicle::_linkDeleted);
     }
 }

void Vehicle::_linkDeleted(SerialLink* link)
{
    Q_UNUSED(link)
    _link = NULL;
    qDebug()<<"Vehicle : link deleted";
    emit linkDeleted(this);
}

void Vehicle::_mavlinkMessageReceived(SerialLink*link, mavlink_message_t &message)
{
    //qDebug()<<"vehicle: mavlinkMessageReceived";
    if(_link != link){
        _addLink(link);
    }

    switch(message.msgid) {
    case MAVLINK_MSG_ID_HEARTBEAT:
        _handleHeartbeat(message);
        break;

    case MAVLINK_MSG_ID_PID_SET_Confirm:
    {
        //qDebug()<<"Vehicle: PID_SET_Confirm message received";
        mavlink_pid_set_confirm_t pid_set_confirm_t;
        mavlink_msg_pid_set_confirm_decode(&message,&pid_set_confirm_t);
        emit PIDSetConfirm(pid_set_confirm_t);
    }

    default:
        break;
    }
    if((message.msgid == MAVLINK_MSG_ID_PARAM_VALUE) || (message.msgid == MAVLINK_MSG_ID_STATUSTEXT))
    {
        _uas->receiveMessage(link,message);
    }
}

void Vehicle::_handleHeartbeat(mavlink_message_t& message)
{
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(&message, &heartbeat);

    if (_base_mode != heartbeat.base_mode)
    {
        _base_mode = heartbeat.base_mode;
        QString shortModeText = _flightMode(_base_mode);

        emit modeChanged(shortModeText);
    }
}

QString Vehicle::_flightMode(uint8_t baseMode)
{
    QString flightMode = "Unknown";
    //qDebug()<<"baseMode"<<baseMode;
    switch(baseMode){
    case 2:
        flightMode = QString(tr("手动"));
        break;
    case 3:
        flightMode = QString(tr("定高"));
        break;
    case 4:
        flightMode = QString(tr("定点"));
        break;
    default:
        qDebug()<<"Unknown flight mode:";
        break;
        }
    return flightMode;
}
