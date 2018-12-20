#ifndef PX4AUTOPILOTPLUGIN_H
#define PX4AUTOPILOTPLUGIN_H

/**
 * Create vehicle components and check if they need calibration
 * The only interface to call ParameterLoader to get parameter Fact for calibration
  **/
#include "SensorsComponent.h"
#include "Fact.h"
#include "PX4ParameterMetaData.h"
//#include "Vehicle.h"

class Vehicle;

class PX4AutopilotPlugin : public QObject
{
    Q_OBJECT

public:
    PX4AutopilotPlugin(Vehicle* vehicle, QObject* parent);
    ~PX4AutopilotPlugin();

    /// get Fact of parameter
    Fact* getParamFact(int componentId, const QString& name);
    /// @return true: parameter of the specified name exists
    bool paramExists(int componentId,const QString& name);
    /// reload parameters from flight controller
    void refreshParameters(void);

    /// create vehicle components
    const QVariantList& vehicleComponents(void);

    static const int defaultCompId = -1;

signals:
    void setupCompleteChanged(bool setupComplete);  /// signal sent to SummaryPage when calibration status changes
    void showMessage(const QString text);    /// signal sent to MessagePanel to show calibration messages
    void parametersReadyChanged(bool parametersReady);    /// signal to SetupWidget when initial parameters loaded succesfully
    void setupCompleteStatus(QStringList incomplete);    /// signal to SetupWidget to show which component needs calibration

public slots:
    /// check if calibration is needed when initial parameters loaded completely
    void _parametersReadyChanged(bool parametersReady);

private:
    void _recalcSetupComplete(void);   /// check if components need calibration

    QVariantList _vehicleComponents; /// list of components
    SensorsComponent* _sensorsComponent;   /// sensors component

    Vehicle* _vehicle;
    bool _setupComplete; /// true if all calibration is completed
};

#endif // PX4AUTOPILOTPLUGIN_H
