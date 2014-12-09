#-------------------------------------------------
#
# Project created by QtCreator 2014-12-09T00:44:06
#
#-------------------------------------------------

QT += widgets
qtHaveModule(printsupport): QT += printsupport

TARGET = lab2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/imageviewer
INSTALLS += target


wince*: {
   DEPLOYMENT_PLUGIN += qjpeg qgif
}
