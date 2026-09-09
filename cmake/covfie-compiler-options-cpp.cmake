# SPDX-PackageName: "covfie, a part of the ACTS project"
# SPDX-FileCopyrightText: 2022 CERN
#
# SPDX-License-Identifier: MPL-2.0

cmake_minimum_required(VERSION 3.21)

# Only set these compiler flags if we are the top level project.
if(PROJECT_IS_TOP_LEVEL)
    # Include the helper function(s).
    include(covfie-functions)

    # Turn on a number of warnings for the "known compilers".
    if(
        (
            "${CMAKE_CXX_COMPILER_ID}"
                MATCHES
                "GNU"
        )
        OR (
            "${CMAKE_CXX_COMPILER_ID}"
                MATCHES
                "Clang"
        )
    )
        # Basic flags for all build modes.
        covfie_add_flag( CMAKE_CXX_FLAGS "-Wall" )
        covfie_add_flag( CMAKE_CXX_FLAGS "-Wextra" )
        covfie_add_flag( CMAKE_CXX_FLAGS "-Wshadow" )
        covfie_add_flag( CMAKE_CXX_FLAGS "-Wunused-local-typedefs" )
        covfie_add_flag( CMAKE_CXX_FLAGS "-pedantic" )
        covfie_add_flag( CMAKE_CXX_FLAGS "-Wfloat-conversion" )
        covfie_add_flag( CMAKE_CXX_FLAGS "-Wconversion" )

        # Fail on warnings, if asked for that behaviour.
        if(COVFIE_FAIL_ON_WARNINGS)
            covfie_add_flag( CMAKE_CXX_FLAGS "-Werror" )
        endif()

        # Instrument the code for coverage, if asked for that behaviour.
        if(COVFIE_BUILD_COVERAGE)
            covfie_add_flag( CMAKE_CXX_FLAGS "--coverage" )
            covfie_add_flag( CMAKE_EXE_LINKER_FLAGS "--coverage" )

            # gcov must find the headers again when it writes the report, and
            # it cannot do so from a build directory outside the source tree
            # unless the paths are absolute. Clang has no such flag, but it
            # records absolute paths anyway.
            if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
                covfie_add_flag( CMAKE_CXX_FLAGS "-fprofile-abs-path" )
            endif()

            # An assertion compiles to a branch whose false side only aborts
            # the process, and the library also guards whole blocks with
            # `#ifndef NDEBUG`. None of that reaches a release build, so it
            # would only dilute the report. Define NDEBUG to measure the code
            # that ships.
            covfie_add_flag( CMAKE_CXX_FLAGS "-DNDEBUG" )

            message(
                STATUS
                "Coverage is enabled, so NDEBUG is defined and assertions are disabled in this build."
            )

            if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
                message(
                    WARNING
                    "Coverage is enabled in a ${CMAKE_BUILD_TYPE} build. Use a Debug build, because inlining distorts the report."
                )
            endif()
        endif()
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
        # Basic flags for all build modes.
        string(REGEX REPLACE "/W[0-9]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        covfie_add_flag( CMAKE_CXX_FLAGS "/W4" )

        # Fail on warnings, if asked for that behaviour.
        if(COVFIE_FAIL_ON_WARNINGS)
            covfie_add_flag( CMAKE_CXX_FLAGS "/WX" )
        endif()

        # Turn on the correct setting for the __cplusplus macro with MSVC.
        covfie_add_flag( CMAKE_CXX_FLAGS "/Zc:__cplusplus" )
    endif()

    # Coverage is only wired up for the GCC and Clang flag syntax, so tell the
    # user when the request has no effect rather than reporting nothing later.
    if(
        COVFIE_BUILD_COVERAGE
        AND NOT (
            (
                "${CMAKE_CXX_COMPILER_ID}"
                    MATCHES
                    "GNU"
            )
            OR (
                "${CMAKE_CXX_COMPILER_ID}"
                    MATCHES
                    "Clang"
            )
        )
    )
        message(
            WARNING
            "Coverage was requested, but it is not supported for the ${CMAKE_CXX_COMPILER_ID} compiler."
        )
    endif()
endif()
