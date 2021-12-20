QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


LIBS += -L/usr/lib64/
LIBS += -L/usr/lib/
LIBS += -L/usr/local/lib/
LIBS += -lcppcrypto -lboost_system -lboost_filesystem -pthread

SOURCES += \
    login_widget.cpp \
    main.cpp \
    mainwindow.cpp \
    mymodel.cpp

HEADERS += \
    login_widget.h \
    mainwindow.h \
    mymodel.h

FORMS += \
    login_widget.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
