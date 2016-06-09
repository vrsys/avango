###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_VOLUME_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/gua/volume/VolumeLoader.cpp
    src/avango/gua/volume/VolumeNode.cpp
    src/avango/gua/volume/VolumePassDescription.cpp
    src/avango/gua/volume/Init.cpp
    include/avango/gua/volume/VolumeLoader.hpp
    include/avango/gua/volume/VolumeNode.hpp
    include/avango/gua/volume/VolumePassDescription.hpp
    include/avango/gua/volume/Init.hpp
)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
  ${LIB_PATHS}
  ${GUACAMOLE_VOLUME_LIBRARY_DIRS}
  ${PYTHON3_LIBRARY_DIRS}
  ${BULLET_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${PYTHON3_INCLUDE_DIRS}
    ${GUACAMOLE_VOLUME_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-gua/include
    ../avango-python/include
    ../../guacamole-volume/include
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY( avango_volume SHARED
    ${AVANGO_VOLUME_SRC}
)

ADD_DEPENDENCIES ( avango_volume avango )

###############################################################################
# set preprocessor configuration
###############################################################################
SET(AVANGO_VOLUME_BUILD_FLAGS "${BUILD_FLAGS} -D AV_VOLUME_LIBRARY")

set_target_properties(avango_volume PROPERTIES COMPILE_FLAGS ${AVANGO_VOLUME_BUILD_FLAGS})

LIST(APPEND LIBRARIES avango avango_script avango_gua ${GUACAMOLE_VOLUME_LIBRARIES})

TARGET_LINK_LIBRARIES( avango_volume
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
  post_build_install_target(avango_volume ${AVANGO_INSTALL_DIRECTORY}/avango/avango_volume.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_volume ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_volume.so)
ENDIF (MSVC)
