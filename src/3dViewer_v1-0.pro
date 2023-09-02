QT += core gui openglwidgets testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

include(qtGifImage/src/gifimage/qtgifimage.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    c/transformations.c \
    cpp/main.cpp \
    cpp/mainwindow.cpp \
    cpp/opengl.cpp \
    c/parse_obj.c


HEADERS += \
    c/transformations.h \
    cpp/mainwindow.h \
    cpp/opengl.h \
    c/parse_obj.h 

FORMS += \
    ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource_files/res.qrc
