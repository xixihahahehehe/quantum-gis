QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += charts

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
    collection/flowgraph.cpp \
    dialog/chartviewdialog.cpp \
    dialog/gen_graph_dialog.cpp \
    dialog/open_odshp.cpp \
    geometries/flowdata.cpp \
    geometries/oddata.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_graphrelated.cpp \
    threads/workers.cpp \
    visualize/flow_viz.cpp \
    visualize/menuwidget.cpp \
    visualize/propertytable.cpp \
    method/CalculateBtwId.cpp \
    method/DistanceDecayPara.cpp

HEADERS += \
    auxiliary_func.h \
    collection/flowcollection.h \
    collection/odcollection.h \
    collection/flowgraph.h \
    dialog/chartviewdialog.h \
    dialog/gen_graph_dialog.h \
    dialog/open_odshp.h \
    geometries/flowdata.h \
    geometries/oddata.h \
    mainwindow.h \
    threads/workers.h \
    visualize/flow_viz.h \
    visualize/menuwidget.h \
    visualize/propertytable.h \
    method/CalculateBtwId.h \
    method/DistanceDecayPara.h

FORMS += \
    dialog/chartviewdialog.ui \
    dialog/gen_graph_dialog.ui \
    dialog/open_odshp.ui \
    mainwindow.ui \
    visualize/flow_viz.ui \
    visualize/menuwidget.ui \
    visualize/propertytable.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#unix:!macx|win32: LIBS += -LD:/gdal_debug_x64/lib/ -lgdal

#INCLUDEPATH += D:/gdal_debug_x64/include
#DEPENDPATH += D:/gdal_debug_x64/include

#win32:!win32-g++: PRE_TARGETDEPS += D:/gdal_debug_x64/lib/gdal.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += D:/gdal_debug_x64/lib/libgdal.a

win32: LIBS += -L$$PWD/../gdal_debug_x64/lib/ -lgdal

INCLUDEPATH += $$PWD/../gdal_debug_x64/include
DEPENDPATH += $$PWD/../gdal_debug_x64/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../gdal_debug_x64/lib/gdal.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../gdal_debug_x64/lib/libgdal.a

win32: LIBS += -L$$PWD/../OSGeo4W/lib/ -lproj_6_1_d

INCLUDEPATH += $$PWD/../OSGeo4W/include
DEPENDPATH += $$PWD/../OSGeo4W/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../OSGeo4W/lib/proj_6_1_d.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../OSGeo4W/lib/libproj_6_1_d.a

win32: LIBS += -L$$PWD/../igraph/lib/ -ligraph

INCLUDEPATH += $$PWD/../igraph/include
DEPENDPATH += $$PWD/../igraph/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../igraph/lib/igraph.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/../igraph/lib/libigraph.a

unix:!macx|win32: LIBS += -L$$PWD/../geos/lib/ -lgeos_d

INCLUDEPATH += $$PWD/../geos/include
DEPENDPATH += $$PWD/../geos/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../geos/lib/geos_d.lib
else:unix:!macx|win32-g++: PRE_TARGETDEPS += $$PWD/../geos/lib/libgeos_d.a
