#include "SensorsGroupPage.h"
#include "ui_SensorsGroupPage.h"

#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>

SensorsGroupPage::SensorsGroupPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SensorsGroupPage),
    _sensorsController(NULL),
    _calOrientationWidget(NULL),
    _accelCalInProgress(false),
    _magCalInProgress(false),
    _gyroCalInProgress(false),
    _calInProgress(false),
    _calRotate(false),
    _calDone(false),
    _sidesVisible(false)
{
    ui->setupUi(this);
    _sidesList <<"down"<<"up"<< "left"<<"right"<<"front"<<"back";
    ui->label_StatusText->setText(statusTextDefault);
    _calOrientationWidget = new CalRotationWidget;
    ui->stackedWidget_Orientations->addWidget(_calOrientationWidget);

    connect(this,&SensorsGroupPage::status,_calOrientationWidget,&CalRotationWidget::_status);
    connect(this,&SensorsGroupPage::calStatusChanged,_calOrientationWidget,&CalRotationWidget::_calStatusChanged);
    //connect(this,&SensorsGroupPage::sidesVisibleChanged,_calOrientationWidget,&CalRotationWidget::_sidesVisibleChanged);
}

SensorsGroupPage::~SensorsGroupPage()
{
    delete ui;
}

void SensorsGroupPage::initSensorsController()
{
    qDebug()<<"SensorsGroupPage::initSensorsController";
    if(!_sensorsController) {
        _sensorsController = new SensorsComponentController;

        connect(ui->pushButton_Mag,&QPushButton::clicked,this,&SensorsGroupPage::_calibrate);
        connect(ui->pushButton_Gyro,&QPushButton::clicked,this,&SensorsGroupPage::_calibrate);
        connect(ui->pushButton_Accel,&QPushButton::clicked,this,&SensorsGroupPage::_calibrate);

        connect(_sensorsController,&SensorsComponentController::handleUASTextMessage,this,&SensorsGroupPage::_handleUASTextMessage);
        connect(this,&SensorsGroupPage::stopCalibration,_sensorsController,&SensorsComponentController::_stopCalibration);
        connect(this,&SensorsGroupPage::sidesVisibleChanged,_calOrientationWidget,&CalRotationWidget::_sidesVisibleChanged);
     }
}

void SensorsGroupPage::_handleUASTextMessage(QString text)
{
    qDebug()<<"text"<<text;
    if(text.contains("progress <")) {
        QString p = text.split('<').last().split('>').first();
        bool ok = false;
        int num = p.toInt(&ok);
        if(ok) {
            ui->progressBar_CalProgress->setProperty("value",float(num));
        }
        return;
    }

    //text "[cal] calibration started: 2 accel". All calibration messages start with [cal]

    //append text to statusTextArea
     //All calibration messages start with [cal]
    QString calPrefix("[cal] ");
    if(!text.startsWith(calPrefix)) {
        return;
    }
    text = text.right(text.length() - calPrefix.length());

    QString calStartPrefix("start calibration: ");
    if(text.startsWith(calStartPrefix)) {

        text = text.right(text.length() - calStartPrefix.length());
        text = text.split(" ").last();
        qDebug()<<"text"<<text;

        _startVisualCalibration();

        if(text == "accel" || text == "mag" || text == "gyro") {

            //All red ,"Incompleted"
            _calInProgress = false;
            _calDone = false;
            _sidesVisible = 0;

            for(int i = 0; i < _sidesList.count();i++) {
                emit calStatusChanged(_sidesList.at(i),_calInProgress,_calRotate,_calDone);
            }
            emit status(calStartedText);

            if(text == "accel") {
                _accelCalInProgress = true;
                _sidesVisible = 63;  // 111111 == 63
            }
            else if(text == "gyro") {
                _gyroCalInProgress = true;
                _sidesVisible = 32;//downSide Visible,100000
            }
            else if (text == "mag") {
                _magCalInProgress = true;
                _sidesVisible = 63;
            }
            else {
                Q_ASSERT(false);
            }
            _setVisualState(_sidesVisible);//set relevant sides visible
        }
        return;
    }

    if(text.endsWith("orientation detected")) {
        QString side = text.section(" ",0,0);

        if(side == "down" || side == "up" || side == "left"
                || side == "right" || side == "front" || side == "back") {
            _calInProgress = true;
           if(_magCalInProgress) {
               _calRotate = true;
           }
            emit calStatusChanged(side,_calInProgress,_calRotate,_calDone);
        }

        if(_magCalInProgress) {
           emit status(magOrientationDetectedText);
        } else {
           emit status(otherOrientationDetectedText);
        }
        return;
    }

    if(text.endsWith("side done, rotate to a different side")) {
        QString side = text.section(" ",0,0);

        if(side == "down" || side == "up" || side == "left"
                || side == "right" || side == "front" || side == "back") {
            _calInProgress = false;
            _calRotate = false;
            _calDone = true;
            emit calStatusChanged(side,_calInProgress,_calRotate,_calDone);
        }
        emit status(oneSideDoneText);
        return;
    }

    if(text.startsWith("calibration success")) {
        _stopCalibration(StopCalibrationSuccess);
        return;
    }

    if(text.startsWith("calibration failed")) {
        _stopCalibration(StopCalibrationFailed);
        return;
    }
}

