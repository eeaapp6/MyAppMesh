include_guard(GLOBAL)

function(appmesh_configure_output_directories)
    foreach(_config Debug Release RelWithDebInfo MinSizeRel)
        string(TOUPPER "${_config}" _config_upper)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${_config_upper}
            "${CMAKE_BINARY_DIR}/runtime/${_config}" PARENT_SCOPE)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${_config_upper}
            "${CMAKE_BINARY_DIR}/runtime/${_config}" PARENT_SCOPE)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${_config_upper}
            "${CMAKE_BINARY_DIR}/lib/${_config}" PARENT_SCOPE)
        set(CMAKE_PDB_OUTPUT_DIRECTORY_${_config_upper}
            "${CMAKE_BINARY_DIR}/symbols/${_config}" PARENT_SCOPE)
    endforeach()
endfunction()

function(appmesh_stage_runtime target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:APPMesh::HDF5>"
            "$<TARGET_FILE_DIR:${target}>"
        COMMENT "Staging configuration-matched FastCAE Tools runtime")
endfunction()
