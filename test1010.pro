QT       += core gui
QT       += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += charts
CONFIG += qwt
DEFINES += QT_DLL QWT_DLL
LIBS += -L"D:\Qt\Qt5.12.5\build-qwt-Desktop_Qt_5_12_5_MSVC2017_64bit-Debug\lib" -lqwtd
LIBS += -L"D:\Qt\Qt5.12.5\build-qwt-Desktop_Qt_5_12_5_MSVC2017_64bit-Debug\lib" -lqwt
INCLUDEPATH += D:\Qt\Qt5.12.5\5.12.5\msvc2017_64\include\qwt

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
    arrow.cpp \
    auxiliary_func.cpp \
    charts/QWChartView.cpp \
    charts/charts.cpp \
    collection/flowcollection.cpp \
    collection/odcollection.cpp \
    collection/flowgraph.cpp \
    dialog/bindingdialog.cpp \
    dialog/chartviewdialog.cpp \
    dialog/gen_graph_dialog.cpp \
    dialog/graphexploredlg.cpp \
    dialog/open_odshp.cpp \
    geometries/flowdata.cpp \
    geometries/oddata.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_graphrelated.cpp \
    method/histogram.cpp \
    pietest/cpiewidget.cpp \
    pietest/pietest.cpp \
    src/prpack.cpp \
    src/prpack/prpack_base_graph.cpp \
    src/prpack/prpack_igraph_graph.cpp \
    src/prpack/prpack_preprocessed_ge_graph.cpp \
    src/prpack/prpack_preprocessed_gs_graph.cpp \
    src/prpack/prpack_preprocessed_scc_graph.cpp \
    src/prpack/prpack_preprocessed_schur_graph.cpp \
    src/prpack/prpack_result.cpp \
    src/prpack/prpack_solver.cpp \
    src/prpack/prpack_utils.cpp \
    threads/workers.cpp \
    visualize/flow_viz.cpp \
    visualize/flow_viz_opengl.cpp \
    visualize/menuwidget.cpp \
    visualize/propertytable.cpp \
    method/CalculateBtwId.cpp \
    method/DistanceDecayPara.cpp

HEADERS += \
    arrow.h \
    auxiliary_func.h \
    charts/QWChartView.h \
    charts/charts.h \
    collection/flowcollection.h \
    collection/odcollection.h \
    collection/flowgraph.h \
    dialog/bindingdialog.h \
    dialog/chartviewdialog.h \
    dialog/gen_graph_dialog.h \
    dialog/graphexploredlg.h \
    dialog/open_odshp.h \
    geometries/flowdata.h \
    geometries/oddata.h \
    mainwindow.h \
    method/histogram.h \
    pietest/cpiewidget.h \
    pietest/pietest.h \
    src/prpack.h \
    src/prpack/prpack.h \
    src/prpack/prpack_base_graph.h \
    src/prpack/prpack_csc.h \
    src/prpack/prpack_csr.h \
    src/prpack/prpack_edge_list.h \
    src/prpack/prpack_igraph_graph.h \
    src/prpack/prpack_preprocessed_ge_graph.h \
    src/prpack/prpack_preprocessed_graph.h \
    src/prpack/prpack_preprocessed_gs_graph.h \
    src/prpack/prpack_preprocessed_scc_graph.h \
    src/prpack/prpack_preprocessed_schur_graph.h \
    src/prpack/prpack_result.h \
    src/prpack/prpack_solver.h \
    src/prpack/prpack_utils.h \
    threads/workers.h \
    visualize/flow_viz.h \
    visualize/flow_viz_opengl.h \
    visualize/menuwidget.h \
    visualize/propertytable.h \
    method/CalculateBtwId.h \
    method/DistanceDecayPara.h

FORMS += \
    charts/charts.ui \
    dialog/bindingdialog.ui \
    dialog/chartviewdialog.ui \
    dialog/gen_graph_dialog.ui \
    dialog/graphexploredlg.ui \
    dialog/open_odshp.ui \
    mainwindow.ui \
    pietest/pietest.ui \
    visualize/flow_viz.ui \
    visualize/flow_viz_opengl.ui \
    visualize/menuwidget.ui \
    visualize/propertytable.ui

LIBS += -lopengl32 -lglu32

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#unix:!macx|win32: LIBS += -LD:/gdal_debug_x64/lib/ -lgdal

#INCLUDEPATH += D:/gdal_debug_x64/include
#DEPENDPATH += D:/gdal_debug_x64/include

#win32:!win32-g++: PRE_TARGETDEPS += D:/gdal_debug_x64/lib/gdal.lib
#else:unix:!macx|win32-g++: PRE_TARGETDEPS += D:/gdal_debug_x64/lib/libgdal.a

win32: LIBS += -LD:/gdal_debug_x64/lib/ -lgdal

INCLUDEPATH += D:/gdal_debug_x64/include
DEPENDPATH += D:/gdal_debug_x64/include

win32:!win32-g++: PRE_TARGETDEPS += D:/gdal_debug_x64/lib/gdal.lib
else:win32-g++: PRE_TARGETDEPS += D:/gdal_debug_x64/lib/libgdal.a

win32: LIBS += -LD:/GEOS/lib/ -lgeos_c

INCLUDEPATH += D:/GEOS/include
DEPENDPATH += D:/GEOS/include

win32:!win32-g++: PRE_TARGETDEPS += D:/GEOS/lib/geos_c.lib
else:win32-g++: PRE_TARGETDEPS += D:/GEOS/lib/libgeos_c.a

win32: LIBS += -LD:/GEOS/lib/ -llibgeos

INCLUDEPATH += D:/GEOS/include
DEPENDPATH += D:/GEOS/include

win32:!win32-g++: PRE_TARGETDEPS += D:/GEOS/lib/libgeos.lib
else:win32-g++: PRE_TARGETDEPS += D:/GEOS/lib/liblibgeos.a

win32: LIBS += -LD:/igraph/lib/ -ligraph

INCLUDEPATH += D:/igraph/include
DEPENDPATH += D:/igraph/include

win32:!win32-g++: PRE_TARGETDEPS += D:/igraph/lib/igraph.lib
else:win32-g++: PRE_TARGETDEPS += D:/igraph/lib/libigraph.a

win32: LIBS += -LD:/OSGeo4W/lib/ -lproj_6_1_d

INCLUDEPATH += D:/OSGeo4W/include
DEPENDPATH += D:/OSGeo4W/include

win32:!win32-g++: PRE_TARGETDEPS += D:/OSGeo4W/lib/proj_6_1_d.lib
else:win32-g++: PRE_TARGETDEPS += D:/OSGeo4W/lib/libproj_6_1_d.a


DISTFILES += \
    src/prpack/prpack.inc
