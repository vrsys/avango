###############################################################################
# determine source and header files
###############################################################################
file(GLOB_RECURSE AVANGO_GUA_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} 
	src/avango/gua/*.cpp 
	include/avango/gua/*.hpp
)


file(GLOB_RECURSE AVANGO_GUA_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	python/avango/gua/*.c  
	python/avango/gua/*.cpp 
	python/avango/gua/*.h 
	python/avango/gua/*.hpp
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
ADD_LIBRARY( avango_gua SHARED
    	     ${AVANGO_GUA_SRC}
)

ADD_LIBRARY( _gua SHARED
    	     ${AVANGO_GUA_PYTHON_SRC}
)

ADD_DEPENDENCIES(avango_gua avango)
ADD_DEPENDENCIES(_gua avango avango_gua)

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

MESSAGE(${BULLET_INCLUDE_DIRS})

###############################################################################
# set preprocessor configuration
###############################################################################
SET (BUILD_FLAGS 
	"-D _AVANGO_DAEMON_DEBUG=${AVANGO_DAEMON_DEBUG} -D _AVANGO_DAEMON_VRPN_SUPPORT=${AVANGO_DAEMON_VRPN_SUPPORT} -D _AVANGO_DISTRIBUTION_SUPPORT=${AVANGO_DISTRIBUTION_SUPPORT} -D _AVANGO_ZMQ_DISTRIBUTION_SUPPORT=${AVANGO_ZMQ_DISTRIBUTION_SUPPORT} -D _AVANGO_PCL_SUPPORT=${AVANGO_PCL_SUPPORT} -D _AVANGO_LOG_LEVEL=${AVANGO_LOG_LEVEL} -D _AVANGO_VERSION_MAJOR=${AVANGO_VERSION_MAJOR} -D _AVANGO_VERSION_MINOR=${AVANGO_VERSION_MINOR} -D _AVANGO_VERSION_MAINT=${AVANGO_VERSION_MAINT}"
)

IF (MSVC)
	SET( BUILD_FLAGS "${BUILD_FLAGS} -D AV_GUA_LIBRARY /bigobj ")
ENDIF (MSVC)

set_target_properties(avango_gua PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
set_target_properties(avango_gua PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

set_target_properties(_gua PROPERTIES COMPILE_FLAGS ${BUILD_FLAGS})
set_target_properties(_gua PROPERTIES COMPILE_DEFINITIONS_DEBUG "_AVANGO_DEBUG=1")

TARGET_LINK_LIBRARIES( avango_gua 
                       debug 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango guacamole boost_program_options boost_regex
                       optimized 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango guacamole boost_program_options boost_regex
)

TARGET_LINK_LIBRARIES( _gua
                       debug 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango avango_gua avangoScript guacamole boost_program_options boost_regex
                       optimized 
                       ${LIBRARIES} ${PYTHON_LIBRARIES} ${BOOST_LIBRARIES} avango avango_gua avangoScript guacamole boost_program_options boost_regex
)
SET_TARGET_PROPERTIES( _gua PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES( _gua PROPERTIES COMPILE_FLAGS -fPIC)

###############################################################################
# targets for unittesting
###############################################################################
IF (AVANGO_UNITTESTS)

ENDIF (AVANGO_UNITTESTS)
