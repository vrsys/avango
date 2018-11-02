ADD_LIBRARY( _daemon SHARED "_daemon.cpp" )

set_target_properties( _daemon
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/daemon
)

TARGET_LINK_LIBRARIES( _daemon avango_daemon)

set_target_properties( _daemon PROPERTIES COMPILE_FLAGS "-D BOOST_PYTHON_STATIC_LIB")
SET_TARGET_PROPERTIES( _daemon PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_daemon ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/daemon/_daemon.pyd)
ELSEIF (UNIX)
  post_build_install_target(_daemon ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/daemon/_daemon.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/daemon
    _daemon
)
