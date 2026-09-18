add_library(appmesh_geometry_io STATIC
    GeometryIO/GeometryIO.h
    GeometryIO/GeometryIO.cpp)

target_include_directories(appmesh_geometry_io PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(appmesh_geometry_io
    PUBLIC appmesh_operators_model Qt5::Core
    PRIVATE APPMesh::FITKGeoCompOCC APPMesh::FITKInterfaceGeometry APPMesh::FITKInterfaceModel)
