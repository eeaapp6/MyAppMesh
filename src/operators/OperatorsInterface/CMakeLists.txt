add_library(appmesh_operators_interface STATIC
    ErrorInfo.cpp
    IOperator.cpp
    OperatorResult.cpp
    Task.cpp)

target_include_directories(appmesh_operators_interface
    PUBLIC "${PROJECT_SOURCE_DIR}/src")

target_link_libraries(appmesh_operators_interface
    PUBLIC Qt5::Core)
