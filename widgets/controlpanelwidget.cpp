#include "controlpanelwidget.h"

ControlPanelWidget::ControlPanelWidget(QWidget *parent) : QWidget(parent)
{
    setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    rotationButton = new ControlButtonGroup(this, RadarParameters::RotationSpeed, "ВРАЩ", {"СТОП", "3 ОБ.", "6 ОБ."});
    radiationButton = new ControlButtonGroup(this, RadarParameters::RadiationPower, "ИЗЛ", {"ВЫКЛ", "50%", "100%"});
    maxDistantionButton = new ControlButtonGroup(this, RadarParameters::MaxDistance, "ЗАП", {"350", "600", "1200"});
    stationStatus = new StationStatus(this);
    systemMessageBrowser = new MessageBrowser(this);
    mousePositionInfo = new MousePositionInfo(this);

    QVBoxLayout *rightLayout = new QVBoxLayout(this);
    rightLayout->setObjectName("rightLayout");
    rightLayout->addWidget(rotationButton);
    rightLayout->addWidget(radiationButton);
    rightLayout->addWidget(maxDistantionButton);
    rightLayout->addWidget(stationStatus);
//    rightLayout->addItem(new QSpacerItem(2, 2, QSizePolicy::Minimum, QSizePolicy::Expanding));
    rightLayout->addWidget(systemMessageBrowser);
    rightLayout->addWidget(mousePositionInfo);

    connect(rotationButton, &ControlButtonGroup::clickSignal, this, &ControlPanelWidget::clickButtonSlot);
    connect(radiationButton, &ControlButtonGroup::clickSignal, this, &ControlPanelWidget::clickButtonSlot);
    connect(maxDistantionButton, &ControlButtonGroup::clickSignal, this, &ControlPanelWidget::clickButtonSlot);

}

void ControlPanelWidget::setRadarStatus(bool isNorma)
{
    stationStatus->setStatus(isNorma);
}

void ControlPanelWidget::addSystemMessage(QString message)
{
    systemMessageBrowser->addMessage(message);
}

void ControlPanelWidget::setCheckedButton(RadarParameters parameter, int value)
{
    switch (parameter) {
        case RadarParameters::RotationSpeed:
            rotationButton->setCheckedButton(int(value));
            break;
        case RadarParameters::RadiationPower:
            radiationButton->setCheckedButton(int(value));
            break;
        case RadarParameters::MaxDistance:
            maxDistantionButton->setCheckedButton(int(value));
            break;
        default:
            break;
    }
}

void ControlPanelWidget::clickButtonSlot(RadarParameters parameter, quint8 buttonIndex)
{
    emit clickButtonSignal(parameter, buttonIndex);
}

void ControlPanelWidget::mousePositionSlot(int x, int y, float distance, float azimut)
{
    mousePositionInfo->mousePositionSlot(x, y, distance, azimut);
}
