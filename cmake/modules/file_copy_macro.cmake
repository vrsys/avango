MACRO (ADD_PARAMETER parameter input)
    SET(tmp "${parameter} ${input}")
    MESSAGE(${parameter})
    SET(${parameter} ${tmp})
ENDMACRO(ADD_PARAMETER)

# copies in_file to out_file for target
MACRO(COPY_FILE in_file out_file target)
    IF(${in_file} IS_NEWER_THAN ${out_file})
        ADD_CUSTOM_COMMAND (
            TARGET     ${target}
            POST_BUILD
            COMMAND    ${CMAKE_COMMAND}
            ARGS       -E copy ${in_file} ${out_file}
        )
    ENDIF(${in_file} IS_NEWER_THAN ${out_file})
ENDMACRO(COPY_FILE)

# copies in_file to out_dir for target
MACRO(COPY_FILE_INTO_DIRECTORY in_file out_dir target)
    FILE(MAKE_DIRECTORY ${out_dir})
    GET_FILENAME_COMPONENT(file_name ${in_file} NAME)
    COPY_FILE(${in_file} ${out_dir}/${file_name}
${target})
ENDMACRO(COPY_FILE_INTO_DIRECTORY)

# Copies all the files from in_file_list into the out_dir.
# sub-trees are ignored (files are stored in same out_dir)
MACRO(COPY_FILES_INTO_DIRECTORY in_file_list out_dir target)
    FILE(MAKE_DIRECTORY ${out_dir})
    FOREACH(in_file ${in_file_list})
        COPY_FILE_INTO_DIRECTORY(
            ${in_file}
            ${out_dir}
            ${target}
        )
    ENDFOREACH(in_file)
ENDMACRO(COPY_FILES_INTO_DIRECTORY)

# Copies all python files from in_dir into the out_dir.
# sub-trees are ignored (files are stored in same out_dir)
MACRO(COPY_PYTHON_FILES_INTO_DIRECTORY in_dir out_dir target)
    FILE(MAKE_DIRECTORY ${out_dir})
    FILE(GLOB PYTHON_FILES ${in_dir}/*.py)
    FOREACH(in_file ${PYTHON_FILES})
        COPY_FILE_INTO_DIRECTORY(
            ${in_file}
            ${out_dir}
            ${target}
        )
    ENDFOREACH(in_file)
ENDMACRO(COPY_PYTHON_FILES_INTO_DIRECTORY)

# Copies output of target to file
MACRO ( post_build_install_target cmake_target out_file )

  GET_PROPERTY(_SOURCE_PATH TARGET ${cmake_target} PROPERTY LOCATION)
  FILE ( TO_NATIVE_PATH ${_SOURCE_PATH} _SOURCE_PATH )

  GET_FILENAME_COMPONENT(_TARGET_DIRECTORY ${out_file} PATH)

  FILE ( TO_NATIVE_PATH ${_TARGET_DIRECTORY} _TARGET_DIRECTORY )
  FILE ( TO_NATIVE_PATH ${out_file} _TARGET_PATH )

  IF (WIN32)
    ADD_CUSTOM_COMMAND(TARGET ${cmake_target}
                       POST_BUILD
                       COMMAND IF exist ${_TARGET_DIRECTORY} ( copy /Y ${_SOURCE_PATH} ${_TARGET_PATH}) ELSE ( mkdir ${_TARGET_DIRECTORY} &&  copy /Y ${_SOURCE_PATH} ${_TARGET_PATH})
                       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  ELSEIF (UNIX)
    ADD_CUSTOM_COMMAND(TARGET ${cmake_target}
                       POST_BUILD
                       COMMAND mkdir -p ${_TARGET_DIRECTORY}
                       COMMAND cp ${_SOURCE_PATH} ${_TARGET_PATH}
                       WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
  ENDIF (WIN32)

ENDMACRO (post_build_install_target cmake_target out_file)


