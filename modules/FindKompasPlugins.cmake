# FindKompasPlugins - Kompas plugins handling module for CMake
#
# This module depends on Kompas Core and additionaly defines:
#
# KOMPASPLUGINS_FOUND       - True if Kompas plugins were found
#

find_package(KompasCore REQUIRED)

if (NOT KOMPASPLUGINS_FOUND)
    # Paths
    find_path(_KOMPAS_PLUGINS_INCLUDE_DIR
        NAMES StereographicProjection
        PATH_SUFFIXES Kompas/Plugins
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args("KompasPlugins" DEFAULT_MSG
        _KOMPAS_PLUGINS_INCLUDE_DIR
    )

endif()
