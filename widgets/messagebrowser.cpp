#include "messagebrowser.h"

#include <QDateTime>

MessageBrowser::MessageBrowser(QWidget *parent)
    :QTabWidget(parent)
{
    systemMessageBrowser = new QTextBrowser(this);
    airObjectInformation = new QTextBrowser(this);
    addTab(systemMessageBrowser, "Сообщения");
    addTab(airObjectInformation, "Справка о ВО");
}

void MessageBrowser::addMessage(QString message){
    systemMessageBrowser->append(QDateTime::currentDateTime().toString("dd MMMM yyyy HH:mm:ss"));
    systemMessageBrowser->append(message);
}
