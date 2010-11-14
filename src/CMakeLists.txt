add_definitions(-Wall -pedantic)

find_package(Map2XCore REQUIRED)

set(MAP2X_PLUGINS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} ${MAP2X_PLUGINS_INCLUDE_DIR})
include_directories(${MAP2X_CORE_INCLUDE_DIR} ${MAP2X_PLUGINS_INCLUDE_DIR})

if(QT_PLUGINS)
    find_package(Qt4 REQUIRED)
endif()

add_subdirectory(StereographicProjection)
add_subdirectory(WorldMap1689RasterModel)

if(BUILD_TESTS)
    enable_testing()
endif()
