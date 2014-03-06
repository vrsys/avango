###############################################################################
# determine source and header files
###############################################################################
file(GLOB_RECURSE AVANGO_GUA_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/gua/*.cpp
    include/avango/gua/*.hpp
)

IF (NOT ${AVANGO_DISTRIBUTION_SUPPORT})
list (REMOVE_ITEM  AVANGO_GUA_SRC
    src/avango/gua/network/Init.cpp
    src/avango/gua/network/NetTransform.cpp
    src/avango/gua/network/SharedContainerHolder.cpp
    include/avango/gua/network/FragmentGroup.h
    include/avango/gua/network/Init.h
    include/avango/gua/network/NetTransform.h
    include/avango/gua/network/SharedContainerHolder.h
)
ENDIF (NOT ${AVANGO_DISTRIBUTION_SUPPORT})

file(GLOB_RECURSE AVANGO_GUA_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    python/avango/gua/*.c
    python/avango/gua/*.cpp
    python/avango/gua/*.h
    python/avango/gua/*.hpp
)

IF (NOT ${AVANGO_DISTRIBUTION_SUPPORT})
list (REMOVE_ITEM  AVANGO_GUA_PYTHON_SRC
    python/avango/gua/network/NetTransform.h
    python/avango/gua/network/NetTransform.cpp
)
ENDIF (NOT ${AVANGO_DISTRIBUTION_SUPPORT})

###############################################################################
# set link directories
###############################################################################
LINK_DIRECTORIES(${LIB_PATHS}
                 ${PYTHON_LIBRARY_DIRS}
                 ${BOOST_LIBRARY_DIRS}
                 ${SCHISM_LIBRARY_DIRS}
                 ${BULLET_LIBRARY_DIRS}
)


###############################################################################
# create libraries
###############################################################################
INCLUDE_DIRECTORIES(BEFORE
  include
  include/avango/python/
  ${INCLUDE_PATHS}
  ${PYTHON_INCLUDE_DIRS}
  ${SCHISM_INCLUDE_DIRS}
  ${BULLET_INCLUDE_DIRS}
  ${JSON_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/avango-core/include
  ${CMAKE_SOURCE_DIR}/avango-python/include
)

ADD_LIBRARY( avango_gua SHARED
    	     ${AVANGO_GUA_SRC}
)

ADD_LIBRARY( _gua SHARED
    	     ${AVANGO_GUA_PYTHON_SRC}
)

set_target_properties( _gua
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/gua
)

ADD_DEPENDENCIES(avango_gua avango)
ADD_DEPENDENCIES(_gua avango avango_gua)

###############################################################################
# set preprocessor configuration
###############################################################################
SET( AVANGO_GUA_BUILD_FLAGS "${BUILD_FLAGS} -D AV_GUA_LIBRARY")
SET( AVANGO_GUA_PYTHON_BUILD_FLAGS "${BUILD_FLAGS} ")

IF (MSVC)
	SET( AVANGO_GUA_BUILD_FLAGS "${AVANGO_GUA_BUILD_FLAGS} /bigobj")
ENDIF (MSVC)

set_target_properties(avango_gua PROPERTIES COMPILE_FLAGS "${AVANGO_GUA_BUILD_FLAGS}")
set_target_properties(avango_gua PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

set_target_properties(_gua PROPERTIES COMPILE_FLAGS "${AVANGO_GUA_PYTHON_BUILD_FLAGS}")
set_target_properties(_gua PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

###############################################################################
# set dependencies
###############################################################################
SET(_AVANGO_GUA_LIBRARIES ${LIBRARIES}
                          ${PYTHON_LIBRARIES}
                          ${BOOST_LIBRARIES}
                          ${BULLET_LIBRARIES}
                          ${JSON_LIBRARIES}
                          ${ASSIMP_LIBRARIES}
                          ${SCHISM_LIBRARIES}
                          avango
                          avango_gua
)

IF (UNIX)
	LIST(APPEND _AVANGO_GUA_LIBRARIES boost_program_options boost_regex)
ENDIF (UNIX)

TARGET_LINK_LIBRARIES( avango_gua
                       debug
                       ${_AVANGO_GUA_LIBRARIES}
                       optimized
                       ${_AVANGO_GUA_LIBRARIES}
)

TARGET_LINK_LIBRARIES( _gua
                       debug
                       ${_AVANGO_GUA_LIBRARIES}
                       optimized
                       ${_AVANGO_GUA_LIBRARIES}
)

SET_TARGET_PROPERTIES( _gua PROPERTIES PREFIX "")

IF (UNIX)
	SET_TARGET_PROPERTIES( _gua PROPERTIES COMPILE_FLAGS -fPIC)
ENDIF (UNIX)

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(avango_gua ${AVANGO_INSTALL_DIRECTORY}/avango/avango_gua.dll)
  post_build_install_target(_gua ${AVANGO_INSTALL_DIRECTORY}/python2.7/avango/gua/_gua.pyd)
ELSEIF (UNIX)
  post_build_install_target(avango_gua ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_gua.so)
  post_build_install_target(_gua ${AVANGO_INSTALL_DIRECTORY}/python2.7/avango/gua/_gua.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_SOURCE_DIR}/avango-gua/lib/avango/gua
    ${AVANGO_INSTALL_DIRECTORY}/python2.7/avango/gua
    _gua
)

###############################################################################
# targets for unittesting
###############################################################################
IF (AVANGO_UNITTESTS)

ENDIF (AVANGO_UNITTESTS)
