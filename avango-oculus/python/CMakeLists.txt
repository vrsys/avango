LINK_DIRECTORIES(${LIB_PATHS} ${GUACAMOLE_OCULUS_LIBRARY_DIRS})

ADD_LIBRARY( _oculus SHARED ./avango/oculus/OculusRift.cpp ./avango/oculus/OculusRift.hpp ./avango/oculus/_oculus.cpp )

ADD_DEPENDENCIES ( _oculus avango_oculus )

FILE(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/avango/oculus)

SET_TARGET_PROPERTIES( _oculus
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/oculus
)

LIST(APPEND LIBRARIES avango avango_gua avango_oculus ${GUACAMOLE_OCULUS_LIBRARIES})
TARGET_LINK_LIBRARIES( _oculus debug ${LIBRARIES} optimized ${LIBRARIES})

SET_TARGET_PROPERTIES( _oculus PROPERTIES PREFIX "")

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(_oculus ${AVANGO_INSTALL_DIRECTORY}/python2.7/avango/oculus/_oculus.pyd)
ENDIF (MSVC)
