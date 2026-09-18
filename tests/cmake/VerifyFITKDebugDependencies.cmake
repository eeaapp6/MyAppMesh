if(NOT DEFINED DUMPBIN_EXECUTABLE OR NOT DEFINED APP_EXECUTABLE OR
   NOT DEFINED RUNTIME_DIR OR NOT DEFINED FITK_OUTPUT_DIR OR NOT DEFINED SOURCE_DIR OR
   NOT DEFINED CONFIG)
    message(FATAL_ERROR "T041 dependency validation arguments are incomplete")
endif()

if(NOT CONFIG STREQUAL "Debug")
    message(FATAL_ERROR "T041 rejects non-Debug configuration: ${CONFIG}")
endif()

set(_whitelist FITKAppFramework.dll FITKCore.dll FITKInterfaceModel.dll
    FITKInterfaceGeometry.dll FITKGeoCompOCC.dll)
foreach(_dll IN LISTS _whitelist)
    if(NOT EXISTS "${RUNTIME_DIR}/${_dll}")
        message(FATAL_ERROR "Whitelisted FITK Debug runtime is missing: ${RUNTIME_DIR}/${_dll}")
    endif()
    if(NOT EXISTS "${FITK_OUTPUT_DIR}/bin_d/${_dll}")
        message(FATAL_ERROR "Whitelisted FITK source runtime is missing: ${FITK_OUTPUT_DIR}/bin_d/${_dll}")
    endif()
endforeach()

file(GLOB _staged_fitk "${RUNTIME_DIR}/FITK*.dll")
list(SORT _staged_fitk)
set(_expected
    "${RUNTIME_DIR}/FITKAppFramework.dll"
    "${RUNTIME_DIR}/FITKCore.dll"
    "${RUNTIME_DIR}/FITKGeoCompOCC.dll"
    "${RUNTIME_DIR}/FITKInterfaceGeometry.dll"
    "${RUNTIME_DIR}/FITKInterfaceModel.dll")
list(SORT _expected)
if(NOT _staged_fitk STREQUAL _expected)
    message(FATAL_ERROR "Runtime contains FITK DLLs outside the T041 whitelist: ${_staged_fitk}")
endif()

execute_process(
    COMMAND "${DUMPBIN_EXECUTABLE}" /dependents "${APP_EXECUTABLE}"
    RESULT_VARIABLE _app_exit
    OUTPUT_VARIABLE _app_output
    ERROR_VARIABLE _app_error)
if(NOT _app_exit EQUAL 0)
    message(FATAL_ERROR "dumpbin failed for APPMesh.exe: ${_app_error}")
endif()
string(TOLOWER "${_app_output}" _app_dependencies)
foreach(_required IN ITEMS fitkappframework.dll fitkcore.dll fitkgeocompocc.dll
        fitkinterfacegeometry.dll hdf5_d.dll qt5cored.dll qt5widgetsd.dll)
    string(FIND "${_app_dependencies}" "${_required}" _position)
    if(_position EQUAL -1)
        message(FATAL_ERROR "APPMesh.exe is missing required Debug dependency: ${_required}")
    endif()
endforeach()

set(_forbidden_names
    meshapp.dll graphdata guiframe guiwidget guidialog modeldata operators geometryio hdf5io
    hdf5.dll qt5core.dll qt5gui.dll qt5widgets.dll)
foreach(_forbidden IN LISTS _forbidden_names)
    string(FIND "${_app_dependencies}" "${_forbidden}" _position)
    if(NOT _position EQUAL -1)
        message(FATAL_ERROR "APPMesh.exe depends on forbidden non-Debug or business DLL: ${_forbidden}")
    endif()
endforeach()

foreach(_dll IN LISTS _whitelist)
    execute_process(
        COMMAND "${DUMPBIN_EXECUTABLE}" /dependents "${FITK_OUTPUT_DIR}/bin_d/${_dll}"
        RESULT_VARIABLE _fitk_exit
        OUTPUT_VARIABLE _fitk_output
        ERROR_VARIABLE _fitk_error)
    if(NOT _fitk_exit EQUAL 0)
        message(FATAL_ERROR "dumpbin failed for ${_dll}: ${_fitk_error}")
    endif()
    string(TOLOWER "${_fitk_output}" _fitk_dependencies)
    foreach(_debug_runtime IN ITEMS vcruntime140d.dll ucrtbased.dll qt5cored.dll)
        string(FIND "${_fitk_dependencies}" "${_debug_runtime}" _position)
        if(_position EQUAL -1)
            message(FATAL_ERROR "${_dll} is missing required Debug ABI dependency: ${_debug_runtime}")
        endif()
    endforeach()
    if(_dll STREQUAL "FITKAppFramework.dll")
        foreach(_framework_dependency IN ITEMS fitkcore.dll qt5guid.dll qt5networkd.dll)
            string(FIND "${_fitk_dependencies}" "${_framework_dependency}" _position)
            if(_position EQUAL -1)
                message(FATAL_ERROR "FITKAppFramework.dll is missing audited dependency: ${_framework_dependency}")
            endif()
        endforeach()
    endif()
    foreach(_forbidden IN ITEMS meshapp.dll graphdata guiframe guiwidget guidialog modeldata operators geometryio hdf5io qt5core.dll qt5gui.dll qt5widgets.dll)
        string(FIND "${_fitk_dependencies}" "${_forbidden}" _position)
        if(NOT _position EQUAL -1)
            message(FATAL_ERROR "${_dll} depends on forbidden DLL: ${_forbidden}")
        endif()
    endforeach()
endforeach()

file(READ "${SOURCE_DIR}/cmake/AppMeshFITKDependencies.cmake" _cmake_rules)
foreach(_forbidden IN ITEMS IMPORTED_IMPLIB_RELEASE IMPORTED_LOCATION_RELEASE MAP_IMPORTED_CONFIG_RELEASE MAP_IMPORTED_CONFIG_RELWITHDEBINFO MAP_IMPORTED_CONFIG_MINSIZEREL)
    string(FIND "${_cmake_rules}" "${_forbidden}" _position)
    if(NOT _position EQUAL -1)
        message(FATAL_ERROR "Forbidden FITK imported configuration remains: ${_forbidden}")
    endif()
endforeach()

message(STATUS "Verified T041 FITK Debug whitelist and binary dependencies")
