###############################################################################
# determine source and header files
###############################################################################
file(GLOB_RECURSE AVANGO_TV_3_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/gua/tv_3/scenegraph/TV_3Node.cpp
  src/avango/gua/tv_3/renderer/TV_3Loader.cpp
  src/avango/gua/tv_3/renderer/TV_3SurfacePassDescription.cpp
  #src/avango/gua/tv_3/renderer/TV_3VolumeDescription.cpp
  include/avango/gua/tv_3/scenegraph/TV_3Node.hpp
  include/avango/gua/tv_3/renderer/TV_3Loader.hpp
  include/avango/gua/tv_3/renderer/TV_3SurfacePassDescription.hpp
  #include/avango/gua/tv_3/renderer/TV_3VolumePassDescription.hpp
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
    ${GUACAMOLE_TV_3_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ../avango-gua/include
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY(avango_tv_3 SHARED
  ${AVANGO_TV_3_SRC}
)

ADD_DEPENDENCIES ( avango_tv_3 avango)
set_target_properties(avango_tv_3 PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_TV_3_LIBRARY")
  set_target_properties(avango_tv_3 PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

SET(_AVANGO_TV_3_LIBRARIES ${LIBRARIES} guacamole-tv_3)
LIST(APPEND _AVANGO_TV_3_LIBRARIES )

TARGET_LINK_LIBRARIES( avango_tv_3
  debug avango_gua avango_script avango ${_AVANGO_TV_3_LIBRARIES}
  optimized avango_gua avango_script avango ${_AVANGO_TV_3_LIBRARIES})

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_tv_3
    ${AVANGO_INSTALL_DIRECTORY}/avango/avango_tv_3.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_tv_3
    ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_tv_3.so)
ENDIF (MSVC)
