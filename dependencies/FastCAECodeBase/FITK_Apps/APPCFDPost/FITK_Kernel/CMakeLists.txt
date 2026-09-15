# 根据目录下的pro文件或者实际目录架构添加子模块
add_subdirectory(FITKCore)
add_subdirectory(FITKEasyParam)
add_subdirectory(FITKAppFramework)
add_subdirectory(FITKPython)

# 将目录下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    # 名称使用 FITK_KERNEL
    set(_FITK_KERNEL_TARGETS
        FITKCore
        FITKEasyParam
        FITKAppFramework
        FITKPython
    )

    foreach(_target IN LISTS _FITK_KERNEL_TARGETS)
        if(TARGET ${_target})
            set_target_properties(${_target} PROPERTIES FOLDER "FITK_Kernel")
        endif()
    endforeach()
endif()