#ifndef CALROTATIONWIDGET_H
#define CALROTATIONWIDGET_H

/** Widget to show calibration status
  **/
#include <QWidget>
#include "OrientationWidget.h"

namespace Ui {
class CalRotationWidget;
}

class CalRotationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalRotationWidget(QWidget *parent = 0);
    ~CalRotationWidget();

public slots:
    /// show calibration message
    void _status(QString message);

    /// change calibration status of current widget
    void _calStatusChanged(QString side,bool inProgress,bool rotate,bool done);
    /// change visibility of visual calibration(with pictures)
    void _sidesVisibleChanged(int sidesVisible);

signals:
    /// signal to one widget(OrientationWidget) to change status
    void calStatusChanged(bool inProgress,bool rotate,bool done);

private:
    Ui::CalRotationWidget *ui;

    /// widget to show calibration status of each side
    OrientationWidget* _downWidget;
    OrientationWidget* _upsideDownWidget;
    OrientationWidget* _leftWidget;
    OrientationWidget* _rightWidget;
    OrientationWidget* _noseDownWidget;
    OrientationWidget* _tailDownWidget;

    /// widget in calibration progress
    OrientationWidget* _currentWidget;
};

#endif // CALROTATIONWIDGET_H
