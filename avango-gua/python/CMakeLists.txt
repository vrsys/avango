# determine source and header files
file(GLOB_RECURSE GUACAMOLE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} avango/gua/*.c  avango/gua/*.cpp avango/gua/*.h avango/gua/*.hpp)

ADD_LIBRARY( _gua SHARED
    ${GUACAMOLE_SRC}
)

TARGET_LINK_LIBRARIES( _gua  ${LIBS} avango-gua )
SET_TARGET_PROPERTIES( _gua PROPERTIES PREFIX "")
