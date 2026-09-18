if(NOT CONFIG STREQUAL "Debug")
    message(FATAL_ERROR "T016 rejects non-Debug configuration: ${CONFIG}")
endif()
if(NOT EXISTS "${GEOMETRY_DLL}" OR NOT EXISTS "${APP_EXECUTABLE}")
    message(FATAL_ERROR "T016 binary dependency inputs are missing")
endif()

execute_process(COMMAND "${DUMPBIN_EXECUTABLE}" /dependents "${GEOMETRY_DLL}"
    RESULT_VARIABLE _exit OUTPUT_VARIABLE _output ERROR_VARIABLE _error)
if(NOT _exit EQUAL 0)
    message(FATAL_ERROR "dumpbin failed for FITKGeoCompOCC.dll: ${_error}")
endif()
string(TOLOWER "${_output}" _dependencies)
foreach(_required IN ITEMS fitkinterfacegeometry.dll fitkinterfacemodel.dll fitkappframework.dll
        fitkcore.dll qt5cored.dll qt5guid.dll tkbrep.dll tkstep.dll tkiges.dll)
    string(FIND "${_dependencies}" "${_required}" _position)
    if(_position EQUAL -1)
        message(FATAL_ERROR "FITKGeoCompOCC.dll is missing audited dependency: ${_required}")
    endif()
endforeach()
foreach(_forbidden IN ITEMS qt5core.dll qt5gui.dll qt5widgets.dll meshapp.dll graphdata.dll)
    string(FIND "${_dependencies}" "${_forbidden}" _position)
    if(NOT _position EQUAL -1)
        message(FATAL_ERROR "FITKGeoCompOCC.dll contains forbidden dependency: ${_forbidden}")
    endif()
endforeach()

foreach(_runtime IN ITEMS FITKGeoCompOCC.dll FITKInterfaceGeometry.dll FITKInterfaceModel.dll
        TKBO.dll TKBool.dll TKBRep.dll TKernel.dll TKFeat.dll TKFillet.dll TKG2d.dll TKG3d.dll
        TKGeomAlgo.dll TKGeomBase.dll TKIGES.dll TKLCAF.dll TKMath.dll TKMesh.dll TKOffset.dll
        TKPrim.dll TKShHealing.dll TKSTEP.dll TKSTL.dll TKTopAlgo.dll TKXCAF.dll TKXDEIGES.dll
        TKXDESTEP.dll TKXSBase.dll dime0d.dll)
    if(NOT EXISTS "${RUNTIME_DIR}/${_runtime}")
        message(FATAL_ERROR "Audited T016 Debug runtime is missing: ${_runtime}")
    endif()
endforeach()
message(STATUS "Verified T016 FITK/OCC Debug runtime closure")
