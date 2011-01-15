kompas_add_plugin(FreemapSkRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} FreemapSkRasterModel.conf FreemapSkRasterModel.cpp)

if(WIN32)
    target_link_libraries(FreemapSkRasterModel ${KOMPAS_CORE_LIBRARY})
endif()
