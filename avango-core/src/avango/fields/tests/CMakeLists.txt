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

TARGET_LINK_LIBRARIES(unittest_fields avango_unittest)
TARGET_LINK_LIBRARIES(unittest_field_referencing avango_unittest)