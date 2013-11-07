##############################################################################
# search paths
##############################################################################
SET(GUACAMOLE_INCLUDE_SEARCH_DIRS
  ${GUACAMOLE_INCLUDE_DIRS}
  ${GUACAMOLE_INCLUDE_SEARCH_DIR}
  /opt/guacamole/current  
)

SET(GUACAMOLE_LIBRARY_SEARCH_DIRS
  ${GUACAMOLE_LIBRARY_DIRS}
  ${GUACAMOLE_LIBRARY_SEARCH_DIR}
  /opt/guacamole/current/lib/linux_x86
)

##############################################################################
# feedback to provide user-defined paths to search for guacamole
##############################################################################
MACRO (request_guacamole_search_directories)
    
    IF ( NOT GUACAMOLE_INCLUDE_DIRS AND NOT GUACAMOLE_LIBRARY_DIRS )
        SET(GUACAMOLE_INCLUDE_SEARCH_DIR "Please provide guacamole include path." CACHE PATH "path to guacamole headers.")
        SET(GUACAMOLE_LIBRARY_SEARCH_DIR "Please provide guacamole library path." CACHE PATH "path to guacamole libraries.")
        MESSAGE(FATAL_ERROR "find_guacamole.cmake: unable to find guacamole.")
    ENDIF ( NOT GUACAMOLE_INCLUDE_DIRS AND NOT GUACAMOLE_LIBRARY_DIRS )

    IF ( NOT GUACAMOLE_INCLUDE_DIRS )
        SET(GUACAMOLE_INCLUDE_SEARCH_DIR "Please provide guacamole include path." CACHE PATH "path to guacamole headers.")
        MESSAGE(FATAL_ERROR "find_guacamole.cmake: unable to find guacamole headers.")
    ELSE ( NOT GUACAMOLE_INCLUDE_DIRS )
        UNSET(GUACAMOLE_INCLUDE_SEARCH_DIR CACHE)
    ENDIF ( NOT GUACAMOLE_INCLUDE_DIRS )

    IF ( NOT GUACAMOLE_LIBRARY_DIRS )
        SET(GUACAMOLE_LIBRARY_SEARCH_DIR "Please provide guacamole library path." CACHE PATH "path to guacamole libraries.")
        MESSAGE(FATAL_ERROR "find_guacamole.cmake: unable to find guacamole libraries.")
    ELSE ( NOT GUACAMOLE_LIBRARY_DIRS )
        UNSET(GUACAMOLE_LIBRARY_SEARCH_DIR CACHE)
    ENDIF ( NOT GUACAMOLE_LIBRARY_DIRS ) 

ENDMACRO (request_guacamole_search_directories)

##############################################################################
# search
##############################################################################
message("-- checking for guacamole")

IF ( NOT GUACAMOLE_INCLUDE_DIRS )

    SET(_GUACAMOLE_FOUND_INC_DIRS "")

    FOREACH(_SEARCH_DIR ${GUACAMOLE_INCLUDE_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
                NAMES gua/guacamole.hpp
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _GUACAMOLE_FOUND_INC_DIRS ${_CUR_SEARCH})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${GUACAMOLE_INCLUDE_SEARCH_DIRS})

    IF (NOT _GUACAMOLE_FOUND_INC_DIRS)
        request_guacamole_search_directories()
    ENDIF (NOT _GUACAMOLE_FOUND_INC_DIRS)
    
    FOREACH(_INC_DIR ${_GUACAMOLE_FOUND_INC_DIRS})
        LIST(APPEND _GUACAMOLE_INCLUDE_DIRS ${_INC_DIR})
    ENDFOREACH(_INC_DIR ${_GUACAMOLE_FOUND_INC_DIRS})

    IF ( _GUACAMOLE_FOUND_INC_DIRS)
        SET(GUACAMOLE_INCLUDE_DIRS ${_GUACAMOLE_INCLUDE_DIRS} CACHE PATH "guacamole include path.")
    ENDIF ( _GUACAMOLE_FOUND_INC_DIRS)

