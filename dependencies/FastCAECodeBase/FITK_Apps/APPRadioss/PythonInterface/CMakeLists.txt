add_subdirectory(PythonInterfaceGlobal)
add_subdirectory(PythonInterfaceProperties)
add_subdirectory(PythonInterfaceSolution)

# 将 PythonInterface 下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	set(_PYTHONINTERFACE_TARGETS
		PythonInterfaceGlobal
		PythonInterfaceProperties
		PythonInterfaceSolution
	)

	foreach(_target IN LISTS _PYTHONINTERFACE_TARGETS)
		if(TARGET ${_target})
			set_target_properties(${_target} PROPERTIES FOLDER "PythonInterface")
		endif()
	endforeach()
endif()