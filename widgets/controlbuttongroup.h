#ifndef CONTROLBUTTONGROUP_H
#define CONTROLBUTTONGROUP_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include "radarparameters.h"

class ControlButtonGroup : public QWidget
{
    Q_OBJECT

public:
    ControlButtonGroup(QWidget *parent = nullptr);
    ControlButtonGroup(QWidget *parent, RadarParameters parameter, QString labelText, QString button0Text, QString button1Text, QString button2Text);
    ControlButtonGroup(QWidget *parent, RadarParameters parameter,QString labelText, const QStringList& buttonTexts);
    ~ControlButtonGroup();
    void setCheckedButton(int buttonIndex);
    int getActive();

    signals:
        void clickSignal(RadarParameters parametr, quint8 buttonIndex);

public slots:
    void on_btn_clicked();

private:
    quint8 activeStatus;
    RadarParameters parameter;
    QLabel* label;
    QLabel* doubleDotelabel;
    QList<QPushButton*> buttonList;
};
#endif // CONTROLBUTTONGROUP_H
