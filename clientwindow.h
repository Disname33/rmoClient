#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QtCore>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif
#include <QTcpSocket>
#include <QDateTime>
#include <QDataStream>
#include <QTimer>
#include <QTextBrowser>
#include "widgets/controlbuttongroup.h"
#include "widgets/planpositionindicator.h"
#include "widgets/mousepositioninfo.h"
#include "widgets/radarparameters.h"
#include "widgets/stationstatus.h"

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void slotReadyRead();
    void slotDisconnected();
    void slotError(QAbstractSocket::SocketError);
    void slotReconnect();
    void slotConnected();
    void sendToServer(RadarParameters parameter, int buttonIndex);

private:
    QTcpSocket* socket;
    QByteArray data;
    quint16 nextBlockSize;
    QTextBrowser* systemMessageBrowser;
    QString serverAddress;
    quint16 serverPort;
    void errorConnection(QString);
    void connectToServer();

private:
    ControlButtonGroup* rotationButton;
    ControlButtonGroup* radiationButton;
    ControlButtonGroup* maxDistantionButton;
    StationStatus* stationStatus;
    PlanPositionIndicator* planPositionIndicator;
    MousePositionInfo* mousePositionInfo;

};
#endif // CLIENTWINDOW_H
