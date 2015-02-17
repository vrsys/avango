##############################################################################
# search paths
##############################################################################
SET(GUACAMOLE_GUI_INCLUDE_SEARCH_DIRS
  ${GUACAMOLE_GUI_INCLUDE_SEARCH_DIR}
  ${GUACAMOLE_INCLUDE_DIRS}/../plugins/guacamole-gui/include
  ${CMAKE_SOURCE_DIR}/../guacamole/install/include
  /opt/guacamole/current/guacamole/install/include
)

SET(GUACAMOLE_GUI_LIBRARY_SEARCH_DIRS
  ${GUACAMOLE_GUI_LIBRARY_SEARCH_DIR}
  ${GUACAMOLE_LIBRARY_DIRS}
  ${CMAKE_SOURCE_DIR}/../guacamole/install/lib
  /opt/guacamole/current/guacamole/install/lib
)

##############################################################################
# feedback to provide user-defined paths to search for guacamole-gui
##############################################################################
MACRO (request_guacamole_gui_search_directories)

    IF ( NOT GUACAMOLE_GUI_INCLUDE_DIRS AND NOT GUACAMOLE_GUI_LIBRARY_DIRS )
        SET(GUACAMOLE_GUI_INCLUDE_SEARCH_DIR "Please provide guacamole-gui include path." CACHE PATH "path to guacamole-gui headers.")
        SET(GUACAMOLE_GUI_LIBRARY_SEARCH_DIR "Please provide guacamole-gui library path." CACHE PATH "path to guacamole-gui libraries.")
        MESSAGE(FATAL_ERROR "find_guacamole-gui.cmake: unable to find guacamole-gui.")
    ENDIF ( NOT GUACAMOLE_GUI_INCLUDE_DIRS AND NOT GUACAMOLE_GUI_LIBRARY_DIRS )

    IF ( NOT GUACAMOLE_GUI_INCLUDE_DIRS )
        SET(GUACAMOLE_GUI_INCLUDE_SEARCH_DIR "Please provide guacamole-gui include path." CACHE PATH "path to guacamole-gui headers.")
        MESSAGE(FATAL_ERROR "find_guacamole-gui.cmake: unable to find guacamole-gui headers.")
    ELSE ( NOT GUACAMOLE_GUI_INCLUDE_DIRS )
        UNSET(GUACAMOLE_GUI_INCLUDE_SEARCH_DIR CACHE)
    ENDIF ( NOT GUACAMOLE_GUI_INCLUDE_DIRS )

    IF ( NOT GUACAMOLE_GUI_LIBRARY_DIRS )
        SET(GUACAMOLE_GUI_LIBRARY_SEARCH_DIR "Please provide guacamole-gui library path." CACHE PATH "path to guacamole-gui libraries.")
        MESSAGE(FATAL_ERROR "find_guacamole-gui.cmake: unable to find guacamole-gui libraries.")
    ELSE ( NOT GUACAMOLE_GUI_LIBRARY_DIRS )
        UNSET(GUACAMOLE_GUI_LIBRARY_SEARCH_DIR CACHE)
    ENDIF ( NOT GUACAMOLE_GUI_LIBRARY_DIRS )

ENDMACRO (request_guacamole_gui_search_directories)

##############################################################################
# search
##############################################################################
message(STATUS "-- checking for guacamole-gui")

IF ( NOT GUACAMOLE_GUI_INCLUDE_DIRS )

    SET(_GUACAMOLE_GUI_FOUND_INC_DIRS "")

    FOREACH(_SEARCH_DIR ${GUACAMOLE_GUI_INCLUDE_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
                NAMES gua/gui/GuiResource.hpp
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _GUACAMOLE_GUI_FOUND_INC_DIRS ${_CUR_SEARCH})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${GUACAMOLE_GUI_INCLUDE_SEARCH_DIRS})

    IF (NOT _GUACAMOLE_GUI_FOUND_INC_DIRS)
        request_guacamole_gui_search_directories()
    ENDIF (NOT _GUACAMOLE_GUI_FOUND_INC_DIRS)

    FOREACH(_INC_DIR ${_GUACAMOLE_GUI_FOUND_INC_DIRS})
        LIST(APPEND _GUACAMOLE_GUI_INCLUDE_DIRS ${_INC_DIR})
    ENDFOREACH(_INC_DIR ${_GUACAMOLE_GUI_FOUND_INC_DIRS})

    IF ( _GUACAMOLE_GUI_FOUND_INC_DIRS)
        SET(GUACAMOLE_GUI_INCLUDE_DIRS ${_GUACAMOLE_GUI_INCLUDE_DIRS} CACHE PATH "guacamole-gui include path.")
    ENDIF ( _GUACAMOLE_GUI_FOUND_INC_DIRS)

