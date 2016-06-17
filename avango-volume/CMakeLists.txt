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
ADD_LIBRARY( avango_volume SHARED
    ${AVANGO_VOLUME_SRC}
)

target_include_directories(avango_volume PUBLIC include)
###############################################################################
# set preprocessor configuration
###############################################################################
SET(AVANGO_VOLUME_BUILD_FLAGS "${BUILD_FLAGS} -D AV_VOLUME_LIBRARY")

set_target_properties(avango_volume PROPERTIES COMPILE_FLAGS ${AVANGO_VOLUME_BUILD_FLAGS})

TARGET_LINK_LIBRARIES( avango_volume
  avango_gua
  avango_script
)

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
