###############################################################################
# generate Config.h
###############################################################################
FILE(READ include/avango/tools/Config.h.in AVANGO_CONFIG_IN)

SET(AVANGO_CONFIG_OUT ${AVANGO_CONFIG_IN})

STRING(REGEX MATCHALL "%\\([^\\)]+\\)s" AVANGO_CONFIG_VARIABLES ${AVANGO_CONFIG_IN})

FOREACH(_CUR_VARIABLE ${AVANGO_CONFIG_VARIABLES})
    SET(_STRIPPED_VARIABLE "")
    STRING(REGEX REPLACE "%\\(" "" _STRIPPED_VARIABLE ${_CUR_VARIABLE})
    STRING(REGEX REPLACE "\\)s" "" _STRIPPED_VARIABLE ${_STRIPPED_VARIABLE})
    STRING(REPLACE ${_CUR_VARIABLE} _${_STRIPPED_VARIABLE} AVANGO_CONFIG_OUT ${AVANGO_CONFIG_OUT})
ENDFOREACH(_CUR_VARIABLE)

FILE(WRITE include/avango/tools/Config.h ${AVANGO_CONFIG_OUT})

###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_TOOLS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/tools/ChangeSelector.cpp
    src/avango/tools/DragTool.cpp
    src/avango/tools/FieldSelector.cpp
    src/avango/tools/Init.cpp
    src/avango/tools/InjectSelector.cpp
    src/avango/tools/NameSelector.cpp
    src/avango/tools/ObjectSelector.cpp
    src/avango/tools/PickSelector.cpp
    src/avango/tools/ProximitySelector.cpp
    src/avango/tools/ScaledGrabTool.cpp
    src/avango/tools/Selector.cpp
    src/avango/tools/SingleSelector.cpp
    src/avango/tools/TargetHolder.cpp
    src/avango/tools/Tool.cpp
    src/avango/tools/TriggerSelector.cpp
    src/avango/tools/TypeSelector.cpp
    src/avango/tools/UnionSelector.cpp
    include/avango/tools/ChangeSelector.hpp
    include/avango/tools/Config.h
    include/avango/tools/DragTool.hpp
    include/avango/tools/FieldSelector.hpp
    include/avango/tools/Init.hpp
    include/avango/tools/InjectSelector.hpp
    include/avango/tools/NameSelector.hpp
    include/avango/tools/ObjectSelector.hpp
    include/avango/tools/PickSelector.hpp
    include/avango/tools/ProximitySelector.hpp
    include/avango/tools/ScaledGrabTool.hpp
    include/avango/tools/Selector.hpp
    include/avango/tools/SingleSelector.hpp
    include/avango/tools/TargetHolder.hpp
    include/avango/tools/Tool.hpp
    include/avango/tools/TriggerSelector.hpp
    include/avango/tools/TypeSelector.hpp
    include/avango/tools/UnionSelector.hpp
)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${PYTHON_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
)

ADD_LIBRARY(avango_tools SHARED
  ${AVANGO_TOOLS_SRC}
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

ADD_DEPENDENCIES ( avango_tools avango)

###############################################################################
# set preprocessor configuration
###############################################################################
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_TOOLS_DEBUG=${AVANGO_TOOLS_DEBUG}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_TOOLS_VRPN_SUPPORT=${AVANGO_TOOLS_VRPN_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_DISTRIBUTION_SUPPORT=${AVANGO_DISTRIBUTION_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_ZMQ_DISTRIBUTION_SUPPORT=${AVANGO_ZMQ_DISTRIBUTION_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_PCL_SUPPORT=${AVANGO_PCL_SUPPORT}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_LOG_LEVEL=${AVANGO_LOG_LEVEL}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_TOOLS_VERSION_MAJOR=${AVANGO_VERSION_MAJOR}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_TOOLS_VERSION_MINOR=${AVANGO_VERSION_MINOR}")
SET (BUILD_FLAGS "${BUILD_FLAGS} -D _AVANGO_TOOLS_VERSION_MAINT=${AVANGO_VERSION_MAINT}")

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_TOOLS_LIBRARY")
ENDIF (MSVC)

set_target_properties(avango_tools PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})

TARGET_LINK_LIBRARIES( avango_tools
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
  post_build_install_target(avango_tools ${AVANGO_INSTALL_DIRECTORY}/avango/avango_tools.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_tools ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_tools.so)
ENDIF (MSVC)