ENDIF ( NOT GUACAMOLE_GUI_INCLUDE_DIRS )


IF ( NOT GUACAMOLE_GUI_LIBRARY_DIRS )

    SET(_GUACAMOLE_GUI_FOUND_LIB_DIR "")
    SET(_GUACAMOLE_GUI_POSTFIX "")

    FOREACH(_SEARCH_DIR ${GUACAMOLE_GUI_LIBRARY_SEARCH_DIRS})
    IF (UNIX)
      FIND_PATH(_CUR_SEARCH
          NAMES libguacamole-gui.so
          PATHS ${_SEARCH_DIR}
          NO_DEFAULT_PATH)
    ELSEIF(WIN32)
      FIND_PATH(_CUR_SEARCH
          NAMES guacamole-gui.lib
          PATHS ${_SEARCH_DIR}
          PATH_SUFFIXES debug release
          NO_DEFAULT_PATH)
    ENDIF(UNIX)
        IF (_CUR_SEARCH)
            LIST(APPEND _GUACAMOLE_GUI_FOUND_LIB_DIR ${_SEARCH_DIR})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${GUACAMOLE_GUI_LIBRARY_SEARCH_DIRS})

    IF (NOT _GUACAMOLE_GUI_FOUND_LIB_DIR)
        request_guacamole_gui_search_directories()
    ELSE (NOT _GUACAMOLE_GUI_FOUND_LIB_DIR)
        SET(GUACAMOLE_GUI_LIBRARY_DIRS ${_GUACAMOLE_GUI_FOUND_LIB_DIR} CACHE PATH "The guacamole-gui library directory")
    ENDIF (NOT _GUACAMOLE_GUI_FOUND_LIB_DIR)

    SET(_GUACAMOLE_GUI_LIBRARIES "")

    FOREACH(_LIB_DIR ${_GUACAMOLE_GUI_FOUND_LIB_DIR})
        IF (UNIX)
          file(GLOB_RECURSE _GUACAMOLE_GUI_LIBRARIES_ABSOLUTE ${_LIB_DIR}/libguacamole-gui.so)
        ELSEIF(WIN32)
          file(GLOB_RECURSE _GUACAMOLE_GUI_LIBRARIES_ABSOLUTE ${_LIB_DIR}/release/guacamole-gui.lib)
        ENDIF(UNIX)
    ENDFOREACH(_LIB_DIR ${_GUACAMOLE_GUI_FOUND_LIB_DIR})

    IF (_GUACAMOLE_GUI_FOUND_LIB_DIR)
        FOREACH (_GUACAMOLE_GUI_LIB_PATH ${_GUACAMOLE_GUI_LIBRARIES_ABSOLUTE})
            GET_FILENAME_COMPONENT(_GUACAMOLE_GUI_LIB_FILENAME ${_GUACAMOLE_GUI_LIB_PATH} NAME)
            LIST(APPEND _GUACAMOLE_GUI_LIBRARIES ${_GUACAMOLE_GUI_LIB_FILENAME})
        ENDFOREACH(_GUACAMOLE_GUI_LIB_PATH)
        SET(GUACAMOLE_GUI_LIBRARIES ${_GUACAMOLE_GUI_LIBRARIES} CACHE STRING "guacamole-gui libraries")
    ENDIF (_GUACAMOLE_GUI_FOUND_LIB_DIR)

ENDIF ( NOT GUACAMOLE_GUI_LIBRARY_DIRS )

##############################################################################
# verify
##############################################################################
IF ( NOT GUACAMOLE_GUI_INCLUDE_DIRS OR NOT GUACAMOLE_GUI_LIBRARY_DIRS )
    request_GUACAMOLE_GUI_search_directories()
ELSE ( NOT GUACAMOLE_GUI_INCLUDE_DIRS OR NOT GUACAMOLE_GUI_LIBRARY_DIRS )
    UNSET(GUACAMOLE_GUI_INCLUDE_SEARCH_DIR CACHE)
    UNSET(GUACAMOLE_GUI_LIBRARY_SEARCH_DIR CACHE)
    SET(GLOBAL_EXT_DIR ${GUACAMOLE_GUI_INCLUDE_DIRS}/../externals)
    MESSAGE(STATUS "--  found matching guacamole-gui version")
ENDIF ( NOT GUACAMOLE_GUI_INCLUDE_DIRS OR NOT GUACAMOLE_GUI_LIBRARY_DIRS )

