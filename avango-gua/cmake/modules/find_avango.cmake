SET(AV_INCLUDE_SEARCH_DIRS
    /opt/avango/current/include
)

SET(AV_LIBRARY_SEARCH_DIRS
    /opt/avango/current/lib
)

message("-- checking for AVANGO")

IF (NOT AV_INCLUDE_DIRS)

    SET(_AV_FOUND_INC_DIRS "")

    FOREACH(_SEARCH_DIR ${AV_INCLUDE_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
                NAMES avango/MultiField.h
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _AV_FOUND_INC_DIRS ${_CUR_SEARCH})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${AV_INCLUDE_SEARCH_DIRS})

    IF (NOT _AV_FOUND_INC_DIRS)
        MESSAGE(FATAL_ERROR "find_avango.cmake: unable to find avango headers")
    ENDIF (NOT _AV_FOUND_INC_DIRS)
    
    FOREACH(_INC_DIR ${_AV_FOUND_INC_DIRS})
        LIST(APPEND AV_INCLUDE_DIRS ${_INC_DIR})
    ENDFOREACH(_INC_DIR ${_BOOST_FOUND_INC_DIRS})

ENDIF(NOT AV_INCLUDE_DIRS)

IF (        AV_INCLUDE_DIRS
    AND NOT AV_LIBRARIES)

    SET(_AV_FOUND_LIB_DIR "")
    SET(_AV_POSTFIX "")

    FOREACH(_SEARCH_DIR ${AV_LIBRARY_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
                NAMES libavango.so
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _AV_FOUND_LIB_DIR ${_SEARCH_DIR})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${AV_LIBRARY_SEARCH_DIRS})

    IF (NOT _AV_FOUND_LIB_DIR)
        MESSAGE(FATAL_ERROR "find_avango.cmake: unable to find avango library")
    ELSE (NOT _AV_FOUND_LIB_DIR)
        message("--  found matching version")
    ENDIF (NOT _AV_FOUND_LIB_DIR)
    
    FOREACH(_LIB_DIR ${_AV_FOUND_LIB_DIR})
        LIST(APPEND AV_LIBRARIES ${_LIB_DIR}/libavango.so)
        #LIST(APPEND AV_LIBRARIES ${_LIB_DIR}/libavango.so ${_LIB_DIR}/libavangoEnsemble.so)
    ENDFOREACH(_LIB_DIR ${_AV_FOUND_INC_DIRS})
    

ENDIF(        AV_INCLUDE_DIRS
      AND NOT AV_LIBRARIES)



