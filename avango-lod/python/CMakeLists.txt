LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${GUACAMOLE_LOD_LIBRARY_DIRS}
)

file(GLOB_RECURSE AVANGO_LOD_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
    *.hpp
)

include_directories(
  ${PYTHON3_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIR}
)

ADD_LIBRARY( _lod SHARED 
  ${AVANGO_LOD_PYTHON_SRC}
)

set_target_properties( _lod
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/gua/lod
)

ADD_DEPENDENCIES ( _lod avango_lod )

TARGET_LINK_LIBRARIES( _lod
  debug ${LIBRARIES} avango_lod
  optimized ${LIBRARIES} avango_lod)
SET_TARGET_PROPERTIES( _lod PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_lod
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/lod/_lod.pyd)
ELSEIF (UNIX)
  post_build_install_target(_lod
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/lod/_lod.so)
ENDIF (MSVC)

###############################################################################
# install utilities
###############################################################################
COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/lod
    _lod
)
