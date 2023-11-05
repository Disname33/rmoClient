#ifndef STATIONSTATUS_H
#define STATIONSTATUS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>


class StationStatus : public QWidget
{
    Q_OBJECT
public:
    explicit StationStatus(QWidget *parent = nullptr);
    ~StationStatus();
    void setStatus(bool isNorm);

private:
    QLabel* label;
    QPushButton* statusButton;
};

#endif // STATIONSTATUS_H
