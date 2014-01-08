ADD_EXECUTABLE(unittest_application "TestApplication.cpp")
ADD_EXECUTABLE(unittest_time_sensor "TestTimeSensor.cpp")

TARGET_LINK_LIBRARIES (unittest_application avango_core)
TARGET_LINK_LIBRARIES (unittest_time_sensor avango_core)

INCLUDE_DIRECTORIES( ${INCLUDE_PATHS} 
  ${CMAKE_SOURCE_DIR}/avango-unittest/include 
)