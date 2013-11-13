##############################################################################
# search paths
##############################################################################
SET(PYTHON_INCLUDE_SEARCH_DIRS
  ${GLOBAL_EXT_DIR}/inc/python
  ${PYTHON_INCLUDE_DIRS}
  ${PYTHON_INCLUDE_SEARCH_DIR}
  "/usr/include/"
)

SET(PYTHON_LIBRARY_SEARCH_DIRS
  ${GLOBAL_EXT_DIR}/lib
  ${PYTHON_LIBRARY_DIRS}
  ${PYTHON_LIBRARY_SEARCH_DIR}
  "/usr/lib/"
)

##############################################################################
# feedback to provide user-defined paths to search for python
##############################################################################
MACRO (request_python_search_directories)
    
    IF ( NOT PYTHON_INCLUDE_DIRS AND NOT PYTHON_LIBRARY_DIRS )
        SET(PYTHON_INCLUDE_SEARCH_DIR "Please provide python include path." CACHE PATH "path to python headers.")
        SET(PYTHON_LIBRARY_SEARCH_DIR "Please provide python library path." CACHE PATH "path to python libraries.")
        MESSAGE(FATAL_ERROR "find_python.cmake: unable to find python.")
    ENDIF ( NOT PYTHON_INCLUDE_DIRS AND NOT PYTHON_LIBRARY_DIRS )

    IF ( NOT PYTHON_INCLUDE_DIRS )
        SET(PYTHON_INCLUDE_SEARCH_DIR "Please provide python include path." CACHE PATH "path to python headers.")
        MESSAGE(FATAL_ERROR "find_python.cmake: unable to find python headers.")
    ELSE ( NOT PYTHON_INCLUDE_DIRS )
        UNSET(PYTHON_INCLUDE_SEARCH_DIR CACHE)
    ENDIF ( NOT PYTHON_INCLUDE_DIRS )

    IF ( NOT PYTHON_LIBRARY_DIRS )
        SET(PYTHON_LIBRARY_SEARCH_DIR "Please provide python library path." CACHE PATH "path to python libraries.")
        MESSAGE(FATAL_ERROR "find_python.cmake: unable to find python libraries.")
    ELSE ( NOT PYTHON_LIBRARY_DIRS )
        UNSET(PYTHON_LIBRARY_SEARCH_DIR CACHE)
    ENDIF ( NOT PYTHON_LIBRARY_DIRS ) 

ENDMACRO (request_python_search_directories)

##############################################################################
# search
##############################################################################
message("-- checking for PYTHON")

IF (NOT PYTHON_INCLUDE_DIRS)

    SET(_PYTHON_FOUND_INC_DIRS "")
    FOREACH(_SEARCH_DIR ${PYTHON_INCLUDE_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
            NAMES pyconfig.h
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _PYTHON_FOUND_INC_DIRS ${_CUR_SEARCH})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${PYTHON_INCLUDE_SEARCH_DIRS})

    IF (NOT _PYTHON_FOUND_INC_DIRS)
        request_python_search_directories()
    ENDIF (NOT _PYTHON_FOUND_INC_DIRS)
	  
	  FOREACH(_INC_DIR ${_PYTHON_FOUND_INC_DIRS})
        SET(PYTHON_INCLUDE_DIRS ${PYTHON_INCLUDE_DIRS} ${_INC_DIR} CACHE PATH "Python include directory.")
    ENDFOREACH(_INC_DIR ${_PYTHON_FOUND_INC_DIRS})
    
ENDIF (NOT PYTHON_INCLUDE_DIRS)

IF(UNIX)
	SET(PYTHON_LIB_FILENAME "libpython2.7.so")
ELSEIF(WIN32)
	SET(PYTHON_LIB_FILENAME "python27.lib")
ENDIF(UNIX)

IF ( NOT PYTHON_LIBRARY_DIRS )

    SET(_PYTHON_FOUND_LIB_DIR "")
    SET(_PYTHON_POSTFIX "")

    FOREACH(_SEARCH_DIR ${PYTHON_LIBRARY_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
				        NAMES ${PYTHON_LIB_FILENAME}
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _PYTHON_FOUND_LIB_DIR ${_SEARCH_DIR})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${PYTHON_LIBRARY_SEARCH_DIRS})

    IF (NOT _PYTHON_FOUND_LIB_DIR)
        request_python_search_directories()
    ELSE (NOT _PYTHON_FOUND_LIB_DIR)
		    SET(PYTHON_LIBRARY_DIRS ${_PYTHON_FOUND_LIB_DIR} CACHE PATH "The python library directory")
    ENDIF (NOT _PYTHON_FOUND_LIB_DIR)
    
    FOREACH(_LIB_DIR ${_PYTHON_FOUND_LIB_DIR})
        SET(PYTHON_LIBRARIES ${PYTHON_LIB_FILENAME} CACHE FILEPATH "The python library filename.")
    ENDFOREACH(_LIB_DIR ${_PYTHON_FOUND_INC_DIRS})

ENDIF ( NOT PYTHON_LIBRARY_DIRS )

##############################################################################
# verify
##############################################################################
IF ( NOT PYTHON_INCLUDE_DIRS OR NOT PYTHON_LIBRARY_DIRS )
    request_python_search_directories()
ELSE ( NOT PYTHON_INCLUDE_DIRS OR NOT PYTHON_LIBRARY_DIRS ) 
    UNSET(PYTHON_INCLUDE_SEARCH_DIR CACHE)
    UNSET(PYTHON_LIBRARY_SEARCH_DIR CACHE)
    MESSAGE("--  found matching python version")
ENDIF ( NOT PYTHON_INCLUDE_DIRS OR NOT PYTHON_LIBRARY_DIRS )
