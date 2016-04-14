SET (AVANGO_TOOLS_VERSION_MAJOR "${AVANGO_VERSION_MAJOR}")
SET (AVANGO_TOOLS_VERSION_MINOR "${AVANGO_VERSION_MINOR}")
SET (AVANGO_TOOLS_VERSION_MAINT "${AVANGO_VERSION_MAINT}")
option (AVANGO_TOOLS_DEBUG "Enable avango-tools debugging")
SET (AVANGO_TOOLS_GUA_SUPPORT "true")

###############################################################################
# generate Config.h
###############################################################################

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/tools/Config.h.in
                ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/tools/Config.h )

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
    ${PYTHON3_LIBRARY_DIRS}
    ${BULLET_LIBRARY_DIRS}
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

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${RPC_INCLUDE_DIR} ${ZMQ_INCLUDE_DIRS})
ENDIF()

ADD_LIBRARY(avango_tools SHARED
  ${AVANGO_TOOLS_SRC}
)

ADD_DEPENDENCIES ( avango_tools avango)

###############################################################################
# set preprocessor configuration
###############################################################################

SET (AVANGO_TOOLS_BUILD_FLAGS "${BUILD_FLAGS} -D AV_TOOLS_LIBRARY")

set_target_properties(avango_tools PROPERTIES COMPILE_FLAGS ${AVANGO_TOOLS_BUILD_FLAGS})

TARGET_LINK_LIBRARIES( avango_tools 
                       avango_gua 
                       avango_script 
                       avango 
                       ${LIBRARIES}
                     )

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
