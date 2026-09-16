if(NOT DEFINED CMAKE_COMMAND_PATH OR NOT DEFINED APP_EXECUTABLE OR
   NOT DEFINED RUNTIME_DIR OR NOT DEFINED QT_BIN_DIR)
    message(FATAL_ERROR "T041 smoke validation arguments are incomplete")
endif()

execute_process(
    COMMAND "${CMAKE_COMMAND_PATH}" -E env
        "QT_QPA_PLATFORM=offscreen"
        "PATH=${RUNTIME_DIR};${QT_BIN_DIR};$ENV{PATH}"
        "${APP_EXECUTABLE}" --smoke-test
    RESULT_VARIABLE _exit
    OUTPUT_VARIABLE _output
    ERROR_VARIABLE _error)
if(NOT _exit EQUAL 0)
    message(FATAL_ERROR "T041 real FITK smoke test failed with ${_exit}: ${_error}\n${_output}")
endif()

set(_combined "${_output}\n${_error}")
set(_ordered
    fitk.application
    settings.load
    system.check
    global-data.initialize
    components.initialize
    python.register
    main-window.create
    pre-window.initialize
    plugins.initialize
    application.initialize
    operators.register
    command-line.process
    event-loop.enter)
set(_last -1)
foreach(_stage IN LISTS _ordered)
    string(FIND "${_combined}" "start:${_stage}" _position)
    if(_position EQUAL -1 OR _position LESS _last)
        message(FATAL_ERROR "T041 smoke log is missing or misorders stage ${_stage}:\n${_combined}")
    endif()
    set(_last ${_position})
endforeach()

foreach(_required IN ITEMS "adapter=FITKFastCAERegistrationAdapter" "FITKAppFramework=1.0.7" "FITKCore=1.0.4")
    string(FIND "${_combined}" "${_required}" _position)
    if(_position EQUAL -1)
        message(FATAL_ERROR "T041 smoke log is missing real FITK evidence ${_required}:\n${_combined}")
    endif()
endforeach()

set(_shutdown_ordered
    operators.unregister
    application.shutdown
    plugins.shutdown
    pre-window.shutdown
    signals.disconnect
    main-window.destroy
    python.unregister
    components.shutdown
    global-data.shutdown
    fitk.application.shutdown)
set(_last -1)
foreach(_stage IN LISTS _shutdown_ordered)
    string(FIND "${_combined}" "rollback:${_stage}" _position)
    if(_position EQUAL -1 OR _position LESS _last)
        message(FATAL_ERROR "T041 smoke log is missing or misorders shutdown stage ${_stage}:\n${_combined}")
    endif()
    set(_last ${_position})
endforeach()

message(STATUS "Verified non-blocking real FITK smoke lifecycle")
