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

add_test(NAME t001.debug-only-policy
    COMMAND "${CMAKE_COMMAND}"
        "-DSOURCE_DIR=${CMAKE_SOURCE_DIR}"
        "-DBINARY_DIR=${CMAKE_BINARY_DIR}"
        "-DRUNTIME_DIR=$<TARGET_FILE_DIR:appmesh>"
        "-DCONFIG=$<CONFIG>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/VerifyDebugOnlyPolicy.cmake")
set_tests_properties(t001.debug-only-policy PROPERTIES DEPENDS t001.runtime-layout)

add_test(NAME t001.debug-binary-dependencies
    COMMAND "${CMAKE_COMMAND}"
        "-DDUMPBIN_EXECUTABLE=${APPMESH_DUMPBIN_EXECUTABLE}"
        "-DAPP_EXECUTABLE=$<TARGET_FILE:appmesh>"
        "-DCONFIG=$<CONFIG>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/VerifyDebugBinaryDependencies.cmake")
set_tests_properties(t001.debug-binary-dependencies PROPERTIES
    DEPENDS t001.debug-only-policy)

function(appmesh_add_unit_test target source)
    add_executable(${target} "${source}")
    target_link_libraries(${target} PRIVATE appmesh_app Qt5::Core)
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME ${target} COMMAND "$<TARGET_FILE:${target}>")
endfunction()

function(appmesh_add_model_test target source)
    add_executable(${target} "${source}")
    target_link_libraries(${target} PRIVATE appmesh_model Qt5::Core)
    target_include_directories(${target} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    add_test(NAME ${target} COMMAND "$<TARGET_FILE:${target}>")
endfunction()

appmesh_add_unit_test(t002.system-checker unit/app/SystemCheckerTests.cpp)
target_compile_definitions(t002.system-checker PRIVATE
    APPMESH_TEST_TOOLS_DIR="${TOOLS_DIR}"
    APPMESH_TEST_QT_ROOT="$<TARGET_FILE_DIR:Qt5::Core>/..")

appmesh_add_unit_test(t002.settings unit/app/MeshAPPSettingsTests.cpp)
appmesh_add_unit_test(t003.factory-lifecycle unit/app/FactoryLifecycleTests.cpp)
appmesh_add_unit_test(t004.mesh-app-lifecycle unit/app/MeshAppLifecycleTests.cpp)
set_tests_properties(t004.mesh-app-lifecycle PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

appmesh_add_unit_test(t041.fitk-component-mapping unit/app/FITKComponentMappingTests.cpp)
appmesh_add_unit_test(t041.fitk-application-integration
    integration/app/FITKApplicationIntegrationTests.cpp)
set_tests_properties(t041.fitk-application-integration PROPERTIES
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

appmesh_add_model_test(t005.data-object unit/model/DataObjectTests.cpp)
appmesh_add_model_test(t005.application-runtime unit/model/ApplicationRuntimeTests.cpp)
appmesh_add_model_test(t005.application-runtime-concurrency
    unit/model/ApplicationRuntimeConcurrencyTests.cpp)
appmesh_add_unit_test(t005.runtime-integration
    integration/app/ApplicationRuntimeIntegrationTests.cpp)

appmesh_add_model_test(t006.geometry-manager
    unit/model/GeometryManagerTests.cpp)
appmesh_add_model_test(t007.mesh-data
    unit/model/MeshDataTests.cpp)
appmesh_add_model_test(t008.mesh-manager
    unit/model/MeshManagerTests.cpp)

appmesh_add_model_test(t009.runtime-protocol
    unit/model/T009RuntimeProtocolTests.cpp)
appmesh_add_model_test(t009.geometry-concurrency
    unit/model/T009GeometryConcurrencyTests.cpp)
appmesh_add_model_test(t009.mesh-validation-matrix
    unit/model/T009MeshValidationMatrixTests.cpp)
appmesh_add_model_test(t009.mesh-manager-concurrency
    unit/model/T009MeshManagerConcurrencyTests.cpp)

appmesh_add_unit_test(t010.main-window
    unit/gui/MainWindowTests.cpp)
appmesh_add_unit_test(t011.model-widgets
    unit/gui/ModelWidgetsTests.cpp)
set_tests_properties(
    t010.main-window
    t011.model-widgets
    PROPERTIES ENVIRONMENT "QT_QPA_PLATFORM=offscreen")

set_tests_properties(t009.mesh-validation-matrix PROPERTIES TIMEOUT 30)
set_tests_properties(
    t009.runtime-protocol
    t009.geometry-concurrency
    t009.mesh-manager-concurrency
    PROPERTIES TIMEOUT 60)

add_test(NAME t041.fitk-debug-dependencies
    COMMAND "${CMAKE_COMMAND}"
        "-DDUMPBIN_EXECUTABLE=${APPMESH_DUMPBIN_EXECUTABLE}"
        "-DAPP_EXECUTABLE=$<TARGET_FILE:appmesh>"
        "-DRUNTIME_DIR=$<TARGET_FILE_DIR:appmesh>"
        "-DFITK_OUTPUT_DIR=${FITK_OUTPUT_DIR}"
        "-DSOURCE_DIR=${CMAKE_SOURCE_DIR}"
        "-DCONFIG=$<CONFIG>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/VerifyFITKDebugDependencies.cmake")
set_tests_properties(t041.fitk-debug-dependencies PROPERTIES
    DEPENDS t001.debug-binary-dependencies)

add_test(NAME t041.fitk-smoke
    COMMAND "${CMAKE_COMMAND}"
        "-DCMAKE_COMMAND_PATH=${CMAKE_COMMAND}"
        "-DAPP_EXECUTABLE=$<TARGET_FILE:appmesh>"
        "-DRUNTIME_DIR=$<TARGET_FILE_DIR:appmesh>"
        "-DQT_BIN_DIR=$<TARGET_FILE_DIR:Qt5::Core>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/VerifyFITKSmoke.cmake")
set_tests_properties(t041.fitk-smoke PROPERTIES
    DEPENDS t041.fitk-debug-dependencies)
