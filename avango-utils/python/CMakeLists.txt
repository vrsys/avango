
LINK_DIRECTORIES(
    ${LIB_PATHS}
    ${BULLET_LIBRARY_DIRS}
)

include_directories(${PYTHON_INCLUDE_DIR})

ADD_LIBRARY( _utils SHARED "_utils.cpp" )

set_target_properties( _utils
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/utils
)

ADD_DEPENDENCIES ( _utils avango_utils )

TARGET_LINK_LIBRARIES( _utils 
                        avango_utils
                        ${PYTHON_LIBRARY}
                        ${PYTHON3_LIBRARIES}
                        optimized ${Boost_PYTHON_LIBRARY_RELEASE} debug ${Boost_PYTHON_LIBRARY_DEBUG}
                        optimized ${Boost_PYTHON3_LIBRARY_RELEASE} debug ${Boost_PYTHON3_LIBRARY_DEBUG}
                     )

set_target_properties( _utils PROPERTIES COMPILE_FLAGS "-D BOOST_PYTHON_STATIC_LIB")
SET_TARGET_PROPERTIES( _utils PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_utils
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/utils/_utils.pyd)
ELSEIF (UNIX)
  post_build_install_target(_utils ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/utils/_utils.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/utils
    _utils
)
