map2x_add_plugin(StereographicProjection ${MAP2X_PLUGINS_PROJECTION_INSTALL_DIR} StereographicProjection.conf StereographicProjection.cpp)

if(BUILD_TESTS)
    enable_testing()
    add_library(StereographicProjectionTestLib StereographicProjection.cpp)
    add_subdirectory(Test)
endif()
