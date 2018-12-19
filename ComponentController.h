#ifndef COMPONENTCONTROLLER_H
#define COMPONENTCONTROLLER_H

/** Base controller class for component
  **/

#include <QObject>

#include "Fact.h"
#include "PX4AutopioltPlugins/PX4AutopilotPlugin.h"
#include "UAS.h"

class ComponentController : public QObject
{
    Q_OBJECT
public:
    explicit ComponentController(QObject *parent = 0);
    ~ComponentController();

signals:

public slots:

protected:
    /// @return Fact*:  Fact with name paramName
    Fact* getParamFact(int compId,const QString paramName);
    /// @return true: all params listed exist
    bool _allParamsExist(int compId,QStringList params);
    UAS* _uas;
    PX4AutopilotPlugin *_autopilot;
    Vehicle* _vehicle;
};

#endif // COMPONENTCONTROLLER_H
