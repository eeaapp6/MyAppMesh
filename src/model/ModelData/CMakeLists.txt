add_library(appmesh_model STATIC
    DataObject.h
    DataObject.cpp
    ApplicationRuntime.h
    ApplicationRuntime.cpp
    GeometryObject.h
    GeometryObject.cpp
    GeometryManager.h
    GeometryManager.cpp
    MeshTypes.h
    MeshTypes.cpp
    MeshKernel.h
    MeshKernel.cpp
    MeshData.h
    MeshData.cpp
    MeshDataCreator.h
    MeshDataCreator.cpp
    MeshManager.h
    MeshManager.cpp)

target_include_directories(appmesh_model PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(appmesh_model PUBLIC Qt5::Core)
