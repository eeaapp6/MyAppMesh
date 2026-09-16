if(NOT DEFINED DUMPBIN_EXECUTABLE OR NOT DEFINED APP_EXECUTABLE OR NOT DEFINED CONFIG)
    message(FATAL_ERROR "DUMPBIN_EXECUTABLE, APP_EXECUTABLE and CONFIG are required")
endif()

if(NOT CONFIG STREQUAL "Debug")
    message(FATAL_ERROR "Dependency inspection rejects non-Debug configuration: ${CONFIG}")
endif()

execute_process(
    COMMAND "${DUMPBIN_EXECUTABLE}" /dependents "${APP_EXECUTABLE}"
    RESULT_VARIABLE _dumpbin_exit
    OUTPUT_VARIABLE _dumpbin_output
    ERROR_VARIABLE _dumpbin_error)
if(NOT _dumpbin_exit EQUAL 0)
    message(FATAL_ERROR
        "dumpbin /dependents failed with ${_dumpbin_exit}: ${_dumpbin_error}")
endif()

string(TOLOWER "${_dumpbin_output}" _dependencies)
foreach(_required IN ITEMS "hdf5_d.dll" "saribbonbard.dll" "qt5cored.dll" "qt5guid.dll" "qt5widgetsd.dll")
    string(FIND "${_dependencies}" "${_required}" _position)
    if(_position EQUAL -1)
        message(FATAL_ERROR "Missing required Debug dependency: ${_required}")
    endif()
endforeach()

foreach(_forbidden IN ITEMS "hdf5.dll" "saribbonbar.dll" "qt5core.dll" "qt5gui.dll" "qt5widgets.dll")
    string(FIND "${_dependencies}" "${_forbidden}" _position)
    if(NOT _position EQUAL -1)
        message(FATAL_ERROR "Found forbidden non-Debug dependency: ${_forbidden}")
    endif()
endforeach()

message(STATUS "Verified Debug dependencies for ${APP_EXECUTABLE}")
