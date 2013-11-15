LINK_DIRECTORIES(${LIB_PATHS})

ADD_LIBRARY( _daemon SHARED "_daemon.cpp" )

set_target_properties( _daemon
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/daemon
)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/lib/avango/daemon
    _daemon
)

ADD_DEPENDENCIES ( _daemon avangoDaemon )

#IF (NOT ${LIBRARIES} STREQUAL "")
    TARGET_LINK_LIBRARIES( _daemon debug ${LIBRARIES} avangoDaemon optimized ${LIBRARIES} avangoDaemon)
    SET_TARGET_PROPERTIES( _daemon PROPERTIES PREFIX "")
#ENDIF (NOT ${LIBRARIES} STREQUAL "")

