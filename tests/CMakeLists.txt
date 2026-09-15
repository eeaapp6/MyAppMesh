add_test(NAME t001.bootstrap
    COMMAND "${CMAKE_COMMAND}" -E env
        "PATH=$<TARGET_FILE_DIR:Qt5::Core>;$ENV{PATH}"
        "$<TARGET_FILE:appmesh>" --smoke-test)

add_test(NAME t001.runtime-layout
    COMMAND "${CMAKE_COMMAND}"
        "-DRUNTIME_DIR=$<TARGET_FILE_DIR:appmesh>"
        "-DCONFIG=$<CONFIG>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/VerifyRuntimeLayout.cmake")

set_tests_properties(t001.runtime-layout PROPERTIES DEPENDS t001.bootstrap)

function(appmesh_add_unit_test target source)
    add_executable(${target} "${source}")
    target_link_libraries(${target} PRIVATE appmesh_app Qt5::Core)
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME ${target} COMMAND "$<TARGET_FILE:${target}>")
endfunction()

appmesh_add_unit_test(t002.system-checker unit/app/SystemCheckerTests.cpp)
target_compile_definitions(t002.system-checker PRIVATE
    APPMESH_TEST_TOOLS_DIR="${TOOLS_DIR}"
    APPMESH_TEST_QT_ROOT="$<TARGET_FILE_DIR:Qt5::Core>/..")

appmesh_add_unit_test(t002.settings unit/app/MeshAPPSettingsTests.cpp)
appmesh_add_unit_test(t003.factory-lifecycle unit/app/FactoryLifecycleTests.cpp)
