LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${GUACAMOLE_TV_3_LIBRARY_DIRS}
)

file(GLOB_RECURSE AVANGO_TV_3_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
    *.hpp
)

include_directories(
  ${PYTHON3_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIR}
)

ADD_LIBRARY( _tv_3 SHARED 
  ${AVANGO_TV_3_PYTHON_SRC}
)

set_target_properties( _tv_3
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/gua/tv_3
)

ADD_DEPENDENCIES ( _tv_3 avango_tv_3 )

TARGET_LINK_LIBRARIES( _tv_3
  debug ${LIBRARIES} avango_tv_3
  optimized ${LIBRARIES} avango_tv_3)
SET_TARGET_PROPERTIES( _tv_3 PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_tv_3
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/tv_3/_tv_3.pyd)
ELSEIF (UNIX)
  post_build_install_target(_tv_3
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/tv_3/_tv_3.so)
ENDIF (MSVC)

###############################################################################
# install utilities
###############################################################################
COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/tv_3
    _tv_3
)
