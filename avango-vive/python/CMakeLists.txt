LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${GUACAMOLE_VIVE_LIBRARY_DIRS}
)

ADD_LIBRARY( _vive SHARED
    ./avango/vive/ViveWindow.cpp
    ./avango/vive/ViveWindow.hpp
    ./avango/vive/_vive.cpp
)


set_target_properties( _vive
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/vive
)

ADD_DEPENDENCIES ( _vive avango_vive )

TARGET_LINK_LIBRARIES( _vive
                       avango
                       avango_gua
                       avango_vive
                       ${GUACAMOLE_VIVE_LIBRARIES}
                     )

SET_TARGET_PROPERTIES( _vive PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_vive
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/vive/_vive.pyd)
ELSEIF (UNIX)
  post_build_install_target(_vive
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/vive/_vive.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}/avango/vive
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/vive
    _vive
)
