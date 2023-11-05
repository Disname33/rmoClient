#include "clientwindow.h"
#include <QSettings>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setObjectName("TestWindows");
    this->setWindowIcon(QIcon("../rmoclient/radar.png"));
    this->setMinimumSize(640, 480);
    this->setWindowTitle("Рабочее окно оператора");
    QWidget *centralWidget = new QWidget();
    QWidget *rightWidget = new QWidget();
    rightWidget->setMaximumSize(270, 16777215);

//    QSettings settings("/rmoconfig/settings.ini", QSettings::IniFormat, this);
    QSettings settings("./settings.ini", QSettings::IniFormat, this);
    settings.beginGroup("ServerConnection");
    serverAddress = settings.value("address", "127.0.0.1").toString();
//    settings.setValue("address", "127.0.0.1");
    serverPort = settings.value("port", 2323).toUInt();
//    settings.setValue("port", 2323);
    settings.endGroup();

    planPositionIndicator = new PlanPositionIndicator(centralWidget);
    rotationButton = new ControlButtonGroup(rightWidget, RadarParameters::RotationSpeed, "ВРАЩ", {"СТОП", "3 ОБ.", "6 ОБ."});
    radiationButton = new ControlButtonGroup(rightWidget, RadarParameters::RadiationPower, "ИЗЛ", {"ВЫКЛ", "50%", "100%"});
    maxDistantionButton = new ControlButtonGroup(rightWidget, RadarParameters::MaxDistance, "ЗАП", {"350", "600", "1200"});
    stationStatus = new StationStatus(rightWidget);
    mousePositionInfo = new MousePositionInfo(rightWidget);
    systemMessageBrowser = new QTextBrowser(rightWidget);


     QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
     rightLayout->setObjectName("rightLayout");
     rightLayout->addWidget(rotationButton);
     rightLayout->addWidget(radiationButton);
     rightLayout->addWidget(maxDistantionButton);
     rightLayout->addWidget(stationStatus);
     rightLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
     rightLayout->addWidget(systemMessageBrowser);
     rightLayout->addWidget(mousePositionInfo);

     QHBoxLayout *mainLayout = new QHBoxLayout();
     mainLayout->setContentsMargins(0, 0, 0, 0);
     mainLayout->addWidget(planPositionIndicator);
     mainLayout->addWidget(rightWidget);
     centralWidget->setLayout(mainLayout);
     setCentralWidget(centralWidget);

     connect(planPositionIndicator, &PlanPositionIndicator::mousePositionSignal, mousePositionInfo, &MousePositionInfo::mousePositionSlot);

     connect(rotationButton, &ControlButtonGroup::clickSignal, this, &ClientWindow::sendToServer);
     connect(radiationButton, &ControlButtonGroup::clickSignal, this, &ClientWindow::sendToServer);
     connect(maxDistantionButton, &ControlButtonGroup::clickSignal, this, &ClientWindow::sendToServer);

     nextBlockSize = 0;
     socket = new QTcpSocket(this);

     QTimer *reconnectTimer = new QTimer(this);
     reconnectTimer->setInterval(5000);
     reconnectTimer->setSingleShot(true);

     connect(socket, &QTcpSocket::readyRead, this, &ClientWindow::slotReadyRead);
     connect(socket, &QTcpSocket::connected, this, &ClientWindow::slotConnected);
     connect(socket, &QTcpSocket::disconnected, this, &ClientWindow::slotDisconnected);
//     connect(socket, &QTcpSocket::errorOccurred, this, &ClientWindow::slotError);
     connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ClientWindow::slotError);
     connect(reconnectTimer, &QTimer::timeout, this, &ClientWindow::slotReconnect);

     connectToServer();
}


//Open connection to server and
void ClientWindow::connectToServer()
{
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost(serverAddress, serverPort);
    }

}


//If connected to server setting green palette to status button if connection successful
void ClientWindow::slotConnected()
{
    stationStatus->setStatus(true);
    planPositionIndicator->setIsServerConnected(true);
    systemMessageBrowser->append(QDateTime::currentDateTime().toString("dd MMMM yyyy HH:mm:ss"));
    systemMessageBrowser->append("Установленно соединение с сервером "+serverAddress);
}

//Receiving and processing messages from the server
void ClientWindow::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_12);

    while (true) {
        if (nextBlockSize == 0) {
            if (socket->bytesAvailable() < 2)
                break;
            in >> nextBlockSize;
        }
        if (socket->bytesAvailable() < nextBlockSize)
            break;
        nextBlockSize = 0;
        RadarParameters parameter;
        float value;
        in  >> (quint32&) parameter>> value;

        switch (parameter) {
            case RadarParameters::RotationSpeed:
                rotationButton->setCheckedButton(int(value));
                break;
            case RadarParameters::RadiationPower:
                radiationButton->setCheckedButton(int(value));
                planPositionIndicator->setRadiation(bool(value));
                break;
            case RadarParameters::AntennaPosition:
//                systemMessageBrowser->append(QString::number(value));
                planPositionIndicator->setBeamAzimut(value);
                break;
            case RadarParameters::MaxDistance:
                maxDistantionButton->setCheckedButton(int(value));
                planPositionIndicator->setMaxDistance(int(value));
                break;
            default:
                qDebug() << "Unknown Radar Parameter";
                break;
        }
    }
}

// Sending message to the server
void ClientWindow::sendToServer(RadarParameters parameter, int buttonIndex)
{
    data.clear();

    QDataStream out(&data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_12);
    if(socket->state() == QAbstractSocket::ConnectedState || socket->state() == QAbstractSocket::ConnectingState)
    {
        out << quint16(0) << parameter << buttonIndex;
        out.device()->seek(0);
        out << quint16(data.size()-sizeof(quint16));
        socket->write(data);
    }
    else
    {
        errorConnection("Is no connection to the server!");
    }
}

//Processing error connection
void ClientWindow::slotError(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::RemoteHostClosedError) {
        qDebug() << "Server closed connection";
    } else {
        //
        qDebug() << "Connection error";
    }
    QTimer::singleShot(5000, this, &ClientWindow::slotReconnect);
    errorConnection(socket->errorString());
}

//If disconnecting when try reconnect in 5 seconds
void ClientWindow::slotDisconnected()
{
    systemMessageBrowser->append(QDateTime::currentDateTime().toString("dd MMMM yyyy HH:mm:ss"));
    systemMessageBrowser->append("Cоединение с сервером разорвано");
    QTimer::singleShot(5000, this, &ClientWindow::slotReconnect);
}

void ClientWindow::slotReconnect()
{
connectToServer();
}

//Setting red palette to status button if connection error
void ClientWindow::errorConnection(QString e)
{
    qDebug() << e;
    stationStatus->setStatus(false);
    planPositionIndicator->setIsServerConnected(false);
}

ClientWindow::~ClientWindow()
{
    socket->close();
    socket->deleteLater();

}

