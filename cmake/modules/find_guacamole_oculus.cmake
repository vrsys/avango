##############################################################################
# search paths
##############################################################################
SET(GUACAMOLE_OCULUS_INCLUDE_SEARCH_DIRS
  ${GUACAMOLE_OCULUS_INCLUDE_SEARCH_DIR}
  ${GUACAMOLE_INCLUDE_DIRS}/../plugins/guacamole-oculus/include
  ${CMAKE_SOURCE_DIR}/../guacamole-oculus/include
  /opt/guacamole/current/guacamole-oculus/include
)

SET(GUACAMOLE_OCULUS_LIBRARY_SEARCH_DIRS
  ${GUACAMOLE_OCULUS_LIBRARY_SEARCH_DIR}
  ${GUACAMOLE_LIBRARY_DIRS}
  ${CMAKE_SOURCE_DIR}/../guacamole-oculus/lib
  /opt/guacamole/current/guacamole-oculus/lib
)

##############################################################################
# feedback to provide user-defined paths to search for guacamole-oculus
##############################################################################
MACRO (request_guacamole_oculus_search_directories)

    IF ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS AND NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )
        SET(GUACAMOLE_OCULUS_INCLUDE_SEARCH_DIR "Please provide guacamole-oculus include path." CACHE PATH "path to guacamole-oculus headers.")
        SET(GUACAMOLE_OCULUS_LIBRARY_SEARCH_DIR "Please provide guacamole-oculus library path." CACHE PATH "path to guacamole-oculus libraries.")
        MESSAGE(FATAL_ERROR "find_guacamole-oculus.cmake: unable to find guacamole-oculus.")
    ENDIF ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS AND NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )

    IF ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS )
        SET(GUACAMOLE_OCULUS_INCLUDE_SEARCH_DIR "Please provide guacamole-oculus include path." CACHE PATH "path to guacamole-oculus headers.")
        MESSAGE(FATAL_ERROR "find_guacamole-oculus.cmake: unable to find guacamole-oculus headers.")
    ELSE ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS )
        UNSET(GUACAMOLE_OCULUS_INCLUDE_SEARCH_DIR CACHE)
    ENDIF ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS )

    IF ( NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )
        SET(GUACAMOLE_OCULUS_LIBRARY_SEARCH_DIR "Please provide guacamole-oculus library path." CACHE PATH "path to guacamole-oculus libraries.")
        MESSAGE(FATAL_ERROR "find_guacamole-oculus.cmake: unable to find guacamole-oculus libraries.")
    ELSE ( NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )
        UNSET(GUACAMOLE_OCULUS_LIBRARY_SEARCH_DIR CACHE)
    ENDIF ( NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )

ENDMACRO (request_guacamole_oculus_search_directories)

##############################################################################
# search
##############################################################################
message(STATUS "-- checking for guacamole-oculus")

IF ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS )

    SET(_GUACAMOLE_OCULUS_FOUND_INC_DIRS "")

    FOREACH(_SEARCH_DIR ${GUACAMOLE_OCULUS_INCLUDE_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
                NAMES gua/OculusRift.hpp
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _GUACAMOLE_OCULUS_FOUND_INC_DIRS ${_CUR_SEARCH})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${GUACAMOLE_OCULUS_INCLUDE_SEARCH_DIRS})

    IF (NOT _GUACAMOLE_OCULUS_FOUND_INC_DIRS)
        request_guacamole_oculus_search_directories()
    ENDIF (NOT _GUACAMOLE_OCULUS_FOUND_INC_DIRS)

    FOREACH(_INC_DIR ${_GUACAMOLE_OCULUS_FOUND_INC_DIRS})
        LIST(APPEND _GUACAMOLE_OCULUS_INCLUDE_DIRS ${_INC_DIR})
    ENDFOREACH(_INC_DIR ${_GUACAMOLE_OCULUS_FOUND_INC_DIRS})

    IF ( _GUACAMOLE_OCULUS_FOUND_INC_DIRS)
        SET(GUACAMOLE_OCULUS_INCLUDE_DIRS ${_GUACAMOLE_OCULUS_INCLUDE_DIRS} CACHE PATH "guacamole-oculus include path.")
    ENDIF ( _GUACAMOLE_OCULUS_FOUND_INC_DIRS)

