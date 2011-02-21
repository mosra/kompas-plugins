kompas_add_plugin(MapQuestOpenRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} MapQuestOpenRasterModel.conf MapQuestOpenRasterModel.cpp)

if(WIN32)
    target_link_libraries(MapQuestOpenRasterModel ${KOMPAS_CORE_LIBRARIES})
endif()
