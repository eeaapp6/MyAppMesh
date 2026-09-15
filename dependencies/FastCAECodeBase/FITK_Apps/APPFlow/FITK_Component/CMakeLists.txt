add_subdirectory(FITKGeoCompOCC)
add_subdirectory(FITKRenderWindowVTK)
add_subdirectory(FITKOFDriver)
add_subdirectory(FITKOFDictWriter)
add_subdirectory(FITKOFMeshIO)
add_subdirectory(FITKMeshGenOF)
add_subdirectory(FITKWidget)
add_subdirectory(FITKCompMessageWidget)
add_subdirectory(FITKFluidVTKGraphAdaptor)
add_subdirectory(FITKGeoOCCIOHDF5)
add_subdirectory(FITKFlowOFIOHDF5)
add_subdirectory(FITKAIAssistant)

# 将 FITK_Component 下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	set(_FITK_COMPONENT_TARGETS
		FITKGeoCompOCC
		FITKRenderWindowVTK
		FITKMeshGenOF
		FITKWidget
		FITKCompMessageWidget
		FITKFluidVTKGraphAdaptor
		FITKOFDictWriter
		FITKOFDriver
		FITKOFMeshIO
		FITKGeoOCCIOHDF5
		FITKFlowOFIOHDF5
		FITKAIAssistant
	)

	foreach(_target IN LISTS _FITK_COMPONENT_TARGETS)
		if(TARGET ${_target})
			set_target_properties(${_target} PROPERTIES FOLDER "FITK_Component")
		endif()
	endforeach()
endif()