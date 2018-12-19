#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include <QListWidget>
#include <QPushButton>

#include "FactMetaData.h"
#include "PX4ParameterMetaData.h"

class SensorsGroupPage;
class SummaryViewPage;
class Vehicle;
class ComponentController;
class MessagePanel;
class ParamPage;

namespace Ui {
class SetupWidget;
}

class SetupWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SetupWidget(QWidget *parent = 0);
    ~SetupWidget();

signals:
    /// signal to MessagePanel to display important messages
     void showMessage(const QString text);

public slots:
     void _activeVehicleChanged(Vehicle* vehicle);

private slots:
     /// called by buttons to show specified panel
     void _showPanel();     
     /// show specified panel
     void _showCompPanel(QString compName);

     /// called when initial parameters loaded succesfully
     void _parametersReadyChanged(bool parametersReady);

private:
     /// show specified page accordingly
     void _showComponentPanel(QString name);

      Ui::SetupWidget *ui;
      Vehicle* _vehicle;

      /// panel for sensors calibration
      SensorsGroupPage* _sensorsPage;
      /// panel for displaying overall calibration status
      SummaryViewPage* _summaryPage;
      /// panel for PID params setting
      ParamPage* _paramPage;

      /// panel for displaying important messages
      MessagePanel*  _messagePanel;
      /// selected button
      QPushButton* _selectedBtn;

      const QString _disconnectedVehicleText = QString(tr("连接飞机，地面站将会自动检测到飞机 "));
};

#endif // SETUPWIDGET_H
