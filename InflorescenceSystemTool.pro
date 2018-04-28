#-------------------------------------------------
#
# Project created by QtCreator 2017-09-22T01:50:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InflorescenceSystemTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += /opt/cuda/include
LIBS += -lglog \
        -lboost_system \
        -lcaffe \
        -lprotobuf


SOURCES += \
    main.cpp \
    inflorescencesystemwindow.cpp \
    util/filedirselectionwidget.cpp \
    extractordialog/extractorconfigdialog.cpp \
    extractordialog/chtextractorconfigdialog.cpp \
    budextractortestwidget.cpp \
    extractortest/budextractortest.cpp \
    extractortest/imageextractortest.cpp \
    extractordialog/frstextractorconfigdialog.cpp \
    parametertesterwidget.cpp \
    convolutionalsegmenter/convolutionalsegmenter.cpp \
    convolutionalsegmenter/convolutionalsegmenterparameters.cpp \
    convolutionalsegmenter/convolutionalsegmenterparameterdialog.cpp \
    segmentationruntimetester.cpp \
    budextractorruntimetestwidget.cpp \
    confusionmatrix.cpp \
    segmentationtesterwidget.cpp \
    budextractorwidget.cpp \
    inflorescencecollection.cpp

HEADERS += \
    inflorescencesystemwindow.h \
    util/filedirselectionwidget.h \
    extractordialog/extractorconfigdialog.h \
    extractordialog/chtextractorconfigdialog.h \
    budextractortestwidget.h \
    extractortest/budextractortest.h \
    extractortest/imageextractortest.h \
    extractordialog/frstextractorconfigdialog.h \
    parametertesterwidget.h \
    convolutionalsegmenter/convolutionalsegmenter.h \
    convolutionalsegmenter/convolutionalsegmenterparameters.h \
    convolutionalsegmenter/convolutionalsegmenterparameterdialog.h \
    segmentationruntimetester.h \
    budextractorruntimetestwidget.h \
    confusionmatrix.h \
    segmentationtesterwidget.h \
    budextractorwidget.h \
    inflorescencecollection.h

include(extractors/extractors.pri)
include(imagelib/imagelib.pri)
