###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_OCULUS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/oculus/*.cpp
    include/avango/oculus/*.hpp
)

# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/avango-oculus/cmake/modules)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
  ${LIB_PATHS}
  ${GUACAMOLE_OCULUS_LIBRARY_DIRS}
  ${PYTHON3_LIBRARY_DIRS}
  ${BULLET_LIBRARY_DIRS}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    ${PYTHON3_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIR}
    ${OVR_INCLUDE_DIR}
    ${GUACAMOLE_OCULUS_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-gua/include
    ../avango-python/include
    ../../guacamole-oculus/include
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${RPC_INCLUDE_DIR} ${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY( avango_oculus SHARED
    ${AVANGO_OCULUS_SRC}
)

ADD_DEPENDENCIES ( avango_oculus avango )

###############################################################################
# set preprocessor configuration
###############################################################################
SET(AVANGO_OCULUS_BUILD_FLAGS "${BUILD_FLAGS} -D AV_OCULUS_LIBRARY")

set_target_properties(avango_oculus PROPERTIES COMPILE_FLAGS ${AVANGO_OCULUS_BUILD_FLAGS})

LIST(APPEND LIBRARIES avango avango_script avango_gua ${OVR_LIBRARY} ${GUACAMOLE_OCULUS_LIBRARIES})

TARGET_LINK_LIBRARIES( avango_oculus ${LIBRARIES} )

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_oculus ${AVANGO_INSTALL_DIRECTORY}/avango/avango_oculus.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_oculus ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_oculus.so)
ENDIF (MSVC)
