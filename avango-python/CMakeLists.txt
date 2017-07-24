###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_SCRIPT_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/script/Init.cpp
  src/avango/script/Script.cpp
  src/avango/script/Types.cpp
  src/avango/script/Update.cpp
  include/avango/python/script/Init.h
  include/avango/python/script/Script.h
  include/avango/python/script/Types.h
)

file(GLOB AVANGO_PYTHON_AVANGO_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/_avango.cpp
  src/avango/FieldContainer.cpp
  src/avango/Field.cpp
  src/avango/exceptions.cpp
  src/avango/InputStream.cpp
  src/avango/OutputStream.cpp
)

file(GLOB AVANGO_PYTHON_SCRIPT_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/script/_script.cpp
)

###############################################################################
# set link directories
###############################################################################
LINK_DIRECTORIES(${LIB_PATHS}
                 ${PYTHON3_LIBRARY_DIRS}
                 ${Boost_LIBRARY_DIRS}
)


###############################################################################
# create libraries
###############################################################################
ADD_LIBRARY( avango_script SHARED
    	     ${AVANGO_SCRIPT_SRC}
)

ADD_LIBRARY( _avango SHARED
    	     ${AVANGO_PYTHON_AVANGO_SRC}
)

ADD_LIBRARY( _script SHARED
    	     ${AVANGO_PYTHON_SCRIPT_SRC}
)

ADD_DEPENDENCIES(avango_script avango)
ADD_DEPENDENCIES(_avango avango avango_script)
ADD_DEPENDENCIES(_script avango _avango)

INCLUDE_DIRECTORIES(BEFORE
  include
  include/avango/python/
	${INCLUDE_PATHS}
  ${PYTHON3_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/avango-core/include
)

IF (AVANGO_DISTRIBUTION_SUPPORT)
  include_directories(${ZMQ_INCLUDE_DIRS})
ENDIF()

###############################################################################
# set preprocessor configuration
###############################################################################
#SET( AVANGO_SCRIPT_BUILD_FLAGS "${BUILD_FLAGS} -D AV_PYTHON_LIBRARY -D BOOST_PYTHON_STATIC_LIB")
SET( AVANGO_SCRIPT_BUILD_FLAGS "${BUILD_FLAGS} -D AV_PYTHON_LIBRARY")

set_target_properties(avango_script PROPERTIES COMPILE_FLAGS ${AVANGO_SCRIPT_BUILD_FLAGS})
set_target_properties(avango_script PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

set_target_properties(_avango PROPERTIES COMPILE_FLAGS ${AVANGO_SCRIPT_BUILD_FLAGS})
set_target_properties(_avango PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

set_target_properties(_script PROPERTIES COMPILE_FLAGS ${AVANGO_SCRIPT_BUILD_FLAGS})
set_target_properties(_script PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

TARGET_LINK_LIBRARIES( avango_script
                       ${LIBRARIES} 
                       ${PYTHON3_LIBRARIES} 
                       ${PYTHON_LIBRARY} 
                       optimized ${Boost_PYTHON_LIBRARY_RELEASE} debug ${Boost_PYTHON_LIBRARY_DEBUG}
                       optimized ${Boost_PYTHON3_LIBRARY_RELEASE} debug ${Boost_PYTHON3_LIBRARY_DEBUG}
                       avango
)

TARGET_LINK_LIBRARIES( _avango
                       ${LIBRARIES} 
                       ${PYTHON3_LIBRARIES} 
                       ${PYTHON_LIBRARY} 
                       avango 
                       avango_script
)

SET_TARGET_PROPERTIES( _avango PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES( _script
                       ${LIBRARIES} 
                       ${PYTHON3_LIBRARIES} 
                       ${PYTHON_LIBRARY} 
                       optimized ${Boost_PYTHON_LIBRARY_RELEASE} debug ${Boost_PYTHON_LIBRARY_DEBUG}
                       optimized ${Boost_PYTHON3_LIBRARY_RELEASE} debug ${Boost_PYTHON3_LIBRARY_DEBUG}
                       avango 
                       avango_script 
                       _avango
)
SET_TARGET_PROPERTIES( _script PROPERTIES PREFIX "")

###############################################################################
# targets for unittesting
###############################################################################
IF (AVANGO_UNITTESTS)

ENDIF (AVANGO_UNITTESTS)

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_script ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/script/_script.pyd)
  post_build_install_target(_avango ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/_avango.pyd)
  post_build_install_target(avango_script ${AVANGO_INSTALL_DIRECTORY}/avango/avango_script.dll)
ELSEIF (UNIX)
  post_build_install_target(_script ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/script/_script.so)
  post_build_install_target(_avango ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/_avango.so)
  post_build_install_target(avango_script ${AVANGO_INSTALL_DIRECTORY}/avango/libavango_script.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_SOURCE_DIR}/avango-python/src/avango
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango
    _avango
)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_SOURCE_DIR}/avango-python/src/avango/script
    ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/script
    _script
)


