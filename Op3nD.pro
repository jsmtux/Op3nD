######################################################################
# Automatically generated by qmake (2.01a) S�b Set 28 17:01:15 2013
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += . src src/ProjectManagement src/view
INCLUDEPATH += . src/ProjectManagement

QMAKE_CXXFLAGS+=-std=gnu++11

# Input
HEADERS += src/ProjectManagement/FileSys.h \
           src/ProjectManagement/MXML.h \
           src/ProjectManagement/Project.h
FORMS += src/view/mainWindow.ui src/view/newProjectDialog.ui
SOURCES += src/main.cpp \
           src/ProjectManagement/FileSys.cpp \
           src/ProjectManagement/MXML.cpp \
           src/ProjectManagement/Project.cpp
