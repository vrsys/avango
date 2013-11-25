###############################################################################
# determine source and header files
###############################################################################
file(GLOB AVANGO_OCULUS_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    src/avango/oculus/*.cpp
    include/avango/oculus/*.hpp
)


# Location where cmake first looks for modules.
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/avango-oculus/cmake/modules)

include(find_ovr)
include(find_guacamole_oculus)

###############################################################################
# set link directories and link dependencies
###############################################################################
LINK_DIRECTORIES(${LIB_PATHS} ${GUACAMOLE_OCULUS_LIBRARY_DIRS} ${PYTHON_LIBRARY_DIRS})

INCLUDE_DIRECTORIES(${INCLUDE_PATHS}
                    ${SCHISM_INCLUDE_DIRS}
                    ${BULLET_INCLUDE_DIRS}
					${PYTHON_INCLUDE_DIRS}
					${GUACAMOLE_OCULUS_INCLUDE_DIRS}
					include
					../avango-core/include
					../avango-gua/include
					../avango-python/include
					../../guacamole-oculus/include
)

ADD_LIBRARY( avango_oculus SHARED
    	     ${AVANGO_OCULUS_SRC}
)

IF (MSVC)
	set_target_properties(avango_oculus PROPERTIES COMPILE_FLAGS "-D AV_OCULUS_LIBRARY")
ENDIF (MSVC)

ADD_DEPENDENCIES ( avango_oculus avango )

###############################################################################
# set preprocessor configuration
###############################################################################
LIST(APPEND LIBRARIES avango avango_script avango_gua ${GUACAMOLE_OCULUS_LIBRARIES})

TARGET_LINK_LIBRARIES( avango_oculus debug ${LIBRARIES} optimized ${LIBRARIES} )

###############################################################################
# python module
###############################################################################
add_subdirectory(python)

###############################################################################
# rename libaries in windows
###############################################################################
IF (MSVC)
  post_build_install_target(avango_oculus ${AVANGO_INSTALL_DIRECTORY}/avango/avango_oculus.dll)
ENDIF (MSVC)

