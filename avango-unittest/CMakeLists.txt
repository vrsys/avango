################################################################
# avango-unittests
################################################################

# determine sources
file(GLOB_RECURSE AVANGO_UNITTEST_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  src/avango/*.cpp
  include/avango/*.h
  include/avango/logging/*.h
)

ADD_LIBRARY( avango_unittest SHARED
    	     ${AVANGO_UNITTEST_SRC}
)