void SensorsGroupPage::_stopCalibration(StopCalibrationCode code)
{
    emit stopCalibration();

    ui->pushButton_Accel->setEnabled(true);
    ui->pushButton_Mag->setEnabled(true);
    ui->pushButton_Gyro->setEnabled(true);

    if(code == StopCalibrationSuccess) {
        _resetVisualState();
        ui->progressBar_CalProgress->setProperty("value",100);
    } else {
        ui->progressBar_CalProgress->setProperty("value",0);
    }

    if(code == StopCalibrationSuccess) {
        emit status(tr("校准完成"));
        ui->label_StatusText->setText(statusTextDefault);

    } else if(code == StopCalibrationFailed) {
        ui->label_StatusText->setText(statusTextDefault);
        ui->stackedWidget_Orientations->setCurrentWidget(ui->nullWidget);
        ui->label_StatusText->setVisible(true);
        ui->label_StatusText->setStyleSheet("color: red");
        ui->label_StatusText->setText(tr("校准失败"));
    }
    _magCalInProgress = false;
    _accelCalInProgress = false;
    _gyroCalInProgress = false;
}

void SensorsGroupPage::_startVisualCalibration()
{
    ui->pushButton_Accel->setEnabled(false);
    ui->pushButton_Mag->setEnabled(false);
    ui->pushButton_Gyro->setEnabled(false);

    ui->label_StatusText->setVisible(false);
    ui->progressBar_CalProgress->setProperty("value",0);

    _resetVisualState();
}

void SensorsGroupPage::_resetVisualState()
{
    //All green ,"Completed"
    _calInProgress = false;
     _calRotate = false;
     _calDone = true;
     for(int i = 0; i < _sidesList.count();i++) {
         emit calStatusChanged(_sidesList.at(i),_calInProgress,_calRotate,_calDone);
     }
}

void SensorsGroupPage::_setVisualState(int sidesVisible)
{
    if(sidesVisible!=0) {
       ui->stackedWidget_Orientations->setCurrentWidget(_calOrientationWidget);
   }
   emit sidesVisibleChanged(sidesVisible);
}

void SensorsGroupPage::_calibrate()
{
    //qDebug()<<"SensorsGroupPage::_calibrate()";
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString btnName = btn->text();

    //hide all calibration area
    //ui->label_CalAreaHelpText->setVisible(false);

    if (btnName == "Mag") {
         _sensorsController->calibrateMag();
    }
    else if (btnName == "Gyro") {
        _sensorsController->calibrateGyro();
    }
    else if (btnName == "Accel") {
        _sensorsController->calibrateAccel();
    }
}
