#ifndef PX4PARAMETERMETADATA_H
#define PX4PARAMETERMETADATA_H

/**
 * Load and parse parameter meta file, add metadata of parameter to related Fact object
  **/
#include <QObject>
#include "FactMetaData.h"
#include "Fact.h"

class PX4ParameterMetaData : public QObject
{
    Q_OBJECT
public:
    explicit PX4ParameterMetaData(QObject *parent = 0);
    ~PX4ParameterMetaData();

    void loadParamFactMetaDataFile(QString &metaDataFile);

    /// add metadata of parameter to related Fact object
    void addMetaDataToFact(Fact* fact);

signals:

public slots:

private:
    /// state machine for parsing parameter metadata xml file
    enum {
        XMLStateNone,
        XMLStateFoundParameters,
        XMLStateFoundParameter,
        XMLStateDone
    };

    /// read and parse parameter metadata xml file
    void _loadParameterFactMetaDataFile(const QString& metaDataFile);

    /// true if parameter metadata file already loaded
    bool _parameterMetaDataLoaded;

    /// key: parameter name
    /// value: parameter  metadata of specified name
    QMap<QString, FactMetaData*> _mapParameterName2FactMetaData;
};

#endif // PX4PARAMETERMETADATA_H
