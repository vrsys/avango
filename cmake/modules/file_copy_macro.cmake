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
    GET_FILENAME_COMPONENT(file_name ${in_file} NAME)
    COPY_FILE(${in_file} ${out_dir}/${file_name}
${target})
ENDMACRO(COPY_FILE_INTO_DIRECTORY)

# Copies all the files from in_file_list into the out_dir.
# sub-trees are ignored (files are stored in same out_dir)
MACRO(COPY_FILES_INTO_DIRECTORY in_file_list out_dir target)
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
    FILE(GLOB PYTHON_FILES ${in_dir}/*.py)
    FOREACH(in_file ${PYTHON_FILES})
        COPY_FILE_INTO_DIRECTORY(
            ${in_file}
            ${out_dir}
            ${target}
        )
    ENDFOREACH(in_file)
ENDMACRO(COPY_PYTHON_FILES_INTO_DIRECTORY)
