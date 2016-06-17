###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_AWESOMIUM_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/gua/gui/GuiResourceNode.cpp
    include/avango/gua/gui/GuiResourceNode.h
)

###############################################################################
# set link directories and link dependencies
###############################################################################
ADD_LIBRARY(avango_awesomium SHARED
  ${AVANGO_AWESOMIUM_SRC}
)

target_include_directories(avango_awesomium PUBLIC
  include
)

###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_AWESOMIUM_LIBRARY")
  set_target_properties(avango_awesomium PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

TARGET_LINK_LIBRARIES(avango_awesomium 
                      avango_gua
                      avango_script
)

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
