SET (AVANGO_SOUND_VERSION_MAJOR "${AVANGO_VERSION_MAJOR}")
SET (AVANGO_SOUND_VERSION_MINOR "${AVANGO_VERSION_MINOR}")
SET (AVANGO_SOUND_VERSION_MAINT "${AVANGO_VERSION_MAINT}")
option (AVANGO_SOUND_DEBUG "Enable avango-sound debugging")

###############################################################################
# generate Config.h
###############################################################################

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/sound/Config.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/sound/Config.h )

###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_SOUND_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/sound/SampleBuffer.cpp
    src/avango/sound/SoundRenderer.cpp
    src/avango/sound/SoundSource.cpp
    src/avango/sound/SoundTraverser.cpp
    src/avango/sound/Init.cpp
    include/avango/sound/Config.h
    include/avango/sound/Doxygen.h
    include/avango/sound/Platform.h
    include/avango/sound/SampleBuffer.h
    include/avango/sound/SoundRenderer.h
    include/avango/sound/SoundSource.h
    include/avango/sound/SoundTraverser.h
)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${PYTHON3_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ${PYTHON3_INCLUDE_DIRS}
    ../avango-gua/include
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY(avango_sound SHARED
  ${AVANGO_SOUND_SRC}
)

ADD_DEPENDENCIES ( avango_sound avango)

###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_SOUND_LIBRARY")
  set_target_properties(avango_sound PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

TARGET_LINK_LIBRARIES( avango_sound 
                       avango_gua 
                       avango_script 
                       avango 
                       ${LIBRARIES} 
                     )

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_sound ${AVANGO_INSTALL_DIRECTORY}/avango/avango_sound.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_sound ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_sound.so)
ENDIF (MSVC)
