add_definitions(-Wall -pedantic)

find_package(Map2XCore REQUIRED)
include_directories(${MAP2X_CORE_INCLUDE_DIR} ${MAP2X_PLUGINS_INCLUDE_DIR})

if(QT_PLUGINS)
    find_package(Qt4 REQUIRED)
endif()
