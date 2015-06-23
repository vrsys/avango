###############################################################################
# determine source and header files
###############################################################################
file(GLOB_RECURSE AVANGO_SKELANIM_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/gua/skelanim/*.cpp
    include/avango/gua/skelanim/*.hpp
)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${PYTHON3_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${PYTHON3_INCLUDE_DIRS}
    ${GUACAMOLE_SKELANIM_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ../avango-gua/include
)

ADD_LIBRARY(avango_skelanim SHARED
  ${AVANGO_SKELANIM_SRC}
)

ADD_DEPENDENCIES ( avango_skelanim avango)
set_target_properties(avango_skelanim PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_SKELANIM_LIBRARY")
  set_target_properties(avango_skelanim PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

SET(_AVANGO_SKELANIM_LIBRARIES ${LIBRARIES} guacamole-skelanim)
LIST(APPEND _AVANGO_SKELANIM_LIBRARIES )

TARGET_LINK_LIBRARIES( avango_skelanim
  debug avango_gua avango_script avango ${_AVANGO_SKELANIM_LIBRARIES}
  optimized avango_gua avango_script avango ${_AVANGO_SKELANIM_LIBRARIES})

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_skelanim
    ${AVANGO_INSTALL_DIRECTORY}/avango/avango_skelanim.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_skelanim
    ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_skelanim.so)
ENDIF (MSVC)
