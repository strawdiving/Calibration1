#-------------------------------------------------
#
# Project created by QtCreator 2016-06-24T09:26:30
#
#-------------------------------------------------

QT       += core \
                  gui \
                  serialport \

!MobileBuild {4
QT += \
    printsupport \
    serialport \
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Drones
TEMPLATE = app

include(./MAVLinkLib.pri)
INCLUDEPATH +=./libs/mavlink/include/mavlink/v1.0

SOURCES += main.cpp\
        mainwindow.cpp \
    linkmanager.cpp \
    SerialCommunication.cpp \
    Fact.cpp \
    FactMetaData.cpp \
    ParameterLoader.cpp \
    PX4ParameterMetaData.cpp \
    VehicleComponent.cpp \
    PX4AutopioltPlugins/PX4AutopilotPlugin.cpp \
    PX4AutopioltPlugins/SensorsComponent.cpp \
    PX4FirmwarePlugin.cpp \
    Vehicle.cpp \
    PX4AutopioltPlugins/SensorsComponentController.cpp \
    QuadApplication.cpp \
    Toolbox.cpp \
    VehicleManager.cpp \
    UAS.cpp \
    ComponentController.cpp \
    SetupWizard/MessagePanel.cpp \
    SetupWidget.cpp \
    SetupWizard/SummaryViewPage.cpp \
    SetupWizard/SensorsGroupPage.cpp \
    SetupWizard/CalRotationWidget.cpp \
    SetupWizard/OrientationWidget.cpp \
    SetupWizard/ParamPage.cpp \
    PX4AutopioltPlugins/ParamEditController.cpp \
    SetupWizard/PIDParamSet.cpp \

HEADERS  += mainwindow.h \
    linkmanager.h \
    SerialCommunication.h \
    Fact.h \
    FactMetaData.h \
    ParameterLoader.h \
    PX4ParameterMetaData.h \
    VehicleComponent.h \
    PX4AutopioltPlugins/PX4AutopilotPlugin.h \
    PX4AutopioltPlugins/SensorsComponent.h \
    PX4FirmwarePlugin.h \
    Vehicle.h \
    PX4AutopioltPlugins/SensorsComponentController.h \
    QuadApplication.h \
    Toolbox.h \
    VehicleManager.h \
    UAS.h \
    ComponentController.h \
    SetupWizard/MessagePanel.h \
    SetupWidget.h \
    SetupWizard/SummaryViewPage.h \
    SetupWizard/SensorsGroupPage.h \
    SetupWizard/CalRotationWidget.h \
    SetupWizard/OrientationWidget.h \
    SetupWizard/ParamPage.h \
    PX4AutopioltPlugins/ParamEditController.h \
    SetupWizard/PIDParamSet.h \

FORMS    += mainwindow.ui \
    SetupWidget.ui \
    SetupWizard/SummaryViewPage.ui \
    SetupWizard/SensorsGroupPage.ui \
    SetupWizard/CalRotationWidget.ui \
    SetupWizard/PIDParamSet.ui \
    SetupWizard/ParamPage.ui

UI_DIR  += E:/baseModule-12.17
RESOURCES += \
    qml.qrc


