set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

include_directories(${KOMPAS_CORE_INCLUDE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})

if(QT_PLUGINS OR BUILD_TESTS)
    # Don't display long long warnings for Qt
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
    include_directories(${QT_INCLUDE_DIR})
endif()

if(QT_PLUGINS)
    include_directories(${KOMPAS_QT_INCLUDE_DIR})

    add_subdirectory(ImageRasterModel)
endif()

add_subdirectory(FilesystemCache)
add_subdirectory(FreemapSkRasterModel)
add_subdirectory(HikeBikeMapRasterModel)
add_subdirectory(MapQuestOpenRasterModel)
add_subdirectory(MoonCelestialBody)
add_subdirectory(MarsCelestialBody)
add_subdirectory(StereographicProjection)
add_subdirectory(WorldMap1630RasterModel)
add_subdirectory(WorldMap1689RasterModel)

if(BUILD_TESTS)
    enable_testing()
endif()
