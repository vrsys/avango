LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${GUACAMOLE_SKELANIM_LIBRARY_DIRS}
)

file(GLOB_RECURSE AVANGO_SKELANIM_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
    *.hpp
)

include_directories(
  ${PYTHON3_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIR}
)

ADD_LIBRARY( _skelanim SHARED 
    ${AVANGO_SKELANIM_PYTHON_SRC}
)

set_target_properties( _skelanim
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/gua/skelanim
)

ADD_DEPENDENCIES ( _skelanim avango_skelanim )

TARGET_LINK_LIBRARIES( _skelanim
  debug ${LIBRARIES} avango_skelanim
  optimized ${LIBRARIES} avango_skelanim)
SET_TARGET_PROPERTIES( _skelanim PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_skelanim
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/skelanim/_skelanim.pyd)
ELSEIF (UNIX)
  post_build_install_target(_skelanim
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/skelanim/_skelanim.so)
ENDIF (MSVC)

###############################################################################
# install utilities
###############################################################################
COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/skelanim
    _skelanim
)
