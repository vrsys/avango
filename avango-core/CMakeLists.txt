# read replacement text from file
FILE(READ include/avango/Config.h.in AVANGO_CONFIG_IN)

###############################################################################
# write Config.h
###############################################################################
SET(AVANGO_CONFIG_OUT ${AVANGO_CONFIG_IN})

STRING(REGEX MATCHALL "%\\([^\\)]+\\)s" AVANGO_CONFIG_VARIABLES ${AVANGO_CONFIG_IN})

FOREACH(_CUR_VARIABLE ${AVANGO_CONFIG_VARIABLES})
	SET(_STRIPPED_VARIABLE "")
	STRING(REGEX REPLACE "%\\(" "" _STRIPPED_VARIABLE ${_CUR_VARIABLE})
	STRING(REGEX REPLACE "\\)s" "" _STRIPPED_VARIABLE ${_STRIPPED_VARIABLE})
	STRING(REPLACE ${_CUR_VARIABLE} _${_STRIPPED_VARIABLE} AVANGO_CONFIG_OUT ${AVANGO_CONFIG_OUT})		
ENDFOREACH(_CUR_VARIABLE)

STRING(REPLACE "av::logging::_AVANGO_LOG_LEVEL" "av::logging::${AVANGO_LOG_LEVEL}" AVANGO_CONFIG_OUT ${AVANGO_CONFIG_OUT})

FILE(WRITE include/avango/Config.h ${AVANGO_CONFIG_OUT})

###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_CORE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/*.cpp
  src/avango/actions/*.cpp
  src/avango/fields/*.cpp
  src/avango/interface/*.cpp
  src/avango/logging/*.cpp
  src/avango/nodes/*.cpp
  src/avango/streams/*.cpp
  src/avango/support/*.cpp
  src/avango/types/*.cpp
  include/avango/*.h
  include/avango/logging/*.h
)

###############################################################################
# optional sources for distribution support
###############################################################################
IF (${AVANGO_DISTRIBUTION_SUPPORT})
  file(GLOB AVANGO_CORE_DISTRIBUTION_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	src/avango/network/*.cpp
	src/avango/network/*.h
  )
  IF (NOT ${AVANGO_ZMQ_DISTRIBUTION_SUPPORT})
    list (REMOVE_ITEM  AVANGO_CORE_DISTRIBUTION_SRC
	src/avango/network/NetNodeClient.cpp
	src/avango/network/NetNodeClient.h
	src/avango/network/NetNodeServer.cpp
	src/avango/network/NetNodeServer.h
   )
  ENDIF (NOT ${AVANGO_ZMQ_DISTRIBUTION_SUPPORT})
ENDIF (${AVANGO_DISTRIBUTION_SUPPORT})

LINK_DIRECTORIES(${LIB_PATHS})

ADD_LIBRARY( avango_core SHARED
    	     ${AVANGO_CORE_SRC}
)

INCLUDE_DIRECTORIES( avango_core 
	${INCLUDE_PATHS} 
	include
	../avango-unittest/include
)

IF (NOT ${LIBRARIES} STREQUAL "")
    TARGET_LINK_LIBRARIES( avango_core debug ${LIBRARIES} optimized ${LIBRARIES})
ENDIF (NOT ${LIBRARIES} STREQUAL "")

###############################################################################
# targets for unittesting
###############################################################################
IF (AVANGO_UNITTESTS)
   # general tests
   ADD_EXECUTABLE(unittest_mock_field_container "src/avango/nodes/tests/MockFieldContainer.cpp")
   ADD_EXECUTABLE(unittest_abstract_node "src/avango/nodes/tests/TestAbstractNode.cpp")
   ADD_EXECUTABLE(unittest_evaluation "src/avango/nodes/tests/TestEvaluation.cpp")
   ADD_EXECUTABLE(unittest_evaluation_order "src/avango/nodes/tests/TestEvaluationOrder.cpp")
   ADD_EXECUTABLE(unittest_field_connection "src/avango/nodes/tests/TestFieldConnectionChangeInNotify.cpp")
   ADD_EXECUTABLE(unittest_nodes_field_container "src/avango/nodes/tests/TestFieldContainer.cpp")
   ADD_EXECUTABLE(unittest_nodes "src/avango/nodes/tests/TestNodes.cpp")
   ADD_EXECUTABLE(unittest_nodes_unref "src/avango/nodes/tests/TestUnref.cpp")
   
   #logging
   ADD_EXECUTABLE(unittest_logging "src/avango/logging/tests/TestLogging.cpp")
   
   #interface
   ADD_EXECUTABLE(unittest_application "src/avango/interface/tests/TestApplication.cpp")
   ADD_EXECUTABLE(unittest_time_sensor "src/avango/interface/tests/TestTimeSensor.cpp")

   # type tests
   ADD_EXECUTABLE(unittest_container_pool "src/avango/types/tests/TestContainerPool.cpp")
   ADD_EXECUTABLE(unittest_libtype "src/avango/types/tests/TestLibType.cpp")
   ADD_EXECUTABLE(unittest_type "src/avango/types/tests/TestType.cpp")
   
   # fields
   ADD_EXECUTABLE(unittest_fields "src/avango/fields/tests/TestFields.cpp")
   IF (AVANGO_DISTRIBUTION_SUPPORT)
       ADD_EXECUTABLE(unittest_clone_fields "src/avango/fields/tests/TestCloneFields.cpp")
       ADD_EXECUTABLE(unittest_field_const "src/avango/fields/tests/TestFieldConst.cpp")
       ADD_EXECUTABLE(unittest_field_multipush "src/avango/fields/tests/TestFieldMultiPush.cpp")
       ADD_EXECUTABLE(unittest_field_referencing "src/avango/fields/tests/TestFieldReferencing.cpp")
       ADD_EXECUTABLE(unittest_field_type_conversion "src/avango/fields/tests/TestFieldTypeConversion.cpp")
       ADD_EXECUTABLE(unittest_field_value_callbacks "src/avango/fields/tests/TestFieldValueCallbacks.cpp")
       ADD_EXECUTABLE(unittest_multifields "src/avango/fields/tests/TestMultiFields.cpp")
       # network
       ADD_EXECUTABLE(unittest_net_lock "src/avango/network/tests/TestNetLock.cpp")
       ADD_EXECUTABLE(unittest_net_node "src/avango/network/tests/TestNetNode.cpp")
       ADD_EXECUTABLE(unittest_network "src/avango/network/tests/TestNetwork.cpp")
   ENDIF (AVANGO_DISTRIBUTION_SUPPORT)
   
   TARGET_LINK_LIBRARIES( unittest_fields debug avango_core optimized avango_core)
ENDIF (AVANGO_UNITTESTS)