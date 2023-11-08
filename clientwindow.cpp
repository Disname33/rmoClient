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
    QHBoxLayout *rightLayout = new QHBoxLayout();

    controlPanelWidget = new ControlPanelWidget(rightWidget);
    settingsWidget = new RadarSettingsWidget(rightWidget);
    infoWidget = new QWidget(rightWidget);
    exitWidget = new ExitWidget(rightWidget);
    settingsWidget->hide();
    infoWidget->hide();
    exitWidget->hide();
    rightWidget->setMaximumSize(270, 16777215);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->addWidget(controlPanelWidget);
    rightLayout->addWidget(settingsWidget);
    rightLayout->addWidget(infoWidget);
    rightLayout->addWidget(exitWidget);
    rightWidget->setLayout(rightLayout);

    QSettings settings("./settings.ini", QSettings::IniFormat, this);
    settings.beginGroup("ServerConnection");
    serverAddress = settings.value("address", "127.0.0.1").toString();
    serverPort = settings.value("port", 2323).toUInt();
    settings.endGroup();

    planPositionIndicator = new PlanPositionIndicator(centralWidget);

     QHBoxLayout *mainLayout = new QHBoxLayout();
     mainLayout->setContentsMargins(0, 0, 0, 0);
     mainLayout->addWidget(planPositionIndicator);
     mainLayout->addWidget(rightWidget);
     centralWidget->setLayout(mainLayout);
     setCentralWidget(centralWidget);

     createMenuToolBar();


     connect(controlPanelWidget, &ControlPanelWidget::clickButtonSignal, this, &ClientWindow::sendToServer);
     connect(planPositionIndicator, &PlanPositionIndicator::mousePositionSignal, controlPanelWidget, &ControlPanelWidget::mousePositionSlot);


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

void ClientWindow::createMenuToolBar()
{
    menuToolBar = addToolBar("Menu");
    menuToolBar->setMovable(false);
    menuActions.push_back(menuToolBar->addAction("Пульт РЛС"));
    menuActions.push_back(menuToolBar->addAction("Настройка"));
    menuActions.push_back(menuToolBar->addAction("Информация"));
    QWidget* spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    menuToolBar->addWidget(spacer);
    dateLabel = new QLabel(this);
    dateLabel->setMargin(2);
    dateLabel->setPalette(QPalette(Qt::white));
    timeLabel = new QLabel(this);
    timeLabel->setMargin(2);
    dateLabel->setPalette(QPalette(Qt::white));
    updateDateTime();
    menuToolBar->addWidget(dateLabel);
    menuToolBar->addWidget(timeLabel);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ClientWindow::updateDateTime);
    timer->start(1000);
    menuActions.push_back(menuToolBar->addAction("Выход"));

    for (auto menuAction : qAsConst(menuActions)) {
        connect(menuAction, &QAction::triggered, this, &ClientWindow::changeRightPanel);
        menuAction->setCheckable(true);
    }

    menuActions[0]->setChecked(true);

}

void ClientWindow::updateDateTime()
{
    dateLabel->setText(QDate::currentDate().toString(" dd.MM.yyyy "));
    timeLabel->setText(QTime::currentTime().toString(" hh:mm:ss "));
}


//If connected to server setting green palette to status button if connection successful
void ClientWindow::slotConnected()
{
    controlPanelWidget->setRadarStatus(true);
    planPositionIndicator->setIsServerConnected(true);
    controlPanelWidget->addSystemMessage("Установленно соединение с сервером "+serverAddress);
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
                controlPanelWidget->setCheckedButton(parameter, int(value));
                break;
            case RadarParameters::RadiationPower:
                controlPanelWidget->setCheckedButton(parameter, int(value));
                planPositionIndicator->setRadiation(bool(value));
                break;
            case RadarParameters::AntennaPosition:
                planPositionIndicator->setBeamAzimut(value);
                break;
            case RadarParameters::MaxDistance:
                controlPanelWidget->setCheckedButton(parameter, int(value));
                planPositionIndicator->setMaxDistance(int(value));
                break;
            default:
                qDebug() << "Unknown Radar Parameter";
                break;
        }
    }
}

// Sending message to the server
void ClientWindow::sendToServer(RadarParameters parameter, quint8 buttonIndex)
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

void ClientWindow::changeRightPanel()
{
    QAction* action = (QAction*)sender();
    qInfo() << action->text();
    for (auto menuAction : qAsConst(menuActions)) {
        menuAction->setChecked(menuAction == action);
    }

    controlPanelWidget->setVisible(menuActions[0] == action);
    settingsWidget->setVisible(menuActions[1] == action);
    infoWidget->setVisible(menuActions[2] == action);
    exitWidget->setVisible(menuActions[3] == action);
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
    controlPanelWidget->addSystemMessage("Cоединение с сервером разорвано");
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
    controlPanelWidget->setRadarStatus(false);
    planPositionIndicator->setIsServerConnected(false);
}


ClientWindow::~ClientWindow()
{
    socket->close();
    socket->deleteLater();
    qDeleteAll(menuActions);
}

