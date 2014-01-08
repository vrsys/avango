
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${BULLET_LIBRARY_DIRS}
)

ADD_LIBRARY( _sound SHARED "_sound.cpp" )

set_target_properties( _sound
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/sound
)

ADD_DEPENDENCIES ( _sound avango_sound )

TARGET_LINK_LIBRARIES( _sound debug ${LIBRARIES} avango_sound optimized ${LIBRARIES} avango_sound)
SET_TARGET_PROPERTIES( _sound PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_sound ${AVANGO_INSTALL_DIRECTORY}/avango/sound/_sound.pyd)
ELSEIF (UNIX)
  post_build_install_target(_sound ${AVANGO_INSTALL_DIRECTORY}/avango/sound/_sound.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/avango/sound
    _sound
)
