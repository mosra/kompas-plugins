kompas_add_plugin(StereographicProjection ${KOMPAS_PLUGINS_PROJECTION_INSTALL_DIR} StereographicProjection.conf StereographicProjection.cpp)

if(WIN32)
    target_link_libraries(StereographicProjection ${KOMPAS_CORE_LIBRARIES})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_library(StereographicProjectionTestLib StereographicProjection.cpp)
    add_subdirectory(Test)
endif()
