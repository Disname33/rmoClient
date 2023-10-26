#include "rmoclient.h"
#include "./ui_rmoclient.h"
#include <QtMath>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    beamLineAngle = 0.0f;
    nextBlockSize = 0;
    socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 2323);
    //ToDo add reconnect
    connect(socket, &QTcpSocket::readyRead, this, &ClientWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    sendToServer("Connect", "Ok");
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::setCheckedRotationButton(QString buttonText){
    QPushButton* buttons[3] = {ui->buttonRotationStop, ui->buttonRotation3rpm, ui->buttonRotation6rpm};
    for (auto btn : buttons)
    {
        btn->setPalette(QPalette(btn->text() == buttonText?Qt::darkGreen:QColor(239,239,239)));
    }

}

void ClientWindow::on_buttonRotationStop_clicked()
{
    ui->buttonRotationStop->setPalette(QPalette(Qt::yellow));
    sendToServer("rotation", ui->buttonRotationStop->text());

}


void ClientWindow::on_buttonRotation3rpm_clicked()
{

    ui->buttonRotation3rpm->setPalette(QPalette(Qt::yellow));
    sendToServer("rotation", ui->buttonRotation3rpm->text());
}

void ClientWindow::on_buttonRotation6rpm_clicked()
{
    ui->buttonRotation6rpm->setPalette(QPalette(Qt::yellow));
    sendToServer("rotation", ui->buttonRotation6rpm->text());
}


void ClientWindow::setCheckedRadiationButton(QString buttonText){
    QPushButton* buttons[3] = {ui->buttonRadiationOff, ui->buttonRadiation50, ui->buttonRadiation100};
    isRadiationOn = buttonText != "ОТКЛ";
    for (auto btn : buttons)
    {
        btn->setPalette(QPalette(btn->text() == buttonText?Qt::darkGreen:QColor(239,239,239)));
    }

}

void ClientWindow::on_buttonRadiationOff_clicked()
{
     sendToServer("radiation", ui->buttonRadiationOff->text());
}


void ClientWindow::on_buttonRadiation50_clicked()
{
    sendToServer("radiation", ui->buttonRadiation50->text());
}


void ClientWindow::on_buttonRadiation100_clicked()
{
    sendToServer("radiation", ui->buttonRadiation100->text());
}

// Обработка события движения мыши
void ClientWindow::mouseMoveEvent(QMouseEvent *event)
{
    QPoint viewPos = event->pos();
    int x = viewPos.x()-centerX;
    int y = centerY-viewPos.y();
    ui->label_X->setText(QString::number(x));
    ui->label_Y->setText(QString::number(y));
    float distance = sqrt(x*x+y*y)*350/R;
    ui->label_D->setText(QString::number(distance));
    float angle = atan2(x, y)*180/M_PI;
    if (angle<0)
        angle+=360;
    ui->label_A->setText(QString::number(angle));
}

void ClientWindow::paintEvent(QPaintEvent *)
{

    QPainter painter;

    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidth(1);
    painter.setBrush(Qt::black);
    painter.setPen(pen);

    int indicatorHeight = (ui->statusbar->y()-ui->menubar->height())/2;
    painter.drawRect(0, ui->menubar->height(), ui->rightbar->x(),indicatorHeight*2);
    //coordinat system
    centerX = ui->rightbar->x()/2;
    centerY = indicatorHeight+ui->menubar->height();
    painter.translate(centerX, centerY);

    //size
    R = qMin((ui->rightbar->x()+4)/2,indicatorHeight-2);

    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    //clock face
    for(int i = 0; i <12; i++){
        painter.drawLine(0, 0, 0, R);
        painter.rotate(30.0);
    }

    for (int radius =R/7 ; radius<=R; radius+=2*R/7) {
        painter.drawEllipse(-radius, -radius, 2 * radius, 2 * radius);
    }
    pen.setWidth(2);
    painter.setPen(pen);
    for (int radius =2*R/7 ; radius<=R; radius+=2*R/7) {
        painter.drawEllipse(-radius, -radius, 2 * radius, 2 * radius);
    }

    painter.save();
    painter.setPen(isRadiationOn?QColor(67, 210, 40, 220):QColor(67, 40, 255, 220));
    painter.rotate(beamLineAngle);
    painter.drawLine(0, 0, 0, R);
    painter.restore();

    painter.end();
}


void ClientWindow::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_12);

    if(in.status()==QDataStream::Ok)
    {
        for (; ; )
        {
            if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable() < 2)
                    break;
                in >> nextBlockSize;
            }
            if(socket->bytesAvailable()<nextBlockSize)
                break;
            nextBlockSize = 0;
            QString action;
            QString str;
            in >> action >> str;
            qDebug() << str;
            if (action == "angle")
            {
                bool ok;
                float floatValue = str.toFloat(&ok);
                if (ok){
                    beamLineAngle = floatValue;
                    update();
                }
                else
                    qDebug() << "Ошибка конвертации строки в float";
            }

            else if (action == "rotation")
                setCheckedRotationButton(str);
            else if (action == "radiation"){
                setCheckedRadiationButton(str);
                update();
            }
            ui->stationStatusButton->setText("НОРМА");
            ui->stationStatusButton->setPalette(QPalette(Qt::darkGreen));
            break;
        }

    }
    else
    {
       errorConnection("is no connection to the server!");
    }
}

void ClientWindow::sendToServer(QString action, QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    if(socket->state() == QAbstractSocket::ConnectedState || socket->state() == QAbstractSocket::ConnectingState)
    {
        out << quint16(0) << action << str;
        out.device()->seek(0);
        out << quint16(Data.size()-sizeof(quint16));
        socket->write(Data);
    }
    else
    {
        errorConnection("is no connection to the server!");
    }

}

void ClientWindow::errorConnection(QString e)
{
    qDebug() << "is no connection to the server!";
    ui->stationStatusButton->setText("АВАРИЯ");
    ui->stationStatusButton->setPalette(QPalette(Qt::darkRed));
}

