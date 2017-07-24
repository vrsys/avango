
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${BULLET_LIBRARY_DIRS}
)

include_directories(${PYTHON_INCLUDE_DIR})

ADD_LIBRARY( _sound SHARED "_sound.cpp" )

set_target_properties( _sound
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/sound
)

ADD_DEPENDENCIES ( _sound avango_sound )

TARGET_LINK_LIBRARIES( _sound 
                       avango_sound)

set_target_properties(_sound PROPERTIES COMPILE_FLAGS "-D BOOST_PYTHON_STATIC_LIB")
SET_TARGET_PROPERTIES( _sound PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_sound ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/sound/_sound.pyd)
ELSEIF (UNIX)
  post_build_install_target(_sound ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/sound/_sound.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/sound
    _sound
)
