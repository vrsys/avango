###############################################################################
# generate Config.h
###############################################################################
FILE(READ include/avango/sound/Config.h.in AVANGO_CONFIG_IN)

SET(AVANGO_CONFIG_OUT ${AVANGO_CONFIG_IN})

STRING(REGEX MATCHALL "%\\([^\\)]+\\)s" AVANGO_CONFIG_VARIABLES ${AVANGO_CONFIG_IN})

FOREACH(_CUR_VARIABLE ${AVANGO_CONFIG_VARIABLES})
    SET(_STRIPPED_VARIABLE "")
    STRING(REGEX REPLACE "%\\(" "" _STRIPPED_VARIABLE ${_CUR_VARIABLE})
    STRING(REGEX REPLACE "\\)s" "" _STRIPPED_VARIABLE ${_STRIPPED_VARIABLE})
    STRING(REPLACE ${_CUR_VARIABLE} _${_STRIPPED_VARIABLE} AVANGO_CONFIG_OUT ${AVANGO_CONFIG_OUT})
ENDFOREACH(_CUR_VARIABLE)

FILE(WRITE include/avango/sound/Config.h ${AVANGO_CONFIG_OUT})

###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_SOUND_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/SampleBuffer.cpp
    src/SoundRenderer.cpp
    src/SoundSource.cpp
    src/SoundTraverser.cpp
    include/avango/sound/Config.h
    include/avango/sound/Doxygen.h
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
    ${PYTHON_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
)

ADD_LIBRARY(avango_sound SHARED
  ${AVANGO_SOUND_SRC}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ${PYTHON_INCLUDE_DIRS}
    ../avango-gua/include
)

ADD_DEPENDENCIES ( avango_sound avango)

###############################################################################
# set preprocessor configuration
###############################################################################
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_SOUND_DEBUG=${AVANGO_SOUND_DEBUG}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_SOUND_VRPN_SUPPORT=${AVANGO_SOUND_VRPN_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DISTRIBUTION_SUPPORT=${AVANGO_DISTRIBUTION_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_ZMQ_DISTRIBUTION_SUPPORT=${AVANGO_ZMQ_DISTRIBUTION_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_PCL_SUPPORT=${AVANGO_PCL_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_LOG_LEVEL=${AVANGO_LOG_LEVEL}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_SOUND_VERSION_MAJOR=${AVANGO_VERSION_MAJOR}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_SOUND_VERSION_MINOR=${AVANGO_VERSION_MINOR}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_SOUND_VERSION_MAINT=${AVANGO_VERSION_MAINT}")

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_SOUND_LIBRARY")
ENDIF (MSVC)

set_target_properties(avango_sound PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})

TARGET_LINK_LIBRARIES( avango_sound
  debug avango_gua avango_script avango ${LIBRARIES}
  optimized avango_gua avango_script avango ${LIBRARIES})

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
