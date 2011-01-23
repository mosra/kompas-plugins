add_definitions(-Wall -pedantic)

include_directories(${KOMPAS_CORE_INCLUDE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})

add_subdirectory(FreemapSkRasterModel)
add_subdirectory(HikeBikeMapRasterModel)
add_subdirectory(MapQuestOpenRasterModel)
add_subdirectory(StereographicProjection)
add_subdirectory(WorldMap1630RasterModel)
add_subdirectory(WorldMap1689RasterModel)

if(BUILD_TESTS)
    enable_testing()
endif()
