LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${GUACAMOLE_SKELANIM_LIBRARY_DIRS}
)

file(GLOB_RECURSE AVANGO_SKELANIM_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.cpp
    *.hpp
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
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/skelanim/_skelanim.pyd)
ELSEIF (UNIX)
  post_build_install_target(_skelanim
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/skelanim/_skelanim.so)
ENDIF (MSVC)

###############################################################################
# install utilities
###############################################################################
# list python scripts in directory
file(GLOB PYTHON_FILES ABSOLUTE ${CMAKE_CURRENT_SOURCE_DIR}
    *.py
)
# target to trigger copying, All -> built by default
add_custom_target(copy_skelanim_utils ALL)
# copy them over if modified
foreach(PythonFile ${PYTHON_FILES})
  add_custom_command(TARGET copy_skelanim_utils
                    # run cmake command module 
                    COMMAND ${CMAKE_COMMAND} -E
                    # with following command
                    copy_if_different ${PythonFile}
                                      ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/skelanim)
endforeach()