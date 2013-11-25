SET(GUA_INCLUDE_SEARCH_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/../guacamole/include
)

SET(GUA_LIBRARY_SEARCH_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/../guacamole/lib
)

MESSAGE("-- checking for GUACAMOLE")

IF (NOT GUA_INCLUDE_DIRS)

    FIND_PATH(_CUR_SEARCH
        NAMES gua/guacamole.hpp
        PATHS ${GUA_INCLUDE_SEARCH_DIR}
        NO_DEFAULT_PATH)


    IF (_CUR_SEARCH)
        GET_FILENAME_COMPONENT(_INC ${GUA_INCLUDE_SEARCH_DIR} ABSOLUTE)
        SET(GUA_INCLUDE_DIR ${_INC} CACHE FILEPATH "The include directory of guacamole")
        LIST(APPEND GUA_INCLUDE_DIRS ${_INC})
    ELSE (_CUR_SEARCH)
        MESSAGE(FATAL_ERROR "find_guacamole.cmake: unable to find guacamole headers")
    ENDIF(_CUR_SEARCH)

    SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")

ENDIF(NOT GUA_INCLUDE_DIRS)

IF (        GUA_INCLUDE_DIRS
    AND NOT GUA_LIBRARIES)

    FIND_PATH(_CUR_SEARCH
        NAMES libguacamole.so
        PATHS ${GUA_LIBRARY_SEARCH_DIR}
        NO_DEFAULT_PATH)


    IF (_CUR_SEARCH)
        MESSAGE("--  found matching version")
        GET_FILENAME_COMPONENT(_LIB ${GUA_LIBRARY_SEARCH_DIR}/libguacamole.so ABSOLUTE)
        SET(GUA_LIBRARY ${_LIB} CACHE FILEPATH "The library of guacamole")
        LIST(APPEND GUA_LIBRARIES ${_LIB})
    ELSE (_CUR_SEARCH)
        MESSAGE(FATAL_ERROR "find_guacamole.cmake: unable to find guacamole library")
    ENDIF(_CUR_SEARCH)

    SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")

ENDIF(        GUA_INCLUDE_DIRS
      AND NOT GUA_LIBRARIES)
