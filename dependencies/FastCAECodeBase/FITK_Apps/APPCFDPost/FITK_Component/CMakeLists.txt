# 根据目录下的pro文件或者实际目录架构添加子模块
add_subdirectory(FITKWidget)
add_subdirectory(FITKRenderWindowVTK)
add_subdirectory(FITKPlotWindow)
add_subdirectory(FITKHttpPythonDriver)
add_subdirectory(FITKAIAssistant)

# 将目录下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    
    set(_FITK_COMPONENT_TARGETS
        FITKWidget
        FITKRenderWindowVTK
        FITKPlotWindow
        FITKHttpPythonDriver
        FITKAIAssistant
    )

    foreach(_target IN LISTS _FITK_COMPONENT_TARGETS)
        if(TARGET ${_target})
            set_target_properties(${_target} PROPERTIES FOLDER "FITK_Component")
        endif()
    endforeach()
endif()