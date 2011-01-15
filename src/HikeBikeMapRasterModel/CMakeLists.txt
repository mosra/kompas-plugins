kompas_add_plugin(HikeBikeMapRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} HikeBikeMapRasterModel.conf HikeBikeMapRasterModel.cpp)

if(WIN32)
    target_link_libraries(HikeBikeMapRasterModel ${KOMPAS_CORE_LIBRARY})
endif()
