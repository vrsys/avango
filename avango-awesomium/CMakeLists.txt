###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_AWESOMIUM_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/gua/gui/GuiResource.cpp
    include/avango/gua/gui/GuiResource.h
)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${PYTHON3_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
)

ADD_LIBRARY(avango_awesomium SHARED
  ${AVANGO_AWESOMIUM_SRC}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ${PYTHON3_INCLUDE_DIRS}
    ../avango-gua/include
)

ADD_DEPENDENCIES ( avango_awesomium avango)

###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_AWESOMIUM_LIBRARY")
  set_target_properties(avango_awesomium PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

SET(_AVANGO_AWESOMIUM_LIBRARIES ${LIBRARIES})
LIST(APPEND _AVANGO_AWESOMIUM_LIBRARIES )

TARGET_LINK_LIBRARIES( avango_awesomium
  debug avango_gua avango_script avango ${_AVANGO_AWESOMIUM_LIBRARIES}
  optimized avango_gua avango_script avango ${_AVANGO_AWESOMIUM_LIBRARIES})

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_awesomium
    ${AVANGO_INSTALL_DIRECTORY}/avango/avango_awesomium.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_awesomium
    ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_awesomium.so)
ENDIF (MSVC)
