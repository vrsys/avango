LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${GUACAMOLE_NURBS_LIBRARY_DIRS}
)

file(GLOB_RECURSE AVANGO_NURBS_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
    *.hpp
)

include_directories(
  ${PYTHON3_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_LIBRARY( _nurbs SHARED 
  ${AVANGO_NURBS_PYTHON_SRC}
)

set_target_properties( _nurbs
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/gua/nurbs
)

ADD_DEPENDENCIES ( _nurbs avango_nurbs )

TARGET_LINK_LIBRARIES( _nurbs ${LIBRARIES} avango_nurbs)
SET_TARGET_PROPERTIES( _nurbs PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_nurbs
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/nurbs/_nurbs.pyd)
ELSEIF (UNIX)
  post_build_install_target(_nurbs
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/nurbs/_nurbs.so)
ENDIF (MSVC)

###############################################################################
# install utilities
###############################################################################
COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/nurbs
    _nurbs
)
