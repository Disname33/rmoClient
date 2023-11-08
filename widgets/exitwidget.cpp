#include "exitwidget.h"

#include <QApplication>
#include <QPushButton>
#include <QVBoxLayout>

ExitWidget::ExitWidget(QWidget *parent) : QWidget(parent)
{
    QPushButton* exitButton = new QPushButton("Выход", this);
    QPushButton* userChangeButton = new QPushButton("Сменить пользователя", this);
    QPushButton* powerOffButton = new QPushButton("Выключить РМО", this);
    QPushButton* rebootButton = new QPushButton("Перезагрузка РМО", this);
    QVBoxLayout* verticalLayout = new QVBoxLayout();
    setLayout(verticalLayout);
    verticalLayout->addWidget(exitButton);
    verticalLayout->addWidget(userChangeButton);
    verticalLayout->addWidget(powerOffButton);
    verticalLayout->addWidget(rebootButton);
    verticalLayout->addItem(new QSpacerItem(2, 2, QSizePolicy::Minimum, QSizePolicy::Expanding));
    connect(exitButton, &QPushButton::clicked, this, &ExitWidget::closeWindows);
    connect(userChangeButton, &QPushButton::clicked, this, &ExitWidget::userChange);
    connect(powerOffButton, &QPushButton::clicked, this, &ExitWidget::poweroff);
    connect(rebootButton, &QPushButton::clicked, this, &ExitWidget::reboot);
}

void ExitWidget::closeWindows()
{
    QApplication::quit();
}

void ExitWidget::userChange()
{
    system("logout");
}

void ExitWidget::poweroff()
{
    system("poweroff");
}

void ExitWidget::reboot()
{
    system("reboot");
}

