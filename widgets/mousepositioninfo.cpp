#include "mousepositioninfo.h"

MousePositionInfo::MousePositionInfo(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(220, 30);
    this->setMinimumSize(220, 70);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setContentsMargins(0, 0, 0, 0);
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    QWidget *mouseInfo = new QWidget();
    mouseInfo->setLayout(gridLayout);
    labelX = new QLabel("-", this);
    labelY = new QLabel("-", this);
    labelAzimut = new QLabel("-", this);
    labelDistance = new QLabel("-", this);
    QLabel* labelXTitle = new QLabel("X:", this);
    labelXTitle->setAlignment(Qt::AlignCenter);
    QLabel* labelYTitle = new QLabel("Y:", this);
    labelYTitle->setAlignment(Qt::AlignCenter);
    QLabel* labelAzimutTitle = new QLabel("A:", this);
    labelAzimutTitle->setAlignment(Qt::AlignCenter);
    QLabel* labelDistanceTitle = new QLabel("D:", this);
    labelDistanceTitle->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(labelXTitle, 0, 0);
    gridLayout->addWidget(labelX, 0, 1);
    gridLayout->addWidget(labelYTitle, 1, 0);
    gridLayout->addWidget(labelY, 1, 1);
    gridLayout->addWidget(labelAzimutTitle, 0, 2);
    gridLayout->addWidget(labelAzimut, 0, 3);
    gridLayout->addWidget(labelDistanceTitle, 1, 2);
    gridLayout->addWidget(labelDistance, 1, 3);

    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    QLabel* labelTitle = new QLabel("КООРДИНАТЫ МАРКЕРА:", this);
    vBoxLayout->addWidget(labelTitle, 1);
    vBoxLayout->addWidget(mouseInfo, 2);



}

void MousePositionInfo::mousePositionSlot(int x, int y, float distance, float azimut){
    labelX->setText(QString::number(x));
    labelY->setText(QString::number(y));
    labelDistance->setText(QString::number(distance));
    labelAzimut->setText(QString::number(azimut));
}

MousePositionInfo::~MousePositionInfo(){

}

