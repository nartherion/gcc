function(nartherion_gcc_set_compiler_warnings TARGET_PROJECT)
    set(CLANG_WARNINGS
        -Wall
        -Wextra
        -Werror
        # Warn if a variable declaration shadows one from a parent context.
        -Wshadow
        # Warn if a class with virtual functions has a non-virtual destructor.
        -Wnon-virtual-dtor
        # Warn for C-style casts.
        -Wold-style-cast
        # Warn for potential performance problem casts.
        -Wcast-align
        # Warn on anything being unused.
        -Wunused
        # Warn if you overload (not override) a virtual function.
        -Woverloaded-virtual
        # Warn if non-standard C++ is used.
        -Wpedantic
        # Warn on type conversions that may lose data.
        -Wconversion
        # Warn on sign conversions.
        -Wsign-conversion
        # Warn if a null dereference is detected.
        -Wnull-dereference
        # Warn if float is implicitly promoted to double.
        -Wdouble-promotion
        # Warn on security issues around functions that format output.
        -Wformat=2
        # Warn on statements that fallthrough without an explicit annotation.
        -Wimplicit-fallthrough)

    set(GCC_WARNINGS
        ${CLANG_WARNINGS}
        # Warn if indentation implies blocks where blocks do not exist.
        -Wmisleading-indentation
        # Warn if if {} else {} chain has duplicated conditions.
        -Wduplicated-cond
        # Warn if if {} else {} branches have duplicated code.
        -Wduplicated-branches
        # Warn about logical operations being used where bitwise were probably wanted.
        -Wlogical-op
        # Warn if you perform a cast to the same type.
        -Wuseless-cast
        # Warn if an overridden member function is not marked 'override' or 'final'.
        -Wsuggest-override)

    if(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
        set(PROJECT_WARNINGS_CXX ${CLANG_WARNINGS})
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(PROJECT_WARNINGS_CXX ${GCC_WARNINGS})
    else()
        message(AUTHOR_WARNING "No compiler warnings set for C++ compiler: '${CMAKE_CXX_COMPILER_ID}'")
    endif()
    set(PROJECT_WARNINGS_C "${PROJECT_WARNINGS_CXX}")

    target_compile_options(${TARGET_PROJECT} INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${PROJECT_WARNINGS_CXX}>
                                                       $<$<COMPILE_LANGUAGE:C>:${PROJECT_WARNINGS_C}>)
endfunction()
