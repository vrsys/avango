# determine source and header files
file(GLOB_RECURSE GUACAMOLE_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} avango/gua/*.c  avango/gua/*.cpp ../include/avango/gua/*.h ../include/avango/gua/*.hpp)

ADD_LIBRARY( avango-gua SHARED
    ${GUACAMOLE_SRC}
)

TARGET_LINK_LIBRARIES( avango-gua  ${LIBS} )
