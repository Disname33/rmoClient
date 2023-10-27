#ifndef RMOCLIENT_H
#define RMOCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMouseEvent>
#include <QPainter>
#include <QDateTime>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE


class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void on_buttonRotationStop_clicked();
    void on_buttonRotation3rpm_clicked();
    void on_buttonRotation6rpm_clicked();
    void on_buttonRadiationOff_clicked();
    void on_buttonRadiation50_clicked();
    void on_buttonRadiation100_clicked();

    void mouseMoveEvent(QMouseEvent *) override;

    void slotReadyRead();
    void slotDisconnected();
    void slotError(QAbstractSocket::SocketError);
    void slotReconnect();
    void slotConnected();

private:
    Ui::ClientWindow *ui;
    QTcpSocket *socket;
    QByteArray data;
    quint16 nextBlockSize;
    void sendToServer(QString, QString);
    void errorConnection(QString);
    void setCheckedRotationButton(QString);
    void setCheckedRadiationButton(QString);
    void connectToServer();

protected:
    void paintEvent(QPaintEvent *) override;
    int R, centerX, centerY;
    float beamLineAngle;
    bool isRadiationOn = false;

};
#endif // RMOCLIENT_H
