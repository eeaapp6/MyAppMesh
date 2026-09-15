#add_subdirectory(FITKAbaqusData)
#add_subdirectory(FITKAbaqusIOINP)
#add_subdirectory(FITKAbaqusRadiossDataMapper)
add_subdirectory(FITKCompMessageWidget)
add_subdirectory(FITKGeoCompOCC)
add_subdirectory(FITKGeoOCCIOHDF5)
add_subdirectory(FITKGmshExeDriver)
add_subdirectory(FITKGmshMshIO)
add_subdirectory(FITKAIAssistant)
add_subdirectory(FITKHttpPythonDriver)
add_subdirectory(FITKMeshIO)
#add_subdirectory(FITKNastranBDFIO)
add_subdirectory(FITKPlotWindow)
add_subdirectory(FITKRadiossData)
add_subdirectory(FITKRadiossDataIOHDF5)
add_subdirectory(FITKRadiossLSDynaTransfer)
add_subdirectory(FITKRadiossRadIO)
add_subdirectory(FITKRenderWindowVTK)
add_subdirectory(FITKWidget)

# 将 FITK_Component 下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	set(_FITK_COMPONENT_TARGETS
#		FITKAbaqusData
#		FITKAbaqusRadiossDataMapper
		FITKCompMessageWidget
		FITKGeoCompOCC
		FITKGeoOCCIOHDF5
		FITKGmshExeDriver
		FITKGmshMshIO
		FITKAIAssistant
		FITKHttpPythonDriver
		FITKMeshIO
#		FITKNastranBDFIO
		FITKPlotWindow
		FITKRadiossData
		FITKRadiossDataIOHDF5
		FITKRadiossLSDynaTransfer
		FITKRadiossRadIO
		FITKRenderWindowVTK
		FITKWidget
	)

	foreach(_target IN LISTS _FITK_COMPONENT_TARGETS)
		if(TARGET ${_target})
			set_target_properties(${_target} PROPERTIES FOLDER "FITK_Component")
		endif()
	endforeach()
endif()
