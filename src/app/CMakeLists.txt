add_library(appmesh_app STATIC
    AppDiagnostic.h
    SystemChecker.h
    SystemChecker.cpp
    MeshAPPSettings.h
    MeshAPPSettings.cpp
    GlobalDataFactory.h
    GlobalDataFactory.cpp
    ComponentFactory.h
    ComponentFactory.cpp
    ApplicationBoundaries.h
    ApplicationBoundaries.cpp
    FastCAERegistrationAdapter.h
    FastCAERegistrationAdapter.cpp
    FITKFastCAERegistrationAdapter.h
    FITKFastCAERegistrationAdapter.cpp
    FITKComponentMapping.h
    FITKComponentMapping.cpp
    MeshApp.h
    MeshApp.cpp
    MainWindowGenerator.h
    MainWindowGenerator.cpp
    AppInitializer.h
    AppInitializer.cpp
    PreWindowInitializer.h
    PreWindowInitializer.cpp
    SignalProcessor.h
    SignalProcessor.cpp
    PyRegister.h
    PyRegister.cpp)

target_include_directories(appmesh_app PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(appmesh_app
    PUBLIC
        appmesh_gui
        appmesh_model
        appmesh_operators_gui
        appmesh_operators_fitk
        appmesh_geometry_io
        Qt5::Core
        Qt5::Widgets
        APPMesh::FITKAppFramework
        APPMesh::FITKCore)

add_executable(appmesh WIN32 main.cpp)

target_link_libraries(appmesh PRIVATE
    appmesh_app
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    APPMesh::HDF5)

target_compile_definitions(appmesh PRIVATE
    APPMESH_VERSION="${PROJECT_VERSION}"
    APPMESH_BUILD_CONFIG="$<CONFIG>"
    APPMESH_TOOLS_DIR="${TOOLS_DIR}")

set_target_properties(appmesh PROPERTIES
    OUTPUT_NAME APPMesh
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:appmesh>")

appmesh_stage_runtime(appmesh)
appmesh_stage_fitk_runtime(appmesh)
