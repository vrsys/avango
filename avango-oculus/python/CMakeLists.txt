LINK_DIRECTORIES(${LIB_PATHS} ${GUACAMOLE_OCULUS_LIBRARY_DIRS})

ADD_LIBRARY( _oculus SHARED ../src/avango/oculus/OculusRift.cpp ./avango/oculus/OculusRift.cpp ./avango/oculus/OculusRift.hpp ./avango/oculus/_oculus.cpp )

ADD_DEPENDENCIES ( _oculus avango_oculus )

set_target_properties( _oculus
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/oculus
)

LIST(APPEND LIBRARIES avango avango_gua avango_oculus ${GUACAMOLE_OCULUS_LIBRARIES})
TARGET_LINK_LIBRARIES( _oculus debug ${LIBRARIES} optimized ${LIBRARIES})

SET_TARGET_PROPERTIES( _oculus PROPERTIES PREFIX "")


