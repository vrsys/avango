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
                 ${PYTHON_LIBRARY_DIRS} 
                 ${BOOST_LIBRARY_DIRS}
)


###############################################################################
# create libraries
###############################################################################
ADD_LIBRARY( avango_python SHARED
    	     ${AVANGO_SCRIPT_SRC}
)

ADD_LIBRARY( _avango SHARED
    	     ${AVANGO_PYTHON_AVANGO_SRC}
)

ADD_LIBRARY( _script SHARED
    	     ${AVANGO_PYTHON_SCRIPT_SRC}
)

ADD_DEPENDENCIES(avango_python avango_core)
ADD_DEPENDENCIES(_avango avango_core avango_python)
ADD_DEPENDENCIES(_script avango_core _avango)

INCLUDE_DIRECTORIES(BEFORE 
  include
  include/avango/python/
	${INCLUDE_PATHS} 
  ${PYTHON_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/avango-core/include
)

###############################################################################
# set preprocessor configuration
###############################################################################
SET (BUILD_FLAGS 
	"-D _AVANGO_DAEMON_DEBUG=${AVANGO_DAEMON_DEBUG} -D _AVANGO_DAEMON_VRPN_SUPPORT=${AVANGO_DAEMON_VRPN_SUPPORT} -D _AVANGO_DISTRIBUTION_SUPPORT=${AVANGO_DISTRIBUTION_SUPPORT} -D _AVANGO_ZMQ_DISTRIBUTION_SUPPORT=${AVANGO_ZMQ_DISTRIBUTION_SUPPORT} -D _AVANGO_PCL_SUPPORT=${AVANGO_PCL_SUPPORT} -D _AVANGO_LOG_LEVEL=${AVANGO_LOG_LEVEL} -D _AVANGO_VERSION_MAJOR=${AVANGO_VERSION_MAJOR} -D _AVANGO_VERSION_MINOR=${AVANGO_VERSION_MINOR} -D _AVANGO_VERSION_MAINT=${AVANGO_VERSION_MAINT}"
)

IF (MSVC)
	SET( BUILD_FLAGS "${BUILD_FLAGS} -D AV_PYTHON_LIBRARY")
ENDIF (MSVC)

set_target_properties(avango_python PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
set_target_properties(avango_python PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

set_target_properties(_avango PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
set_target_properties(_avango PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

set_target_properties(_script PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
set_target_properties(_script PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

TARGET_LINK_LIBRARIES( avango_python 
                       debug 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango_core
                       optimized 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango_core
)

TARGET_LINK_LIBRARIES( _avango 
                       debug 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango_core avango_python
                       optimized 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango_core avango_python
)
SET_TARGET_PROPERTIES( _avango PROPERTIES PREFIX "")

TARGET_LINK_LIBRARIES( _script 
                       debug 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango_core avango_python _avango
                       optimized 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango_core avango_python _avango
)
SET_TARGET_PROPERTIES( _script PROPERTIES PREFIX "")

###############################################################################
# targets for unittesting
###############################################################################
IF (AVANGO_UNITTESTS)

ENDIF (AVANGO_UNITTESTS)
