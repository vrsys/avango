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
ADD_LIBRARY(avango_skelanim SHARED
  ${AVANGO_SKELANIM_SRC}
)

target_include_directories( avango_skelanim PUBLIC
  include
)

set_target_properties(avango_skelanim PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_SKELANIM_LIBRARY")
  set_target_properties(avango_skelanim PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

TARGET_LINK_LIBRARIES( avango_skelanim
  avango_gua
  avango_script
  guacamole-skelanim
)

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
