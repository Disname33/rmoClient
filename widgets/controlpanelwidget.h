#ifndef CONTROLPANELWIDGET_H
#define CONTROLPANELWIDGET_H

#include <QWidget>
#include "widgets/controlbuttongroup.h"
#include "widgets/mousepositioninfo.h"
#include "widgets/messagebrowser.h"
#include "widgets/radarparameters.h"
#include "widgets/stationstatus.h"

class ControlPanelWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanelWidget(QWidget *parent = nullptr);
    void setRadarStatus(bool isNorma);
    void addSystemMessage(QString message);
    void setCheckedButton(RadarParameters parameter, int value);

signals:
    void clickButtonSignal(RadarParameters parametr, quint8 buttonIndex);

public slots:
    void clickButtonSlot(RadarParameters parameter, quint8 buttonIndex);
    void mousePositionSlot(int x, int y, float distance, float azimut);

private:
    ControlButtonGroup* rotationButton;
    ControlButtonGroup* radiationButton;
    ControlButtonGroup* maxDistantionButton;
    StationStatus* stationStatus;
    MessageBrowser* systemMessageBrowser;
    MousePositionInfo* mousePositionInfo;

};

#endif // CONTROLPANELWIDGET_H
