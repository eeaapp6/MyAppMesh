add_library(appmesh_app STATIC
    AppDiagnostic.h
    SystemChecker.h
    SystemChecker.cpp
    MeshAPPSettings.h
    MeshAPPSettings.cpp
    GlobalDataFactory.h
    GlobalDataFactory.cpp
    ComponentFactory.h
    ComponentFactory.cpp)

target_include_directories(appmesh_app PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_link_libraries(appmesh_app PUBLIC Qt5::Core)

add_executable(appmesh WIN32 main.cpp)

target_link_libraries(appmesh PRIVATE
    appmesh_app
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    APPMesh::HDF5)

target_compile_definitions(appmesh PRIVATE
    APPMESH_VERSION="${PROJECT_VERSION}"
    APPMESH_BUILD_CONFIG="$<CONFIG>")

set_target_properties(appmesh PROPERTIES
    OUTPUT_NAME APPMesh
    VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:appmesh>")

appmesh_stage_runtime(appmesh)
