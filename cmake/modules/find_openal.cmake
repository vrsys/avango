##############################################################################
# search paths
##############################################################################
SET(OPENAL_INCLUDE_SEARCH_DIRS
  ${GLOBAL_EXT_DIR}/inc/openal
  ${OPENAL_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_SEARCH_DIR}
  "/opt/openal/current/include"
)

SET(OPENAL_LIBRARY_SEARCH_DIRS
  ${GLOBAL_EXT_DIR}/lib
  ${OPENAL_LIBRARY_DIRS}
  ${OPENAL_LIBRARY_SEARCH_DIR}
  "/opt/openal/current/lib"
)

##############################################################################
# feedback to provide user-defined paths to search for openal
##############################################################################
MACRO (request_openal_search_directories)
    
  IF ( NOT OPENAL_INCLUDE_DIRS AND NOT OPENAL_LIBRARY_DIRS )
    SET(OPENAL_INCLUDE_SEARCH_DIR "Please provide openal include path." CACHE PATH "path to openal headers.")
    SET(OPENAL_LIBRARY_SEARCH_DIR "Please provide openal library path." CACHE PATH "path to openal libraries.")
    MESSAGE(FATAL_ERROR "find_openal.cmake: unable to find openal.")
  ENDIF ( NOT OPENAL_INCLUDE_DIRS AND NOT OPENAL_LIBRARY_DIRS )

  IF ( NOT OPENAL_INCLUDE_DIRS )
    SET(OPENAL_INCLUDE_SEARCH_DIR "Please provide openal include path." CACHE PATH "path to openal headers.")
    MESSAGE(FATAL_ERROR "find_openal.cmake: unable to find openal headers.")
  ELSE ( NOT OPENAL_INCLUDE_DIRS )
    UNSET(OPENAL_INCLUDE_SEARCH_DIR CACHE)
  ENDIF ( NOT OPENAL_INCLUDE_DIRS )

  IF ( NOT OPENAL_LIBRARY_DIRS )
    SET(OPENAL_LIBRARY_SEARCH_DIR "Please provide openal library path." CACHE PATH "path to openal libraries.")
    MESSAGE(FATAL_ERROR "find_openal.cmake: unable to find openal libraries.")
  ELSE ( NOT OPENAL_LIBRARY_DIRS )
    UNSET(OPENAL_LIBRARY_SEARCH_DIR CACHE)
  ENDIF ( NOT OPENAL_LIBRARY_DIRS ) 

ENDMACRO (request_openal_search_directories)

##############################################################################
# search
##############################################################################
message(STATUS "-- checking for OPENAL")

IF (NOT OPENAL_INCLUDE_DIRS)

  SET(_OPENAL_FOUND_INC_DIRS "")
  FOREACH(_SEARCH_DIR ${OPENAL_INCLUDE_SEARCH_DIRS})
    FIND_PATH(_CUR_SEARCH
      NAMES "AL/al.h"
        PATHS ${_SEARCH_DIR}
        NO_DEFAULT_PATH)
    IF (_CUR_SEARCH)
      LIST(APPEND _OPENAL_FOUND_INC_DIRS ${_CUR_SEARCH})
    ENDIF(_CUR_SEARCH)
    SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
  ENDFOREACH(_SEARCH_DIR ${OPENAL_INCLUDE_SEARCH_DIRS})

  IF (NOT _OPENAL_FOUND_INC_DIRS)
    request_openal_search_directories()
  ENDIF (NOT _OPENAL_FOUND_INC_DIRS)

  FOREACH(_INC_DIR ${_OPENAL_FOUND_INC_DIRS})
    SET(OPENAL_INCLUDE_DIRS ${OPENAL_INCLUDE_DIRS} ${_INC_DIR} CACHE PATH "openal include directory.")
  ENDFOREACH(_INC_DIR ${_OPENAL_FOUND_INC_DIRS})

ENDIF (NOT OPENAL_INCLUDE_DIRS)

IF(UNIX)
  SET(OPENAL_LIB_FILENAME "libopenal.so")
ELSEIF(WIN32)
  SET(OPENAL_LIB_FILENAME "libopenal.lib")
ENDIF(UNIX)

IF ( NOT OPENAL_LIBRARY_DIRS )

  SET(_OPENAL_FOUND_LIB_DIR "")
  SET(_OPENAL_POSTFIX "")

  FOREACH(_SEARCH_DIR ${OPENAL_LIBRARY_SEARCH_DIRS})
    FIND_PATH(_CUR_SEARCH
      NAMES ${OPENAL_LIB_FILENAME}
        PATHS ${_SEARCH_DIR}
        NO_DEFAULT_PATH)
    IF (_CUR_SEARCH)
      LIST(APPEND _OPENAL_FOUND_LIB_DIR ${_SEARCH_DIR})
    ENDIF(_CUR_SEARCH)
    SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
  ENDFOREACH(_SEARCH_DIR ${OPENAL_LIBRARY_SEARCH_DIRS})

  IF (NOT _OPENAL_FOUND_LIB_DIR)
    request_openal_search_directories()
  ELSE (NOT _OPENAL_FOUND_LIB_DIR)
    SET(OPENAL_LIBRARY_DIRS ${_OPENAL_FOUND_LIB_DIR} CACHE PATH "The openal library directory")
  ENDIF (NOT _OPENAL_FOUND_LIB_DIR)

  FOREACH(_LIB_DIR ${_OPENAL_FOUND_LIB_DIR})
    LIST(APPEND _OPENAL_LIBRARIES ${OPENAL_LIB_FILENAME})
  ENDFOREACH(_LIB_DIR ${_OPENAL_FOUND_INC_DIRS})

  IF (_OPENAL_FOUND_LIB_DIR)
    SET(OPENAL_LIBRARIES ${_OPENAL_LIBRARIES} CACHE FILEPATH "The openal library filename.")
  ENDIF (_OPENAL_FOUND_LIB_DIR)

ENDIF ( NOT OPENAL_LIBRARY_DIRS )

##############################################################################
# verify
##############################################################################
IF ( NOT OPENAL_INCLUDE_DIRS OR NOT OPENAL_LIBRARY_DIRS )
  request_openal_search_directories()
ELSE ( NOT OPENAL_INCLUDE_DIRS OR NOT OPENAL_LIBRARY_DIRS ) 
  UNSET(OPENAL_INCLUDE_SEARCH_DIR CACHE)
  UNSET(OPENAL_LIBRARY_SEARCH_DIR CACHE)
  MESSAGE(STATUS "--  found matching openal version")
ENDIF ( NOT OPENAL_INCLUDE_DIRS OR NOT OPENAL_LIBRARY_DIRS )
