#ifndef EXITWIDGET_H
#define EXITWIDGET_H

#include <QWidget>

class ExitWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExitWidget(QWidget *parent = nullptr);

signals:
    void buttonClickedSignal();
private slots:
    void closeWindows();
    void userChange();
    void poweroff();
    void reboot();

};

#endif // EXITWIDGET_H
