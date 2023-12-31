QT       += core gui network sql multimedia

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
    aiaudio.cpp \
    aichat.cpp \
    aipaint.cpp \
    audio.cpp \
    groupchat.cpp \
    friendrequest.cpp \
    httpaudio.cpp \
    receivefiledialog.cpp \
    resetpassword.cpp \
    http.cpp \
    chat.cpp \
    facecharacter.cpp \
    fileoperation.cpp \
    home.cpp \
    main.cpp \
    register.cpp \
    screenwidget.cpp \
    speech.cpp \
    user.cpp \
    widget.cpp \
    protocol.cpp

HEADERS += \
    aiaudio.h \
    aichat.h \
    aipaint.h \
    audio.h \
    groupchat.h \
    friendrequest.h \
    httpaudio.h \
    receivefiledialog.h \
    resetpassword.h \
    http.h \
    chat.h \
    chatbubble.h \
    facecharacter.h \
    fileoperation.h \
    home.h \
    register.h \
    screenwidget.h \
    speech.h \
    uishadow.h \
    user.h \
    widget.h \
    protocol.h

FORMS += \
    aiaudio.ui \
    aichat.ui \
    aipaint.ui \
    receivefiledialog.ui \
    groupchat.ui \
    resetpassword.ui \
    friendrequest.ui \
    chat.ui \
    facecharacter.ui \
    fileoperation.ui \
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
