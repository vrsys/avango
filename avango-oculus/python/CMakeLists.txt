ADD_LIBRARY( _oculus SHARED
    ./avango/oculus/OculusWindow.cpp
    ./avango/oculus/OculusWindow.hpp
    ./avango/oculus/_oculus.cpp
)

set_target_properties( _oculus
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/oculus
)

TARGET_LINK_LIBRARIES(oculus avango_oculus)

SET_TARGET_PROPERTIES( _oculus PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_oculus ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/oculus/_oculus.pyd)
ELSEIF (UNIX)
  post_build_install_target(_oculus ${AVANGO_INSTALL_DIRECTORY}/python3.5/avango/oculus/_oculus.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}/avango/oculus
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/oculus
    _oculus
)
