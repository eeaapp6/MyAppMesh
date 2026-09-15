# 根据目录下的pro文件或者实际目录架构添加子模块
add_subdirectory(FITKVTKAlgorithm)
add_subdirectory(FITKInterfaceCFDPost)

# 将目录下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    
    set(_FITK_INTERFACE_TARGETS
        FITKVTKAlgorithm
        FITKInterfaceCFDPost
    )

    foreach(_target IN LISTS _FITK_INTERFACE_TARGETS)
        if(TARGET ${_target})
            set_target_properties(${_target} PROPERTIES FOLDER "FITK_Interface")
        endif()
    endforeach()
endif()