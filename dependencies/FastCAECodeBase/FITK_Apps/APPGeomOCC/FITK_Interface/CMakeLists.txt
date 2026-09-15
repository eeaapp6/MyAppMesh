add_subdirectory(FITKInterfaceModel)
add_subdirectory(FITKInterfaceIO)
add_subdirectory(FITKInterfaceGeometry)
add_subdirectory(FITKInterfaceGeoIOHDF5)
add_subdirectory(FITKInterfacePhysics)

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
    set(_FITK_INTERFACE_TARGETS
        FITKInterfaceModel
        FITKInterfaceIO
        FITKInterfaceGeometry
        FITKInterfaceGeoIOHDF5
        FITKInterfacePhysics
    )

    foreach(_target IN LISTS _FITK_INTERFACE_TARGETS)
        if(TARGET ${_target})
            set_target_properties(${_target} PROPERTIES FOLDER "FITK_Interface")
        endif()
    endforeach()
endif()
