TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp
INCLUDEPATH += /usr/local/include \
               /usr/local/include/opencv4 \
               /usr/local/include/opencv4/opencv2

LIBS += /usr/local/lib/libopencv_highgui.so \
        /usr/local/lib/libopencv_core.so    \
        /usr/local/lib/libopencv_imgproc.so \
        /usr/local/lib/libopencv_imgcodecs.so \
        /usr/local/lib/*.so
