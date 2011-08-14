# Uses unordered_map from C++Ox
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

kompas_add_plugin(FilesystemCache ${KOMPAS_PLUGINS_CACHE_INSTALL_DIR} FilesystemCache.conf FilesystemCache.cpp)

if(WIN32)
    target_link_libraries(FilesystemCache ${KOMPAS_CORE_LIBRARIES})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_library(FilesystemCacheTestLib FilesystemCache.cpp)
    target_link_libraries(FilesystemCacheTestLib ${KOMPAS_CORE_LIBRARIES})
    add_subdirectory(Test)
endif()
