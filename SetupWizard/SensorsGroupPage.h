#ifndef SENSORSGROUPPAGE_H
#define SENSORSGROUPPAGE_H

/**
 * UI page for sensors calibration,
 * handle calibration text messages , and show calibration messages and visual status (with pictures)
  **/
#include <QWidget>
#include "PX4AutopioltPlugins/SensorsComponentController.h"
#include "CalRotationWidget.h"

class SetupWidget;

namespace Ui {
class SensorsGroupPage;
}

class SensorsGroupPage : public QWidget
{
    Q_OBJECT

public:
    explicit SensorsGroupPage(QWidget *parent = 0);
    ~SensorsGroupPage();
    /// create SensorsComponentController instance to handle calibration progress
    void initSensorsController();

signals:
    /// signals to SensorsComponentController to stop calibration
    void stopCalibration(void);

    /// signals to CalRotationWidget
    /// show calibration status
    void calStatusChanged(QString side,bool inProgress,bool rotate,bool done);
    /// change visibility of 6 sides
    void sidesVisibleChanged(int sidesVisible);
    /// show calibration messages
    void status(QString message);

private slots:
    /// call SensorsComponentController to start calibration of different calibration type
    void _calibrate(void);
    /// handle status text from UAS, and change calibration states according to text
    void _handleUASTextMessage(QString text);

private:
    Ui::SensorsGroupPage *ui;

    /// code of status when calibration stopped
    enum StopCalibrationCode {
        StopCalibrationSuccess,
        StopCalibrationFailed,
    };

    /// called when calibration for all sides is done to show complete status
    void _resetVisualState();
    /// change visibility of 6 sides
    void _setVisualState(int sidesVisible);

    /// reset calibration status, and disable all calibration buttons
    void _startVisualCalibration();

    void _stopCalibration(StopCalibrationCode code);

    /// mark of status of calibration in progress for Accel/Mag/Gyro
    bool _accelCalInProgress;
    bool _magCalInProgress;
    bool _gyroCalInProgress;

    /// mark of calibration status
    bool _calInProgress;
    bool _calRotate;
    bool _calDone;

    /// number of sides visible in CalRotationWidget
    int _sidesVisible;

    /// 6 sides, up/down/left/right/front/back
    QStringList _sidesList;

    const QString  statusTextDefault = QString(tr("点击上方按钮开始校准 "));

    const QString compassHelp = QString(tr("开始采集数据   \n 请按OK按钮确认,并将飞机绕各个轴旋转360直至校准完成."));
    const QString calStartedText = QString(tr("将飞机按以下任意未完成的方向放置,并保持不动."));
    const QString otherOrientationDetectedText = QString(tr("将飞机保持在当前方向不动"));
    const QString magOrientationDetectedText = QString(tr("按图示将飞机持续转动直至校准完成"));
    const QString oneSideDoneText = QString(tr("将飞机按以下任意未完成的方向放置,并保持不动."));

    /// interface to control sensors calibration
    SensorsComponentController* _sensorsController ;
    /// widget to show calibration status
    CalRotationWidget* _calOrientationWidget;
};
#endif // SENSORSGROUPPAGE_H
