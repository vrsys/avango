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
LINK_DIRECTORIES(
  ${LIB_PATHS}
  ${GUACAMOLE_VIVE_LIBRARY_DIRS}
  ${PYTHON3_LIBRARY_DIRS}
  ${BULLET_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${PYTHON3_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIR}
    ${OPENVR_INCLUDE_DIR}
    ${GUACAMOLE_VIVE_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-gua/include
    ../avango-python/include
    ../../guacamole-vive/include
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY( avango_vive SHARED
  ${AVANGO_VIVE_SRC}
)

ADD_DEPENDENCIES ( avango_vive avango )

###############################################################################
# set preprocessor configuration
###############################################################################
SET(AVANGO_VIVE_BUILD_FLAGS "${BUILD_FLAGS} -D AV_VIVE_LIBRARY")

set_target_properties(avango_vive PROPERTIES COMPILE_FLAGS ${AVANGO_VIVE_BUILD_FLAGS})

LIST(APPEND LIBRARIES avango avango_script avango_gua ${OVR_LIBRARY}
  ${GUACAMOLE_VIVE_LIBRARIES})

TARGET_LINK_LIBRARIES( avango_vive ${LIBRARIES} )

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
