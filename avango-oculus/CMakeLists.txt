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
ADD_LIBRARY( avango_oculus SHARED
  ${AVANGO_OCULUS_SRC}
)

target_include_directories(avango_oculus PUBLIC include)

target_include_directories(avango_oculus SYSTEM PUBLIC
  ${OVR_INCLUDE_DIRS}
  ${GUACAMOLE_OCULUS_INCLUDE_DIRS}
)

###############################################################################
# set preprocessor configuration
###############################################################################
SET(AVANGO_OCULUS_BUILD_FLAGS "${BUILD_FLAGS} -D AV_OCULUS_LIBRARY")

set_target_properties(avango_oculus PROPERTIES COMPILE_FLAGS ${AVANGO_OCULUS_BUILD_FLAGS})

TARGET_LINK_LIBRARIES( avango_oculus
  avango_script
  avango_gua
  guacamole-oculus
  ${OVR_LIBRARY}
)

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
