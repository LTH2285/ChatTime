QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aichat.cpp \
    aipaint.cpp \
    chat.cpp \
    facecharacter.cpp \
    fileoperation.cpp \
    friendrequest.cpp \
    groupchat.cpp \
    home.cpp \
    http.cpp \
    main.cpp \
    register.cpp \
    user.cpp \
    widget.cpp \
    protocol.cpp

HEADERS += \
    aichat.h \
    aipaint.h \
    chat.h \
    chatbubble.h \
    facecharacter.h \
    fileoperation.h \
    friendrequest.h \
    groupchat.h \
    home.h \
    http.h \
    register.h \
    uishadow.h \
    user.h \
    widget.h \
    protocol.h

FORMS += \
    aichat.ui \
    aipaint.ui \
    chat.ui \
    facecharacter.ui \
    fileoperation.ui \
    friendrequest.ui \
    groupchat.ui \
    home.ui \
    register.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc

DISTFILES += \
    图片12.png \
    图片15.png
