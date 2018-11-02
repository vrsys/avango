
ADD_LIBRARY( _openal SHARED "_openal.cpp" )

set_target_properties( _openal
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/sound/openal
)

TARGET_LINK_LIBRARIES( _openal avango_openal)
SET_TARGET_PROPERTIES( _openal PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_openal ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/sound/openal/_openal.pyd)
ELSEIF (UNIX)
  post_build_install_target(_openal ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/sound/openal/_openal.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/sound/openal
    _openal
)
