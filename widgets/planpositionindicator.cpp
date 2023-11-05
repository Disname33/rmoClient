#include "planpositionindicator.h"
#include <QtMath>
#include <QDebug>

PlanPositionIndicator::PlanPositionIndicator(QWidget *parent)
//    : QGraphicsView(parent)
    : QWidget(parent)
{
//    indicatorScene = new QGraphicsScene();
//    beamScene = new QGraphicsScene();
//    this->setScene(indicatorScene);
    setCursor(Qt::CrossCursor);
//    this->setRenderHint(QPainter::Antialiasing);
    this->setMouseTracking(true);
    maxDistance = distanceList[0];
    beamLineAzimut = 0.0f;
//    update();
}

// Mouse position
void PlanPositionIndicator::mouseMoveEvent(QMouseEvent *event)
{
    QPoint viewPos = event->pos();
    int x = viewPos.x()-centerX;
    int y = centerY-viewPos.y();
    float distance = sqrt(x*x+y*y)*maxDistance/maxRadius;
    float azimut = atan2(x, y)*180/M_PI;
    if (azimut<0)
        azimut+=360;
    emit mousePositionSignal(x, y, distance, azimut);

}


// Draw indicator
//void PlanPositionIndicator::updateScene() {
//    indicatorScene->clear();

//    QPen pen;
//    pen.setColor(Qt::black);
//    pen.setWidth(1);
//    indicatorScene->addRect(0, 0, width(), height(), pen, QBrush(QColor(30, 30, 30)));

//    centerX = width() / 2;
//    centerY = height() / 2;
//    indicatorScene->addLine(-centerX, 0, centerX, 0, QPen(Qt::white));
//    indicatorScene->addLine(0, -centerY, 0, centerY, QPen(Qt::white));

//    this->viewport()->update();
//}

void PlanPositionIndicator::paintEvent(QPaintEvent *)
//void PlanPositionIndicator::paintUpdate()
{
        QPainter painter;
        QPen pen;
    //    painter.begin(this->viewport());
        painter.begin(this);
        painter.setRenderHint(QPainter::Antialiasing);

        pen.setColor(Qt::black);
        pen.setWidth(1);
        painter.setBrush(isServerConnected?QColor(30, 30, 30):QColor(50, 50, 50));
        painter.setPen(pen);

        // Draw background
        painter.drawRect(0, 0,width(),height());

        // Set coordinat system
        centerX = width()/2;
        centerY = height()/2;
        painter.translate(centerX, centerY);

        // Set maximal radius size
        maxRadius = qMin(centerX - 2, centerY - 2);

        pen.setColor(Qt::white);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);

        // Draw radial lines
        for(int i = 0; i <6; i++){
            painter.drawLine(0, -maxRadius, 0, maxRadius);
            painter.rotate(30.0);
        }
        int circleCount = maxDistance/50;
        float circleRadius = (float)maxRadius/circleCount;
        // Draw circle lines
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        for (int i = 1;i <= 2 ; i++ ) {
            for (int radius = i ; radius<=circleCount; radius+=2) {
                float curentRadius = (float)radius*circleRadius;
                painter.drawEllipse(-curentRadius, -curentRadius, 2 * curentRadius, 2 * curentRadius);
            }
            pen.setColor(Qt::white);
            pen.setWidth(2);
            painter.setPen(pen);
        }
        if (isServerConnected){
            // Draw beam line
            pen.setColor(isRadiationOn?QColor(67, 210, 40, 220):QColor(67, 40, 255, 220));
            painter.setPen(pen);
            painter.rotate(beamLineAzimut);
            painter.drawLine(0, 0, 0, maxRadius);
        } else {
            // Draw text error
            painter.setPen(Qt::darkRed);
            painter.setBrush(Qt::black);
            painter.setFont(QFont("Arial", 14));
            QString connectedErrorMessage = "НЕТ СОЕДИНЕНИЯ С ОСНОВНЫМ РМО";
            QRect textRect = painter.boundingRect(QRect(0,0,width(),height()), Qt::AlignCenter, connectedErrorMessage);
            painter.rotate(180);
            painter.drawRect(-10-textRect.width()/2, -3-textRect.height()/2, textRect.width()+20, textRect.height()+13);
            painter.drawText(-textRect.width()/2, textRect.height()/2, connectedErrorMessage);
        }
        painter.end();
}

void PlanPositionIndicator::setBeamAzimut(float azimut){
    beamLineAzimut=azimut;
    update();
}

void PlanPositionIndicator::setMaxDistance(int distance)
{
    maxDistance=distanceList[distance];
    update();
}

void PlanPositionIndicator::setRadiation(bool isRadiationOn){
    this->isRadiationOn = isRadiationOn;
    update();
}

void PlanPositionIndicator::setIsServerConnected(bool isServerConnected)
{
    this->isServerConnected = isServerConnected;
    update();
}

PlanPositionIndicator::~PlanPositionIndicator(){
}
