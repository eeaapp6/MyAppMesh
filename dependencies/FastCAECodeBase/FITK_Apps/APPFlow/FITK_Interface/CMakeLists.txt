add_subdirectory(FITKInterfaceModel)
add_subdirectory(FITKVTKAlgorithm)
add_subdirectory(FITKInterfaceMesh)
add_subdirectory(FITKInterfacePhysics)
add_subdirectory(FITKInterfaceIO)
add_subdirectory(FITKInterfaceGeometry)
add_subdirectory(FITKInterfaceFlowOF)
add_subdirectory(FITKInterfaceMeshGen)
add_subdirectory(FITKInterfaceGeoIOHDF5)

# 将 FITK_Interface 下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	set(_FITK_INTERFACE_TARGETS
		FITKInterfaceMesh
		FITKInterfaceModel
		FITKInterfacePhysics
		FITKInterfaceIO
		FITKInterfaceFlowOF
		FITKInterfaceMeshGen
		FITKInterfaceGeometry
		FITKVTKAlgorithm
		FITKInterfaceGeoIOHDF5
	)

	foreach(_target IN LISTS _FITK_INTERFACE_TARGETS)
		if(TARGET ${_target})
			set_target_properties(${_target} PROPERTIES FOLDER "FITK_Interface")
		endif()
	endforeach()
endif()