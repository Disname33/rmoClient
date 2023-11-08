#ifndef MESSAGEBROWSER_H
#define MESSAGEBROWSER_H

#include <QTabWidget>
#include <QTextBrowser>

class MessageBrowser : public QTabWidget
{
    Q_OBJECT
public:
    MessageBrowser(QWidget *parent = nullptr);
    void addMessage(QString message);
private:
    QTextBrowser* systemMessageBrowser;
    QTextBrowser* airObjectInformation;
};

#endif // MESSAGEBROWSER_H
