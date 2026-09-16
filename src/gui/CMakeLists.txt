add_library(appmesh_gui STATIC
    GUIFrame/MainWindow.h
    GUIFrame/MainWindow.cpp
    GUIWidget/ModelTree.h
    GUIWidget/ModelTree.cpp
    GUIWidget/ConsoleWidget.h
    GUIWidget/ConsoleWidget.cpp)

set_target_properties(appmesh_gui PROPERTIES AUTOMOC ON)

target_include_directories(appmesh_gui PUBLIC "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(appmesh_gui
    PUBLIC
        appmesh_model
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets)
