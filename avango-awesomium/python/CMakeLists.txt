ADD_LIBRARY( _gui SHARED
  "_gui.cpp"
  "GuiResourceNode.hpp"
  "GuiResourceNode.cpp"
  )

set_target_properties( _gui
  PROPERTIES LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_SOURCE_DIR}/lib/avango/gua/gui
)

TARGET_LINK_LIBRARIES( _gui avango_awesomium)

SET_TARGET_PROPERTIES( _gui PROPERTIES PREFIX "")

###############################################################################
# install modules
###############################################################################
IF (MSVC)
  post_build_install_target(_gui
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/gui/_gui.pyd)
ELSEIF (UNIX)
  post_build_install_target(_gui
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/gui/_gui.so)
ENDIF (MSVC)

COPY_PYTHON_FILES_INTO_DIRECTORY(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${AVANGO_INSTALL_DIRECTORY}/python3.4/avango/gua/gui
    _gui
)
