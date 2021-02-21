QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11 \
    static

SOURCES += \
    PiCalcQT.cpp \
    button.cpp \
    main.cpp \
    tinyexpr.c

HEADERS += \
    PiCalcQT.h \
    button.h \
    tinyexpr.h

FORMS +=

TRANSLATIONS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
RC_ICONS = Logo.ico

RESOURCES += \
    resources.qrc
