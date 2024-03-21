function(target_link_libraries_with_compile_definitions target)
    set(options)
    set(oneValueArgs LINK_VISIBILITY DEPEND_VISIBILITY EXPORT_VISIBILITY)
    set(multiValueArgs LIBRARIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    target_link_libraries(${target} ${ARG_LINK_VISIBILITY} ${ARG_LIBRARIES})

    set(compile_definitions)
    foreach (library ${libraries})
        get_target_property(defs ${library} ${ARG_DEPEND_VISIBILITY})
        set(compile_definitions ${compile_definitions} ${defs})
    endforeach ()
    target_compile_definitions(${target} ${ARG_EXPORT_VISIBILITY} ${compile_definitions})
endfunction()


function(target_link_libraries_with_definitions target)
    # param: visibility
    set(visibility ${ARGV1})
    # param: libraries
    set(visibilities PUBLIC PRIVATE INTERFACE)
    if (${visibility} IN_LIST visibilities)
        list(SUBLIST ARGV 2 -1 libraries)
    else ()
        list(SUBLIST ARGV 1 -1 libraries)
    endif ()

    target_link_libraries_with_compile_definitions(${target}
            LINK_VISIBILITY PUBLIC
            DEPEND_VISIBILITY COMPILE_DEFINITIONS
            EXPORT_VISIBILITY PUBLIC
            LIBRARIES ${libraries}
    )
endfunction()
