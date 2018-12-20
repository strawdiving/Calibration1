#ifndef PARAMETERLOADER_H
#define PARAMETERLOADER_H

/**
 * Load parameters from flight controller and store parameters in Fact
  **/

#include <QObject>
#include <QVariant>
#include <common/mavlink.h>
#include <QMutex>
#include <QMap>
#include <QTimer>

#include "PX4FirmwarePlugin.h"
#include "Fact.h"
#include "PX4ParameterMetaData.h"

class Vehicle;

class ParameterLoader : public QObject
{
    Q_OBJECT
public:
    explicit ParameterLoader(Vehicle *vehicle, QObject *parent = 0);
    ~ParameterLoader();

    /// @return true: param of specified name exists
    bool paramExists(int componentId,const QString& name);
    /// @return Fact* of specified name
    Fact* getFact(int componentId, const QString& name);
    /// reload all parameters from flight controller when calibration finished
    void refreshParameters(void);

signals:
    /// signal to PX4AutopilotPlugin to start calibration status check
    void paramsReady(bool noMissingParams);
    /// signal to MessagePanel to show parameters loading progress
    void paramLoadProgress(int currentCount,int totalCount);

private slots:
    void _valueUpdated(const QVariant& value);
    void _parameterUpdate(int componentId, QString paramName, int paramCount, int paramId, int type,QVariant value);
    void _initialParamRequestTimeout();

private:
    /// get actual component Id
    int _actualCompId(int componentId);

    void _refreshAllParameters();

    void _addMetaDataToDefaultComponent(void);

    /// check if initial parameters are loaded completely
    void _checkInitialLoadComplete(void);

    Vehicle* _vehicle;
    PX4FirmwarePlugin *_px4FirmwarePlugin;
    PX4ParameterMetaData* _parameterMetaData;
    static Fact _defaultFact;   ///< Used to return default fact, when parameter not found

    /// singleShot timer for requesting initial parameters from flight controller, stopped when parameters received
    QTimer _initialParamRequestTimer;
    QMutex _dataMutex;

    /// store parameters received from flight controller
    /// key: name of parameter, value: QVariant:containing a copy of Fact pointer
    QMap<QString,QVariant> _mapParamName2Variant;

    int _defaultCompId;  /// default component Id
    bool _compParamsComplete;  /// true if all parameters loaded
    int _currentParamCount;  /// number parameters received
    int _totalParamCount;  /// number of all parameters
};

#endif // PARAMETERLOADER_H
