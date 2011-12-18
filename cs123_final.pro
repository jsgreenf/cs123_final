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
    support/resourceloader.h \
    support/mainwindow.h \
    lib/targa.h \
    support/camera.h \
    shape/sphere.h \
    shape/shape.h \
    lib/CS123Common.h \
    math/CS123Vector.h \
    math/vector.h \
    shape/cone.h \
    shape/shape2.h
SOURCES += lab/glwidget.cpp \
    lib/targa.cpp \
    lib/glm.cpp \
    support/resourceloader.cpp \
    support/mainwindow.cpp \
    support/main.cpp \
    shape/sphere.cpp \
    support/camera.cpp \
    shape/shape.cpp \
    shape/cone.cpp \
    shape/shape2.cpp
FORMS += mainwindow.ui \
    support/mainwindow.ui
OTHER_FILES += shaders/reflect.vert \
    shaders/reflect.frag \
    shaders/terrain.frag \
    shaders/terrain.vars \
    shaders/terrain.vert \
    shaders/clouds.frag
RESOURCES += 
