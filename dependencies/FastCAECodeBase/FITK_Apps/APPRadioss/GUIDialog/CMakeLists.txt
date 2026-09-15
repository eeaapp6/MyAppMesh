add_subdirectory(GUIAnalysisDialog)
add_subdirectory(GUIGeometryDialog)
add_subdirectory(GUIMeshDialog)
add_subdirectory(GUIPostDialog)
add_subdirectory(GUIToolDialog)

# 将 GUIDialog 下的几个工程在 VS 中归并到同一个 Solution Folder
if(CMAKE_GENERATOR MATCHES "Visual Studio")
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	set(_GUIDIALOG_TARGETS
		GUIAnalysisDialog
		GUIGeometryDialog
		GUIMeshDialog
		GUIPostDialog
		GUIToolDialog
	)

	foreach(_target IN LISTS _GUIDIALOG_TARGETS)
		if(TARGET ${_target})
			set_target_properties(${_target} PROPERTIES FOLDER "GUIDialog")
		endif()
	endforeach()
endif()
