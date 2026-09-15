add_subdirectory(FITKWidget)
add_subdirectory(FITKCompMessageWidget)
add_subdirectory(FITKGeoCompOCC)
add_subdirectory(FITKRenderWindowOCC)
add_subdirectory(FITKOCCGraphAdaptor)
add_subdirectory(FITKGeoOCCIOHDF5)
add_subdirectory(FITKAIAssistant)
add_subdirectory(FITKHttpPythonDriver)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(_FITK_COMPONENT_TARGETS
        FITKWidget
        FITKCompMessageWidget
        FITKGeoCompOCC
        FITKRenderWindowOCC
        FITKOCCGraphAdaptor
        FITKGeoOCCIOHDF5
        FITKAIAssistant
        FITKHttpPythonDriver
    )

    foreach(_target IN LISTS _FITK_COMPONENT_TARGETS)
        if(TARGET ${_target})
            set_target_properties(${_target} PROPERTIES FOLDER "FITK_Component")
        endif()
    endforeach()
endif()
