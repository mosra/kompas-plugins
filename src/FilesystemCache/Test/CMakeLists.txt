configure_file(${CMAKE_CURRENT_SOURCE_DIR}/FilesystemCacheTestConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/FilesystemCacheTestConfigure.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

kompas_add_test(FilesystemCacheTest
    FilesystemCacheTest.h FilesystemCacheTest.cpp KompasCore FilesystemCacheTestLib)
