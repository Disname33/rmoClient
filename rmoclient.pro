QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
# Пути к заголовочным файлам

# Файлы исходного кода (cpp файлы) для включения в сборку

TARGET = rmoclient

HEADERS += widgets/controlbuttongroup.h \
    widgets/controlpanelwidget.h \
    widgets/exitwidget.h \
    widgets/messagebrowser.h \
    widgets/radarsettingswidget.h \
    widgets/stationstatus.h \
    widgets/radarparameters.h \
    widgets/planpositionindicator.h \
    widgets/mousepositioninfo.h \
    clientwindow.h

SOURCES += widgets/controlbuttongroup.cpp \
               widgets/controlpanelwidget.cpp \
               widgets/exitwidget.cpp \
               widgets/messagebrowser.cpp \
               widgets/radarsettingswidget.cpp \
               widgets/stationstatus.cpp \
               widgets/planpositionindicator.cpp \
               widgets/mousepositioninfo.cpp \
               main.cpp \
               clientwindow.cpp

win32:RC_FILE = res.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md \
    settings.ini
