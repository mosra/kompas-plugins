find_package(Magnum)

include_directories(${MAGNUM_INCLUDE_DIR})

set(OpenGLMapView_SRCS
    BaseTexture.cpp
    EarthShader.cpp
    Earth.cpp
    OpenGLMapView.cpp
    OpenGLMapViewPrivate.cpp
)
qt4_wrap_cpp(OpenGLMapView_MOC
    OpenGLMapView.h
)
kompas_add_resource(OpenGLMapView_RC OpenGLMapView EarthShader.vert EarthShader.frag)
kompas_add_plugin(OpenGLMapView ${KOMPAS_PLUGINS_MAPVIEW_INSTALL_DIR} OpenGLMapView.conf ${OpenGLMapView_SRCS} ${OpenGLMapView_MOC} ${OpenGLMapView_RC})

target_link_libraries(OpenGLMapView ${QT_QTOPENGL_LIBRARY} ${MAGNUM_LIBRARY})
