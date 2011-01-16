add_definitions(-Wall -pedantic)

include_directories(${KOMPAS_CORE_INCLUDE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})

if(QT_PLUGINS)
    find_package(Qt4 REQUIRED)
    find_package(KompasQt REQUIRED)

    include_directories(${KOMPAS_QT_INCLUDE_DIR})

    add_subdirectory(OpenGLMapView)
endif()

add_subdirectory(FreemapSkRasterModel)
add_subdirectory(HikeBikeMapRasterModel)
add_subdirectory(StereographicProjection)
add_subdirectory(WorldMap1630RasterModel)
add_subdirectory(WorldMap1689RasterModel)

if(BUILD_TESTS)
    enable_testing()
endif()
