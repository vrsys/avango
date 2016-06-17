LIST(APPEND INCLUDE_DIRECTORIES ..)

file(GLOB_RECURSE AVANGO_TOOLS_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    avango/tools/*.hpp
    avango/tools/*.cpp
)

ADD_LIBRARY( _tools SHARED
  ${AVANGO_TOOLS_PYTHON_SRC}
)

set_target_properties( _tools
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/tools
)

TARGET_LINK_LIBRARIES( _tools 
                       avango_tools
                     )

set_target_properties( _tools PROPERTIES COMPILE_FLAGS "-D BOOST_PYTHON_STATIC_LIB")
SET_TARGET_PROPERTIES( _tools PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_tools ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/tools/_tools.pyd)
ELSEIF (UNIX)
  post_build_install_target(_tools ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/tools/_tools.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/tools
    _tools
)
