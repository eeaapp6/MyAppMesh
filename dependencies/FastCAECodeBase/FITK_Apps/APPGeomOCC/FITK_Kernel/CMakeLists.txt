add_subdirectory(FITKCore)
add_subdirectory(FITKAppFramework)
add_subdirectory(FITKPython)
add_subdirectory(FITKAdaptor)
add_subdirectory(FITKEasyParam)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(_FITK_KERNEL_TARGETS
        FITKCore
        FITKAppFramework
        FITKPython
        FITKAdaptor
        FITKEasyParam
    )

    foreach(_target IN LISTS _FITK_KERNEL_TARGETS)
        if(TARGET ${_target})
            set_target_properties(${_target} PROPERTIES FOLDER "FITK_Kernel")
        endif()
    endforeach()
endif()
