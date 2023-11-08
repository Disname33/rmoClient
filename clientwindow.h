#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QToolBar>
#include <QVector>
#include <QtCore>
#if QT_VERSION >= 0x050000
#include <QtWidgets/QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif
#include <QTcpSocket>
#include <QDataStream>
#include <QLabel>
#include <QTextBrowser>
#include <QDateTime>
#include <QTimer>
#include <QAction>
#include "widgets/radarparameters.h"
#include "widgets/planpositionindicator.h"
#include "widgets/controlpanelwidget.h"
#include "widgets/radarsettingswidget.h"
#include "widgets/exitwidget.h"


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
    void sendToServer(RadarParameters parameter, quint8 buttonIndex);

    void changeRightPanel();
    void updateDateTime();

private:
    QTcpSocket* socket;
    QByteArray data;
    quint16 nextBlockSize;
    QString serverAddress;
    quint16 serverPort;

    QToolBar* menuToolBar;
    QLabel* dateLabel;
    QLabel* timeLabel;

    ControlPanelWidget* controlPanelWidget;
    RadarSettingsWidget* settingsWidget;
    QWidget* infoWidget;
    ExitWidget* exitWidget;

    QVector <QAction*> menuActions;

    PlanPositionIndicator* planPositionIndicator;
    void errorConnection(QString error);
    void connectToServer();

    void createMenuToolBar();


};
#endif // CLIENTWINDOW_H
