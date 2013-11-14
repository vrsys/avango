LINK_DIRECTORIES(${LIB_PATHS})

ADD_LIBRARY( _daemon SHARED "_daemon.cpp" )

ADD_DEPENDENCIES ( _daemon avangoDaemon )

#IF (NOT ${LIBRARIES} STREQUAL "")
    TARGET_LINK_LIBRARIES( _daemon debug ${LIBRARIES} avangoDaemon optimized ${LIBRARIES} avangoDaemon)
    SET_TARGET_PROPERTIES( _daemon PROPERTIES PREFIX "")
#ENDIF (NOT ${LIBRARIES} STREQUAL "")

