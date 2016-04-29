LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${GUACAMOLE_VOLUME_LIBRARY_DIRS}
)

include_directories(
  ${PYTHON3_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIR}
)

ADD_LIBRARY( _volume SHARED
    "volume/VolumeLoader.cpp"
    "volume/VolumeLoader.hpp"
    "volume/VolumeNode.cpp"
    "volume/VolumeNode.hpp"
    "volume/VolumePassDescription.cpp"
    "volume/VolumePassDescription.hpp"
    "volume/_volume.cpp"
)


set_target_properties( _volume
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/gua/volume
)

ADD_DEPENDENCIES ( _volume avango_volume )

LIST(APPEND LIBRARIES avango avango_gua avango_volume ${GUACAMOLE_VOLUME_LIBRARIES})
TARGET_LINK_LIBRARIES( _volume debug ${LIBRARIES} optimized ${LIBRARIES})
SET_TARGET_PROPERTIES( _volume PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_volume ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/volume/_volume.pyd)
ELSEIF (UNIX)
  post_build_install_target(_volume ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/volume/_volume.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}/volume
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/volume
    _volume
)