ENDIF ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS )


IF ( NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )

    SET(_GUACAMOLE_OCULUS_FOUND_LIB_DIR "")
    SET(_GUACAMOLE_OCULUS_POSTFIX "")

    FOREACH(_SEARCH_DIR ${GUACAMOLE_OCULUS_LIBRARY_SEARCH_DIRS})
		IF (UNIX)
			FIND_PATH(_CUR_SEARCH
					NAMES libguacamole-oculus.so
					PATHS ${_SEARCH_DIR}
					NO_DEFAULT_PATH)
		ELSEIF(WIN32)
			FIND_PATH(_CUR_SEARCH
					NAMES guacamole-oculus.lib
					PATHS ${_SEARCH_DIR}
					PATH_SUFFIXES debug release
					NO_DEFAULT_PATH)
		ENDIF(UNIX)
        IF (_CUR_SEARCH)
            LIST(APPEND _GUACAMOLE_OCULUS_FOUND_LIB_DIR ${_SEARCH_DIR})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${GUACAMOLE_OCULUS_LIBRARY_SEARCH_DIRS})

    IF (NOT _GUACAMOLE_OCULUS_FOUND_LIB_DIR)
        request_guacamole_oculus_search_directories()
    ELSE (NOT _GUACAMOLE_OCULUS_FOUND_LIB_DIR)
		    SET(GUACAMOLE_OCULUS_LIBRARY_DIRS ${_GUACAMOLE_OCULUS_FOUND_LIB_DIR} CACHE PATH "The guacamole-oculus library directory")
    ENDIF (NOT _GUACAMOLE_OCULUS_FOUND_LIB_DIR)

    SET(_GUACAMOLE_OCULUS_LIBRARIES "")

    FOREACH(_LIB_DIR ${_GUACAMOLE_OCULUS_FOUND_LIB_DIR})
		    IF (UNIX)
			    file(GLOB_RECURSE _GUACAMOLE_OCULUS_LIBRARIES_ABSOLUTE ${_LIB_DIR}/libguacamole-oculus.so)
		    ELSEIF(WIN32)
			    file(GLOB_RECURSE _GUACAMOLE_OCULUS_LIBRARIES_ABSOLUTE ${_LIB_DIR}/release/guacamole-oculus.lib)
		    ENDIF(UNIX)
    ENDFOREACH(_LIB_DIR ${_GUACAMOLE_OCULUS_FOUND_LIB_DIR})

    IF (_GUACAMOLE_OCULUS_FOUND_LIB_DIR)
        FOREACH (_GUACAMOLE_OCULUS_LIB_PATH ${_GUACAMOLE_OCULUS_LIBRARIES_ABSOLUTE})
            GET_FILENAME_COMPONENT(_GUACAMOLE_OCULUS_LIB_FILENAME ${_GUACAMOLE_OCULUS_LIB_PATH} NAME)
            LIST(APPEND _GUACAMOLE_OCULUS_LIBRARIES ${_GUACAMOLE_OCULUS_LIB_FILENAME})
        ENDFOREACH(_GUACAMOLE_OCULUS_LIB_PATH)
        SET(GUACAMOLE_OCULUS_LIBRARIES ${_GUACAMOLE_OCULUS_LIBRARIES} CACHE STRING "guacamole-oculus libraries")
    ENDIF (_GUACAMOLE_OCULUS_FOUND_LIB_DIR)

ENDIF ( NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )

##############################################################################
# verify
##############################################################################
IF ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS OR NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )
    request_GUACAMOLE_OCULUS_search_directories()
ELSE ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS OR NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )
    UNSET(GUACAMOLE_OCULUS_INCLUDE_SEARCH_DIR CACHE)
    UNSET(GUACAMOLE_OCULUS_LIBRARY_SEARCH_DIR CACHE)
    SET(GLOBAL_EXT_DIR ${GUACAMOLE_OCULUS_INCLUDE_DIRS}/../externals)
    MESSAGE(STATUS "--  found matching guacamole-oculus version")
ENDIF ( NOT GUACAMOLE_OCULUS_INCLUDE_DIRS OR NOT GUACAMOLE_OCULUS_LIBRARY_DIRS )

