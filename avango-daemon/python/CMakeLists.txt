LINK_DIRECTORIES(${LIB_PATHS})

ADD_LIBRARY( _daemon SHARED "_daemon.cpp" )

ADD_DEPENDENCIES ( _daemon avango_daemon )

IF (NOT ${LIBRARIES} STREQUAL "")
    TARGET_LINK_LIBRARIES( _daemon debug ${LIBRARIES} avango_daemon optimized ${LIBRARIES} avango_daemon)
    SET_TARGET_PROPERTIES( _daemon PROPERTIES PREFIX "")
ENDIF (NOT ${LIBRARIES} STREQUAL "")

