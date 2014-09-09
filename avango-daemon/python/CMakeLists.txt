
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${BULLET_LIBRARY_DIRS}
)

ADD_LIBRARY( _daemon SHARED "_daemon.cpp" )

set_target_properties( _daemon
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/daemon
)

ADD_DEPENDENCIES ( _daemon avango_daemon )

TARGET_LINK_LIBRARIES( _daemon debug ${LIBRARIES} avango_daemon optimized ${LIBRARIES} avango_daemon)
SET_TARGET_PROPERTIES( _daemon PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_daemon ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/daemon/_daemon.pyd)
ELSEIF (UNIX)
  post_build_install_target(_daemon ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/daemon/_daemon.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/daemon
    _daemon
)
