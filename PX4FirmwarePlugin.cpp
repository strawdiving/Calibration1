#include "PX4FirmwarePlugin.h"
#include <QFile>
#include <QXmlStreamReader>
#include <QDebug>
#include "PX4ParameterMetaData.h"
#include "Toolbox.h"


PX4FirmwarePlugin::PX4FirmwarePlugin(QuadApplication *app)
     :ToolBase(app)
{

}

void PX4FirmwarePlugin::setToolbox(Toolbox* toolbox)
{
    ToolBase::setToolbox(toolbox);
}

PX4FirmwarePlugin::~PX4FirmwarePlugin()
{

}

/// create PX4ParameterMetaData object to load and parse parameter metadata file
PX4ParameterMetaData* PX4FirmwarePlugin::loadParamFactMetaDataFile(QString &metaDataFile)
{
    PX4ParameterMetaData*px4MetaData = new PX4ParameterMetaData;

    qDebug()<<"PX4FirmwarePlugin::loadParamFactMetaDataFile";
    px4MetaData->loadParamFactMetaDataFile(metaDataFile);
    return px4MetaData;
}

/// call PX4ParameterMetaData object to add parameter metadata to related Fact object
void PX4FirmwarePlugin::addMetaDataToFact(PX4ParameterMetaData* px4MetaData,Fact* fact)
{
    if(px4MetaData)
    {
       // qDebug()<<"PX4FirmwarePlugin::addMetaDataToFact,px4metaData exists";
        px4MetaData->addMetaDataToFact(fact);
    }
}
