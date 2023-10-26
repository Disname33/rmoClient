#ifndef RMOCLIENT_H
#define RMOCLIENT_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMouseEvent>
#include <QPainter>
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

private:
    Ui::ClientWindow *ui;
    QTcpSocket *socket;
    QByteArray Data;
    void sendToServer(QString, QString);
    void errorConnection(QString);
    void setCheckedRotationButton(QString);
    void setCheckedRadiationButton(QString);
    quint16 nextBlockSize;

protected:
    void paintEvent(QPaintEvent *) override;
    int R, centerX, centerY;
    float beamLineAngle;
    bool isBeanLineRotate = false;
    bool isRadiationOn = false;

};
#endif // RMOCLIENT_H
