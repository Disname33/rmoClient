#include "controlbuttongroup.h"

ControlButtonGroup::ControlButtonGroup(QWidget *parent)
    : ControlButtonGroup(parent, RadarParameters::RotationSpeed, "ВРАЩ", {"ОТКЛ", "3", "6"})
{
}

ControlButtonGroup::ControlButtonGroup(QWidget *parent, RadarParameters parameter, QString labelText, QString button0Text, QString button1Text, QString button2Text)
    : ControlButtonGroup(parent, parameter, labelText, {button0Text, button1Text, button2Text})
{
}

ControlButtonGroup::ControlButtonGroup(QWidget *parent, RadarParameters parameter, QString labelText, const QStringList& buttonTexts)
    : QWidget(parent, Qt::FramelessWindowHint)
{
    this->parameter = parameter;
    this->setMinimumSize(220, 30);
    setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    label = new QLabel(labelText, this);
    doubleDotelabel = new QLabel(":", this);
    layout->addWidget(label);
    layout->addWidget(doubleDotelabel);


    for (int i = 0; i < buttonTexts.size(); i++) {
        QPushButton* button = new QPushButton(this);
        button->setObjectName("btn_" + QString::number(i));
        button->setText(buttonTexts[i]);
        layout->addWidget(button);
        buttonList.append(button);

        connect(button, &QPushButton::clicked, this, &ControlButtonGroup::on_btn_clicked);
    }

}

void ControlButtonGroup::on_btn_clicked(){
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());

    if (clickedButton) {
        QString buttonNumber = clickedButton->objectName().mid(4);
        bool conversionOK = false;
        int buttonIndex = buttonNumber.toInt(&conversionOK);

        if (conversionOK) {
            emit clickSignal(parameter, buttonIndex);
        }
    }
}


void ControlButtonGroup::setCheckedButton(int buttonIndex){
    QPalette palette = this->palette();
    activeStatus = buttonIndex;

    for (int i = 0; i < buttonList.size(); i++) {
        palette.setColor(QPalette::Button, buttonIndex == i?Qt::darkGreen:QColor(239,239,239));
        buttonList[i]->setPalette(palette);
    }
}


int ControlButtonGroup::getActive(){
    return activeStatus;
}


ControlButtonGroup::~ControlButtonGroup(){

    delete label;
    delete doubleDotelabel;

    qDeleteAll(buttonList);
}

