ADD_EXECUTABLE(unittest_fields 
    "TestFields.cpp"
    "TestFieldTypeConversion.cpp"
    "TestFieldMultiPush.cpp"
    "TestMultiFields.cpp"
    "TestCloneFields.cpp"
    "TestFieldConst.cpp"
)

ADD_EXECUTABLE(unittest_field_referencing
    "TestFieldReferencing.cpp"
)

TARGET_LINK_LIBRARIES(unittest_fields avango_core)
TARGET_LINK_LIBRARIES(unittest_field_referencing avango_core)

INCLUDE_DIRECTORIES( ${INCLUDE_PATHS} 
  ${CMAKE_SOURCE_DIR}/avango-unittest/include
  ${CMAKE_SOURCE_DIR}/avango-unittest/include/avango
)