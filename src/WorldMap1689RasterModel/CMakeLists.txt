kompas_add_plugin(WorldMap1689RasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} WorldMap1689RasterModel.conf WorldMap1689RasterModel.cpp)

if(WIN32)
    target_link_libraries(WorldMap1689RasterModel ${KOMPAS_CORE_LIBRARIES} StereographicProjection)
endif()
