###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_SOUND_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/OpenALSoundRenderer.cpp
    include/avango/sound/openal/OpenALSoundRenderer.h
)

# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/avango-openal/cmake/modules)

include(find_openal)

###############################################################################
# set link directories and link dependencies
###############################################################################
ADD_LIBRARY(avango_openal SHARED
  ${AVANGO_SOUND_SRC}
)

target_include_directories( avango_openal PUBLIC
    include
    ../avango-sound/include
)
###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_SOUND_LIBRARY")
  set_target_properties(avango_openal PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

SET(_AVANGO_OPENAL_LIBRARIES openal alut vorbisfile)

TARGET_LINK_LIBRARIES( avango_openal 
                       avango_gua 
                       avango_script  
                      ${_AVANGO_OPENAL_LIBRARIES}
                     )
###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_openal ${AVANGO_INSTALL_DIRECTORY}/avango/avango_openal.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_openal ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_openal.so)
ENDIF (MSVC)
