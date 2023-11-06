#ifndef PLANPOSITIONINDICATOR_H
#define PLANPOSITIONINDICATOR_H

#include <QtCore/qglobal.h>
#include <QWidget>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QPainter>
#include "radarparameters.h"


class PlanPositionIndicator : public QWidget
{
    Q_OBJECT

public:
    PlanPositionIndicator(QWidget *parent = nullptr);
    ~PlanPositionIndicator();
    void setBeamAzimut(float azimut);
    void setMaxDistance(int distance);
    void setRadiation(bool isRadiationOn);

    void setIsServerConnected(bool newIsServerConnected);

signals:
        void mousePositionSignal(int x, int y, float distance, float azimut);

public slots:
    void mouseMoveEvent(QMouseEvent *) override;

protected:
    int distanceList[3]{350,600,1200};
    bool isServerConnected = false;
    void paintEvent(QPaintEvent *) override;
    int centerX, centerY, maxDistance, maxRadius;
    float beamLineAzimut;
    bool isRadiationOn = false;

};
#endif // PLANPOSITIONINDICATOR_H
