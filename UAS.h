/**
 * @file
 *   @brief Class UAS. This class handls calibration related messages.
 *
 */
#ifndef UAS_H
#define UAS_H

#include <QObject>
#include <common/mavlink.h>
#include <mavlink_types.h>
#include "ParameterLoader.h"
#include "Vehicle.h"

//handle calibration related messages
class UAS : public QObject
{
    Q_OBJECT

public:
    UAS(Vehicle* vehicle);
    ~UAS();
    /// Handling  messages of type PARAM_VALUE or STATUSTEXT
   void receiveMessage(SerialLink *link, mavlink_message_t &message);

   /// Request writing parameters to the flight controller (PARAM_SET)
   void writeParameterRaw(int componentId, const QString& paramName, const QVariant& value);
  ///  Request reading parameters from the flight controller (PARAM_REQUEST_READ)
   void readParameterRaw(int componentId, const QString& paramName, int paramIndex);
   void writePIDParams(QString moduleName,QMap<QString,float>paramValue);

   /// type of calibration
   enum CalibrationType {
       CalibrationGyro,
       CalibrationMag,
       CalibrationAccel,
   };

   /// start calibration of type of calType  (MAV_CMD_PREFLIGHT_CALIBRATION)
   void startCalibration(CalibrationType calType);
   /// stop calibration (MAV_CMD_PREFLIGHT_CALIBRATION)
   void stopCalibration(void); 

   /// Vehicle is about to go away
   void shutdownVehicle(void);

protected:

signals:
    /// signal to ParameterLoader to handle
    ///component: componentId
    /// paramName-- param_value.param_id[16]
    /// paramCount--param_value.param_count
    /// paramId--param_value.param_index
    /// type--param_value.param_type
    /// value-- according to the type(MAV_PARAM_TYPE),adjust the param_union value
    void parameterUpdate(int componentId, QString paramName, int paramCount, int paramId, int type, QVariant value);

    ///@brief  A text message from the system has been received
    /// signal to SensorsComponentController to handle calibration text message
    void textMessageReceived(int componentid, QString text);

public slots:

private:
    /// get and send proper parameter info to ParameterLoader to handle
    void _processParamValueMsg(mavlink_message_t &message,const QString &paramName,const mavlink_param_value_t &rawValue,mavlink_param_union_t &paramValue);
    /// change ValueType_t in FactMetaData to MAV_PARAM_TYPE
    MAV_PARAM_TYPE _factTypeToMavType(FactMetaData::ValueType_t valueType);

    ParameterLoader * _parameterLoader;
    Vehicle *_vehicle;

    int _uasId;                    ///< Unique system ID
    const static int defaultComponentId = 0;
};

#endif // UAS_H
