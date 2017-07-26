###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_VIVE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/vive/*.cpp
    include/avango/vive/*.hpp
)

# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/avango-vive/cmake/modules)

###############################################################################
# set link directories and link dependencies
###############################################################################
ADD_LIBRARY( avango_vive SHARED
  ${AVANGO_VIVE_SRC}
)

target_include_directories( avango_vive PUBLIC include)

target_include_directories(avango_vive SYSTEM PUBLIC
  ${OPENVR_INCLUDE_DIRS}
  ${GUACAMOLE_VIVE_INCLUDE_DIRS}
)

###############################################################################
# set preprocessor configuration
###############################################################################
SET(AVANGO_VIVE_BUILD_FLAGS "${BUILD_FLAGS} -D AV_VIVE_LIBRARY")

set_target_properties(avango_vive PROPERTIES COMPILE_FLAGS ${AVANGO_VIVE_BUILD_FLAGS})

TARGET_LINK_LIBRARIES( avango_vive
  avango_script
  avango_gua
  guacamole-vive
  ${OPENVR_LIBRARY}
)

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_vive
    ${AVANGO_INSTALL_DIRECTORY}/avango/avango_vive.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_vive
    ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_vive.so)
ENDIF (MSVC)
