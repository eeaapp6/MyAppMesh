add_subdirectory(FITKInterfaceGeoIOHDF5)
add_subdirectory(FITKInterfaceGeometry)
add_subdirectory(FITKInterfaceIO)
add_subdirectory(FITKInterfaceMesh)
add_subdirectory(FITKInterfaceMeshGen)
add_subdirectory(FITKInterfaceModel)
add_subdirectory(FITKInterfacePhysics)
#add_subdirectory(FITKInterfaceStructural)
add_subdirectory(FITKInterfaceStructuralPost)
add_subdirectory(FITKVTKAlgorithm)

# 将 FITK_Interface 下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	set(_FITK_INTERFACE_TARGETS
		FITKInterfaceGeoIOHDF5
		FITKInterfaceGeometry
		FITKInterfaceIO
		FITKInterfaceMesh
		FITKInterfaceMeshGen
		FITKInterfaceModel
		FITKInterfacePhysics
#		FITKInterfaceStructural
		FITKInterfaceStructuralPost
		FITKVTKAlgorithm
	)

	foreach(_target IN LISTS _FITK_INTERFACE_TARGETS)
		if(TARGET ${_target})
			set_target_properties(${_target} PROPERTIES FOLDER "FITK_Interface")
		endif()
	endforeach()
endif()
