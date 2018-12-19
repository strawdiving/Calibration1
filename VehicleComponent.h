
#ifndef VEHICLECOMPONENT_H
#define VEHICLECOMPONENT_H

/**
 * Abstract class  representing  vehicle components
  **/

#include <QObject>
#include <QStringList>
#include <QVariant>

class PX4AutopilotPlugin;

class VehicleComponent : public QObject
{
    Q_OBJECT
public:
    VehicleComponent(PX4AutopilotPlugin* autopilot, QObject *parent = 0);
    ~VehicleComponent();

    /// name of component
    virtual QString name(void) const = 0;
    /// icon resource of component
    virtual QString iconResource(void) const = 0;
    /// @return true: the component needs to be set
    virtual bool requiresSetup(void) const = 0;

    /// @return list of component names that have not been calibrated
    virtual QStringList setupComplete(void) const = 0;

     /// @return true: Setup panel can be shown while vehicle is armed
     //virtual bool allowSetupWhileArmed(void) const;

    /// @brief Returns an list of parameter names for which a change should cause the setupCompleteChanged
    ///         signal to be emitted. Last element is signalled by NULL.
    virtual QStringList setupCompleteChangedTriggerList(void) const = 0;

    /// Should be called after the component is created (but not in constructor) to setup the
    /// signals which are used to track parameter changes which affect setupComplete state.
//    void setupTriggerSignals(void);

signals:
//    void setupCompleteChanged(bool setupComplete);

public slots:
    /// reload calibration state
//    void _triggerUpdated(QVariant value);

protected:
    PX4AutopilotPlugin* _autopilot;
};

#endif // VEHICLECOMPONENT_H
