add_definitions(-Wall -pedantic)

find_package(KompasCore REQUIRED)

set(KOMPAS_PLUGINS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})
include_directories(${KOMPAS_CORE_INCLUDE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})

if(QT_PLUGINS)
    find_package(Qt4 REQUIRED)
endif()

add_subdirectory(StereographicProjection)
add_subdirectory(WorldMap1689RasterModel)

if(BUILD_TESTS)
    enable_testing()
endif()
