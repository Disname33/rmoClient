#include "stationstatus.h"

StationStatus::StationStatus(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(220, 30);
    setContentsMargins(0, 0, 0, 0);

    label = new QLabel("СОСТОЯНИЕ РЛС:", this);
    statusButton = new QPushButton(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(label);
    layout->addWidget(statusButton);
    setStatus(false);
}

StationStatus::~StationStatus()
{
    delete label;
    delete statusButton;
}

void StationStatus::setStatus(bool isNorm)
{
    QPalette palette = this->palette();
    if (isNorm){
        palette.setColor(QPalette::Button, Qt::darkGreen);
        statusButton->setPalette(palette);
        statusButton->setText("НОРМА");
    } else
    {
        palette.setColor(QPalette::Button, Qt::darkRed);
        statusButton->setPalette(palette);
        statusButton->setText("АВАРИЯ");
    }
}
