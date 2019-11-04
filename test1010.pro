QT       += core gui

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
    auxiliary_func.cpp \
    collection/flowcollection.cpp \
    collection/odcollection.cpp \
    dockwidget.cpp \
    geometries/flowdata.cpp \
    geometries/oddata.cpp \
    main.cpp \
    mainwindow.cpp \
    visualize/flow_viz.cpp

HEADERS += \
    auxiliary_func.h \
    collection/flowcollection.h \
    collection/odcollection.h \
    dockwidget.h \
    geometries/flowdata.h \
    geometries/oddata.h \
    mainwindow.h \
    visualize/flow_viz.h \
    visualize/flow_viz.h

FORMS += \
    dockwidget.ui \
    mainwindow.ui \
    visualize/flow_viz.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target







#unix:!macx|win32: LIBS += -L$$PWD/../../gdal_debug_x64/lib/ -lgdal

#INCLUDEPATH += $$PWD/../../gdal_debug_x64/include
#DEPENDPATH += $$PWD/../../gdal_debug_x64/include

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../gdal_debug_x64/lib/gdal.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/../../gdal_debug_x64/lib/libgdal.a


unix:!macx|win32: LIBS += -LD:/gdal_debug_x64/lib/ -lgdal

INCLUDEPATH += D:/gdal_debug_x64/include
DEPENDPATH += D:/gdal_debug_x64/include

win32:!win32-g++: PRE_TARGETDEPS += D:/gdal_debug_x64/lib/gdal.lib
else:unix:!macx|win32-g++: PRE_TARGETDEPS += D:/gdal_debug_x64/lib/libgdal.a
