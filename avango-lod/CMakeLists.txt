###############################################################################
# determine source and header files
###############################################################################
file(GLOB_RECURSE AVANGO_LOD_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/gua/lod/scenegraph/MLodNode.cpp
  src/avango/gua/lod/scenegraph/PLodNode.cpp
  src/avango/gua/lod/renderer/LodLoader.cpp
  src/avango/gua/lod/renderer/Aux.cpp
  src/avango/gua/lod/renderer/AuxAtlas.cpp
  src/avango/gua/lod/renderer/AuxAtlasTile.cpp
  src/avango/gua/lod/renderer/AuxSparsePoint.cpp
  src/avango/gua/lod/renderer/AuxView.cpp
  src/avango/gua/lod/renderer/PLodPassDescription.cpp
  src/avango/gua/lod/renderer/MLodPassDescription.cpp
  include/avango/gua/lod/scenegraph/MLodNode.hpp
  include/avango/gua/lod/scenegraph/PLodNode.hpp
  include/avango/gua/lod/renderer/LodLoader.hpp
  include/avango/gua/lod/renderer/Aux.hpp
  include/avango/gua/lod/renderer/AuxAtlas.hpp
  include/avango/gua/lod/renderer/AuxAtlasTile.hpp
  include/avango/gua/lod/renderer/AuxSparsePoint.hpp
  include/avango/gua/lod/renderer/AuxView.hpp
  include/avango/gua/lod/renderer/PLodPassDescription.hpp
  include/avango/gua/lod/renderer/MLodPassDescription.hpp
)

###############################################################################
# set link directories and link dependencies
###############################################################################
ADD_LIBRARY(avango_lod SHARED
  ${AVANGO_LOD_SRC}
)

target_include_directories( avango_lod PUBLIC
  include
)

set_target_properties(avango_lod PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
# set preprocessor configuration
###############################################################################

IF (MSVC)
  SET(BUILD_FLAGS "${BUILD_FLAGS} -D AV_LOD_LIBRARY")
  set_target_properties(avango_lod PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
ENDIF (MSVC)

TARGET_LINK_LIBRARIES( avango_lod
  avango_gua
  avango_script
  guacamole-lod
)
###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_lod
    ${AVANGO_INSTALL_DIRECTORY}/avango/avango_lod.dll)
ELSEIF (UNIX)
  post_build_install_target(avango_lod
    ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_lod.so)
ENDIF (MSVC)
