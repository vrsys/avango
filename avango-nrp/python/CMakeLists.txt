
LINK_DIRECTORIES(${LIB_PATHS} ${GUACAMOLE_NRP_LIBRARY_DIRS})

file(GLOB_RECURSE AVANGO_NRP_PYTHON_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp *.hpp)

include_directories(${PYTHON3_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIR})

ADD_LIBRARY(_nrp SHARED ${AVANGO_NRP_PYTHON_SRC})

set_target_properties(_nrp PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/avango/gua/nrp)

ADD_DEPENDENCIES(_nrp avango_nrp)

TARGET_LINK_LIBRARIES(_nrp debug ${LIBRARIES} avango_nrp optimized ${LIBRARIES} avango_nrp)
SET_TARGET_PROPERTIES(_nrp PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
    post_build_install_target(_nrp ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/nrp/_nrp.pyd)
ELSEIF (UNIX)
    post_build_install_target(_nrp ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/nrp/_nrp.so)
ENDIF (MSVC)

###############################################################################
# install utilities
###############################################################################
COPY_PYTHON_FILES_INTO_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR} ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/gua/nrp _nrp)
