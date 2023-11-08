#ifndef RADARSETTINGSWIDGET_H
#define RADARSETTINGSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

class RadarSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RadarSettingsWidget(QWidget *parent = nullptr);

private slots:
    void saveSettings();

private:
    QTableWidget *tableWidget;
};

#endif // RADARSETTINGSWIDGET_H


