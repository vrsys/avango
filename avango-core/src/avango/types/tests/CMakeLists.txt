ADD_EXECUTABLE(unittest_container_pool "TestContainerPool.cpp")
ADD_EXECUTABLE(unittest_types "TestType.cpp" "TestLibType.cpp")

TARGET_LINK_LIBRARIES (unittest_container_pool avango)
TARGET_LINK_LIBRARIES (unittest_types avango)

INCLUDE_DIRECTORIES( ${INCLUDE_PATHS} 
  ${CMAKE_SOURCE_DIR}/avango-unittest/include 
)
