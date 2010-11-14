kompas_add_plugin(WorldMap1630RasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} WorldMap1630RasterModel.conf WorldMap1630RasterModel.cpp)

if(WIN32)
    target_link_libraries(WorldMap1630RasterModel ${KOMPAS_CORE_LIBRARY} StereographicProjection)
endif()
