include_guard(GLOBAL)

function(appmesh_configure_output_directories)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG
        "${CMAKE_BINARY_DIR}/runtime/Debug" PARENT_SCOPE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG
        "${CMAKE_BINARY_DIR}/runtime/Debug" PARENT_SCOPE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG
        "${CMAKE_BINARY_DIR}/lib/Debug" PARENT_SCOPE)
    set(CMAKE_PDB_OUTPUT_DIRECTORY_DEBUG
        "${CMAKE_BINARY_DIR}/symbols/Debug" PARENT_SCOPE)
endfunction()

function(appmesh_stage_runtime target)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            "$<TARGET_FILE:APPMesh::HDF5>"
            "$<TARGET_FILE_DIR:${target}>"
        COMMENT "Staging Debug-only FastCAE Tools runtime")
endfunction()
