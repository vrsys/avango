###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_UTILS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/Bool2And.cpp
    src/Bool2Or.cpp
    src/Bool3And.cpp
    src/Bool3Or.cpp
    src/Init.cpp
    src/MultiValueField.cpp
    # src/PCLPointCloud.cpp
    src/ProximitySensor.cpp
    src/SConscript
    src/Trackball.cpp
    include/avango/utils/Bool2And.h
    include/avango/utils/Bool2Or.h
    include/avango/utils/Bool3And.h
    include/avango/utils/Bool3Or.h
    include/avango/utils/Doxygen.h
    include/avango/utils/Init.h
    include/avango/utils/MultiValueField.h
    # include/avango/utils/PCLPointCloud.h
    include/avango/utils/ProximitySensor.h
    include/avango/utils/register_multivaluefield.h
    include/avango/utils/SConscript
    include/avango/utils/Trackball.h
    include/avango/utils/windows_specific_utils.h
)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${PYTHON_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
)

ADD_LIBRARY(avango_utils SHARED
  ${AVANGO_UTILS_SRC}
)

INCLUDE_DIRECTORIES(
    ${INCLUDE_PATHS}
    ${SCHISM_INCLUDE_DIRS}
    ${BULLET_INCLUDE_DIRS}
    include
    ../avango-core/include
    ../avango-python/include
    ${PYTHON_INCLUDE_DIRS}
    ../avango-gua/include
)

ADD_DEPENDENCIES ( avango_utils avango)

###############################################################################
# set preprocessor configuration
###############################################################################

SET (AVANGO_UTILS_BUILD_FLAGS "${BUILD_FLAGS} -D AV_UTILS_LIBRARY")

set_target_properties(avango_utils PROPERTIES COMPILE_FLAGS ${AVANGO_UTILS_BUILD_FLAGS})

TARGET_LINK_LIBRARIES( avango_utils
  debug avango_gua avango_script avango ${LIBRARIES}
  optimized avango_gua avango_script avango ${LIBRARIES})

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_utils
    ${AVANGO_INSTALL_DIRECTORY}/avango/avango_utils.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_utils
    ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_utils.so)
ENDIF (MSVC)
