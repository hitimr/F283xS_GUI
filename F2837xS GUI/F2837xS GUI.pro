# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

TEMPLATE = app
TARGET = F2837xS GUI
DESTDIR = ../Win32/Release
QT += core widgets gui charts
CONFIG += release
DEFINES += QT_DLL QT_WIDGETS_LIB QT_CHARTS_LIB
INCLUDEPATH += ./GeneratedFiles \
    . \
    ./GeneratedFiles/Release \
    ./third party/QRealFourier-master/code/fftreal
LIBS += -L"$(ProjectDir)/lib" \
    -lQRealFourier
DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(F2837xS GUI.pri)
