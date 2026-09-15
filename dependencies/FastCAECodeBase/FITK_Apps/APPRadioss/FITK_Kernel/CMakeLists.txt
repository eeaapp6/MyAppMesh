add_subdirectory(FITKCore)
add_subdirectory(FITKAppFramework)
add_subdirectory(FITKAdaptor)
add_subdirectory(FITKPython)


# 将 FITK_Kernel 下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	set(_FITK_KERNEL_TARGETS
		FITKCore
		FITKAppFramework
		FITKAdaptor
		FITKPython

	)

	foreach(_target IN LISTS _FITK_KERNEL_TARGETS)
		if(TARGET ${_target})
			set_target_properties(${_target} PROPERTIES FOLDER "FITK_Kernel")
		endif()
	endforeach()
endif()
