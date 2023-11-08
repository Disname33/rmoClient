#include "radarsettingswidget.h"
#include <QHeaderView>

RadarSettingsWidget::RadarSettingsWidget(QWidget *parent) : QWidget(parent)
{
    QSettings settings("./settings.ini", QSettings::IniFormat);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "Параметр" << "Значение");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    settings.beginGroup("ServerConnection");
    QStringList keys = settings.childKeys();
    int row = 0;
    for (auto &key : qAsConst(keys)) {
        tableWidget->insertRow(row);
        QTableWidgetItem *nameItem = new QTableWidgetItem(key);
        nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
        tableWidget->setItem(row, 0, nameItem);
        tableWidget->setItem(row, 1, new QTableWidgetItem(settings.value(key).toString()));
        row++;
    }
    settings.endGroup();

    QPushButton *saveButton = new QPushButton("Сохранить", this);
    connect(saveButton, &QPushButton::clicked, this, &RadarSettingsWidget::saveSettings);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(tableWidget);
    layout->addWidget(saveButton);
    setLayout(layout);
}

void RadarSettingsWidget::saveSettings() {
    QSettings settings("./settings.ini", QSettings::IniFormat);

    settings.beginGroup("ServerConnection");
    for (int row = 0; row < tableWidget->rowCount(); row++) {
        QString key = tableWidget->item(row, 0)->text();
        QString value = tableWidget->item(row, 1)->text();
        settings.setValue(key, value);
    }
    settings.endGroup();
    settings.sync();
}
