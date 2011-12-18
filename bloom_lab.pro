QT += core \
    gui \
    opengl
TARGET = bloom_lab
TEMPLATE = app
INCLUDEPATH += lab \
    lib \
    math \
    shape \
    support
DEPENDPATH += lab \
    lib \
    math \
    shape \
    support
HEADERS += lab/glwidget.h \
    lib/targa.h \
    lib/glm.h \
    math/vector.h \
    support/resourceloader.h \
    support/mainwindow.h \
    lib/targa.h \
    support/camera.h \
    shape/sphere.h \
    shape/shape.h \
    lib/CS123Common.h
SOURCES += lab/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    shape/sphere.cpp \
    support/camera.cpp \
    shape/shape.cpp
FORMS += mainwindow.ui \
    support/mainwindow.ui
OTHER_FILES += shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/terrain.frag \
    shaders/terrain.vars \
    shaders/terrain.vert \
    shaders/clouds.frag
RESOURCES += 
