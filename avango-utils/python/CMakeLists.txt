ADD_LIBRARY( _utils SHARED "_utils.cpp" )

set_target_properties( _utils
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/utils
)

TARGET_LINK_LIBRARIES(_utils avango_utils)

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
