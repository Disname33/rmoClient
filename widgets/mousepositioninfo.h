#ifndef MOUSEPOSITIONINFO_H
#define MOUSEPOSITIONINFO_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>

class MousePositionInfo : public QWidget
{
    Q_OBJECT

public:
    MousePositionInfo(QWidget *parent = nullptr);
    ~MousePositionInfo();

public slots:
    void mousePositionSlot(int x, int y, float distance, float azimut);

private:
    QLabel* labelX;
    QLabel* labelY;
    QLabel* labelDistance;
    QLabel* labelAzimut;
};
#endif // MOUSEPOSITIONINFO_H
