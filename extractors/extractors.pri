CONFIG += c++11

LIBS += `pkg-config --libs opencv`
INCLUDEPATH += $$PWD

SOURCES += \
    $$PWD/budextractor.cpp \
    $$PWD/cannyoperator.cpp \
    $$PWD/circle.cpp \
    $$PWD/circledrawer.cpp \
    $$PWD/CircularHoughTransform.cpp \
    $$PWD/chtbudextractor.cpp \
    $$PWD/fastradialsymmetrytransform.cpp \
    $$PWD/frstbudextractor.cpp

HEADERS += \
    $$PWD/budextractor.h \
    $$PWD/cannyoperator.h \
    $$PWD/circledrawer.h \
    $$PWD/circle.h \
    $$PWD/CircularHoughTransform.h \
    $$PWD/chtbudextractor.h \
    $$PWD/fastradialsymmetrytransform.h \
    $$PWD/frstbudextractor.h

