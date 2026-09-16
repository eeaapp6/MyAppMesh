if(NOT DEFINED DUMPBIN_EXECUTABLE OR NOT DEFINED SARIBBON_DLL OR
   NOT DEFINED RUNTIME_DIR OR NOT DEFINED SOURCE_DIR OR NOT DEFINED CONFIG)
    message(FATAL_ERROR "SARibbon dependency validation arguments are incomplete")
endif()

if(NOT CONFIG STREQUAL "Debug")
    message(FATAL_ERROR "SARibbon dependency inspection rejects non-Debug configuration: ${CONFIG}")
endif()

if(NOT EXISTS "${SARIBBON_DLL}" OR NOT EXISTS "${RUNTIME_DIR}/SARibbonBard.dll")
    message(FATAL_ERROR "The audited/staged Debug SARibbonBard.dll is missing")
endif()
if(EXISTS "${RUNTIME_DIR}/SARibbonBar.dll")
    message(FATAL_ERROR "Release SARibbonBar.dll must not be staged")
endif()

execute_process(
    COMMAND "${DUMPBIN_EXECUTABLE}" /headers "${SARIBBON_DLL}"
    RESULT_VARIABLE _headers_exit
    OUTPUT_VARIABLE _headers)
string(FIND "${_headers}" "8664 machine (x64)" _x64_position)
if(NOT _headers_exit EQUAL 0 OR _x64_position EQUAL -1)
    message(FATAL_ERROR "SARibbonBard.dll is not the audited x64 binary: ${_headers}")
endif()

execute_process(
    COMMAND "${DUMPBIN_EXECUTABLE}" /dependents "${SARIBBON_DLL}"
    RESULT_VARIABLE _dependents_exit
    OUTPUT_VARIABLE _dependents)
if(NOT _dependents_exit EQUAL 0)
    message(FATAL_ERROR "dumpbin failed for SARibbonBard.dll")
endif()
string(TOLOWER "${_dependents}" _dependencies)
foreach(_required IN ITEMS qt5cored.dll qt5guid.dll qt5widgetsd.dll msvcp140d.dll vcruntime140d.dll ucrtbased.dll)
    string(FIND "${_dependencies}" "${_required}" _position)
    if(_position EQUAL -1)
        message(FATAL_ERROR "SARibbonBard.dll is missing Debug ABI dependency: ${_required}")
    endif()
endforeach()
foreach(_forbidden IN ITEMS qt5core.dll qt5gui.dll qt5widgets.dll)
    string(FIND "${_dependencies}" "${_forbidden}" _position)
    if(NOT _position EQUAL -1)
        message(FATAL_ERROR "SARibbonBard.dll contains non-Debug dependency: ${_forbidden}")
    endif()
endforeach()

file(READ "${SOURCE_DIR}/cmake/AppMeshDependencies.cmake" _rules)
foreach(_required_rule IN ITEMS "APPMesh::SARibbon" "IMPORTED_CONFIGURATIONS Debug" "IMPORTED_IMPLIB_DEBUG" "IMPORTED_LOCATION_DEBUG")
    string(FIND "${_rules}" "${_required_rule}" _position)
    if(_position EQUAL -1)
        message(FATAL_ERROR "SARibbon imported target is missing rule: ${_required_rule}")
    endif()
endforeach()

message(STATUS "Verified x64 Debug SARibbon runtime and imported target")
