#-------------------------------------------------
#
# Project created by QtCreator 2021-08-01T12:30:38
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AsterView
TEMPLATE = app


SOURCES += main.cpp\
        CReg.cpp \
        covsrt.c \
        gaussj.c \
        hdffile.cpp \
        imwidget.cpp \
        lfit.c \
        mainwindow.cpp \
        myprofileplot.cpp \
        nrutil.c \
        qcustomplot.cpp \
        svbksb.c \
        svdcmp.c \
        svdfit.c \
        svdvar.c

HEADERS  += mainwindow.h \
    CReg.h \
    hdffile.h \
    imwidget.h \
    myprofileplot.h \
    nr.h \
    nrutil.h \
    qcustomplot.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

LIBS+= -L/Users/hg/workdir/hdf-4.2.5-mac-intel-x86_64/lib -L/usr/local/lib -lm -lmfhdf -ldf -ljpeg -lz -lsz
INCLUDEPATH+= /Users/hg/workdir/hdf-4.2.5-mac-intel-x86_64/include
