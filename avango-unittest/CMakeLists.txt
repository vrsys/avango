################################################################
# avango-unittests
################################################################

SET(AVANGO_UNITTEST_SRC "")

IF (UNIX)
  FILE(GLOB AVANGO_UNITTEST_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/*.cpp
    src/avango/Posix/*.cpp
    include/avango/UnitTest++/*.h
    include/avango/UnitTest++/Posix/*.h
  )

ELSEIF (MSVC)
  FILE(GLOB AVANGO_UNITTEST_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/*.cpp
    src/avango/Win32/*.cpp
    include/avango/UnitTest++/*.h
    include/avango/UnitTest++/Win32/*.h
  )
ENDIF (UNIX)

ADD_LIBRARY( avango_unittest STATIC
    	     ${AVANGO_UNITTEST_SRC}
)
# link with avango to tests dont have to link with it additionally
TARGET_LINK_LIBRARIES(avango_unittest
  avango
)

target_include_directories(avango_unittest SYSTEM PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/UnitTest++
  ${CMAKE_CURRENT_SOURCE_DIR}/include/avango/UnitTest++/tests
)

target_include_directories(avango_unittest SYSTEM INTERFACE
  ${CMAKE_SOURCE_DIR}/avango-unittest/include
  ${CMAKE_SOURCE_DIR}/avango-unittest/include/avango
)