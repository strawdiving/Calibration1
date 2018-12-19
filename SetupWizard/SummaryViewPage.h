#ifndef SUMMARYVIEWPAGE_H
#define SUMMARYVIEWPAGE_H

/** Panel to show overall calibration status
  **/
#include <QWidget>
#include <QModelIndex>
#include <QMap>

class SetupWidget;

namespace Ui {
class SummaryViewPage;
}

class SummaryViewPage : public QWidget
{
    Q_OBJECT

public:
    explicit SummaryViewPage(SetupWidget *calWidget, QWidget *parent = 0);
    ~SummaryViewPage();

signals:
    /// signal to SetupWidget to show the specified panel
    void showPanel(QString compName);

public slots:
    /// change messages about setup/calibration
    void _setupCompleteChange(bool setupComplete);
    /// change visual calibration status (with pictures)
    void _setupCompleteStatus(QStringList incomplete);
    /// show the specified panel
    void _showPanel(QModelIndex);

private:
    Ui::SummaryViewPage *ui;
    const QString setupCompleteText = "Below you'll find a summary of the settings for your vehicle."
                                      "To the left are the setup menus for each component";
    const QString needSetupText = "WARNING: Your vehicle requires setup prior to flight.Please resolve"
                                  " the items marked in red using the menu on the left.";

    /// list of component names that need calibration
    QStringList _incompleteComps;
    /// key: iconResource
    /// value: component name
    QMap<QString,QString> _components;
};

#endif // SUMMARYVIEWPAGE_H
