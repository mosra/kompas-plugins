add_definitions(-Wall -pedantic)

include_directories(${KOMPAS_CORE_INCLUDE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})

if(QT_PLUGINS)
    find_package(Qt4 REQUIRED)
    find_package(KompasQt REQUIRED)
    find_package(Magnum)

    include_directories(${KOMPAS_QT_INCLUDE_DIR})

    if(NOT MAGNUM_FOUND)
        message(WARNING "Magnum was not found. OpenGL map view plugin will not be build.")
    else()
        add_subdirectory(OpenGLMapView)
    endif()
endif()

add_subdirectory(FreemapSkRasterModel)
add_subdirectory(HikeBikeMapRasterModel)
add_subdirectory(MapQuestOpenRasterModel)
add_subdirectory(StereographicProjection)
add_subdirectory(WorldMap1630RasterModel)
add_subdirectory(WorldMap1689RasterModel)

if(BUILD_TESTS)
    enable_testing()
endif()
