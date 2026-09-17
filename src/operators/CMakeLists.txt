add_subdirectory(OperatorsInterface)

add_library(appmesh_operators_model STATIC
    OperatorsModel/ITaskExecutor.h
    OperatorsModel/TaskService.h
    OperatorsModel/TaskService.cpp
    OperatorsModel/GeometryReader.h
    OperatorsModel/GeometryReader.cpp
    OperatorsModel/ImportGeometryOperator.h
    OperatorsModel/ImportGeometryOperator.cpp)

target_include_directories(appmesh_operators_model PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(appmesh_operators_model
    PUBLIC
        appmesh_operators_interface
        appmesh_model
        Qt5::Core)

# Keep the FITK/Qt Widgets ABI dependency outside OperatorsModel itself. The
# adapter uses the already allowlisted FITKCore task pool and introduces no
# second production thread pool.
add_library(appmesh_operators_fitk STATIC
    OperatorsModel/FITKTaskExecutor.h
    OperatorsModel/FITKTaskExecutor.cpp)

target_include_directories(appmesh_operators_fitk PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(appmesh_operators_fitk
    PUBLIC appmesh_operators_model
    PRIVATE APPMesh::FITKCore)

add_library(appmesh_operators_gui STATIC
    OperatorsGUI/GeometryImportController.h
    OperatorsGUI/GeometryImportController.cpp)
set_target_properties(appmesh_operators_gui PROPERTIES AUTOMOC ON)
target_include_directories(appmesh_operators_gui PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(appmesh_operators_gui
    PUBLIC
        appmesh_operators_model
        appmesh_gui
        Qt5::Core
        Qt5::Widgets)
