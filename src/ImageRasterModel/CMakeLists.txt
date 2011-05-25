kompas_add_plugin(ImageRasterModel ${KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR} ImageRasterModel.conf ImageRasterModel.cpp)

if(WIN32)
    target_link_libraries(ImageRasterModel ${KOMPAS_CORE_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
endif()