ENDIF ( NOT GUACAMOLE_INCLUDE_DIRS )


IF ( NOT GUACAMOLE_LIBRARY_DIRS )

    SET(_GUACAMOLE_FOUND_LIB_DIR "")
    SET(_GUACAMOLE_POSTFIX "")

    FOREACH(_SEARCH_DIR ${GUACAMOLE_LIBRARY_SEARCH_DIRS})
		IF (UNIX)
			FIND_PATH(_CUR_SEARCH
					NAMES libguacamole.so
					PATHS ${_SEARCH_DIR}
					NO_DEFAULT_PATH)
		ELSEIF(WIN32)
			FIND_PATH(_CUR_SEARCH
					NAMES guacamole.lib
					PATHS ${_SEARCH_DIR}
					PATH_SUFFIXES debug release
					NO_DEFAULT_PATH)
		ENDIF(UNIX)
        IF (_CUR_SEARCH)
            LIST(APPEND _GUACAMOLE_FOUND_LIB_DIR ${_SEARCH_DIR})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${GUACAMOLE_LIBRARY_SEARCH_DIRS})

    IF (NOT _GUACAMOLE_FOUND_LIB_DIR)
        request_guacamole_search_directories()
    ELSE (NOT _GUACAMOLE_FOUND_LIB_DIR)
		    SET(GUACAMOLE_LIBRARY_DIRS ${_GUACAMOLE_FOUND_LIB_DIR} CACHE PATH "The guacamole library directory")
    ENDIF (NOT _GUACAMOLE_FOUND_LIB_DIR)
    
    SET(_GUACAMOLE_LIBRARIES "")

    FOREACH(_LIB_DIR ${_GUACAMOLE_FOUND_LIB_DIR})
		    IF (UNIX)
			    file(GLOB GUACAMOLE_LIBRARIES ${_LIB_DIR}/*.so)
		    ELSEIF(WIN32)
			    file(GLOB _GUACAMOLE_LIBRARY_ABSOLUTE_PATHS ${_LIB_DIR}/release/*.lib)	
			    FOREACH (_GUACAMOLE_LIB_PATH ${_GUACAMOLE_LIBRARY_ABSOLUTE_PATHS})
				    SET(_GUACAMOLE_LIB_FILENAME, "")
				    GET_FILENAME_COMPONENT(_GUACAMOLE_LIB_FILENAME ${_GUACAMOLE_LIB_PATH} NAME)
				    LIST(APPEND _GUACAMOLE_LIBRARIES ${_GUACAMOLE_LIB_FILENAME})
			    ENDFOREACH(_GUACAMOLE_LIB_PATH)
		    ENDIF(UNIX)
    ENDFOREACH(_LIB_DIR ${_GUACAMOLE_FOUND_LIB_DIR})
    
    IF (_GUACAMOLE_FOUND_LIB_DIR)
        SET(GUACAMOLE_LIBRARIES ${_GUACAMOLE_LIBRARIES} CACHE STRING "guacamole libraries")
    ENDIF (_GUACAMOLE_FOUND_LIB_DIR)

ENDIF ( NOT GUACAMOLE_LIBRARY_DIRS )

##############################################################################
# verify
##############################################################################
IF ( NOT GUACAMOLE_INCLUDE_DIRS OR NOT GUACAMOLE_LIBRARY_DIRS )
    request_guacamole_search_directories()
ELSE ( NOT GUACAMOLE_INCLUDE_DIRS OR NOT GUACAMOLE_LIBRARY_DIRS ) 
    UNSET(GUACAMOLE_INCLUDE_SEARCH_DIR CACHE)
    UNSET(GUACAMOLE_LIBRARY_SEARCH_DIR CACHE)
    MESSAGE("--  found matching guacamole version")
ENDIF ( NOT GUACAMOLE_INCLUDE_DIRS OR NOT GUACAMOLE_LIBRARY_DIRS )

