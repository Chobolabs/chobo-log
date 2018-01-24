#
# chobo-log
# Copyright (c) 2016-2018 Chobolabs Inc.
# http://www.chobolabs.com/
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.2)

project(chobo-log)

option(CHOBO_LOG_SHARED "Build chobo-log as a shared library" OFF)
option(CHOBO_LOG_BUILD_EXAMPLES "Build chobo-log examples" OFF)
option(CHOBO_LOG_BUILD_TESTS "Build chobo-log tests" OFF)
option(CHOBO_LOG_BUILD_PERF "Build performance tests (requires manual step)" OFF)

# optional sinks
option(CHOBO_LOG_COMMON_SINKS "Add common sinks" ON)
option(CHOBO_LOG_ANDROID_SINK "Add android sink" OFF)
option(CHOBO_LOG_IOS_SINK "Add iOS sink" OFF)

if(NOT COMMAND chobo_source_group)
    # we're not in choboland so include macros
    include(build/cmake/macros.cmake)
endif()

set(inc ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(inc_path ${inc}/chobo/log)
set(src_path ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(sources)
chobo_source_group(Log sources
    ${inc}/chobo/Log.h
    ${inc_path}/Config.h
    ${inc_path}/Logger.h
    ${src_path}/LogManager.cpp
    ${inc_path}/LogManager.h
    ${inc_path}/PPCat.h
    ${inc_path}/Sink.h
    ${src_path}/Sink.cpp
    ${inc_path}/Scope.h
	${src_path}/Scope.cpp
    ${inc_path}/Severity.h
    ${src_path}/Src.h
    ${src_path}/Export.cpp
)

chobo_source_group(Extensions sources
    ${inc_path}/ext/DelayedLogger.h
)

if(CHOBO_LOG_COMMON_SINKS)
    chobo_source_group(Sinks sources
        ${inc_path}/sinks/ConsoleSink.h
        ${src_path}/sinks/ConsoleSink.cpp
        ${inc_path}/sinks/FILERecorder.h
        ${src_path}/sinks/FILERecorder.cpp
        ${inc_path}/sinks/RingBufferSink.h
        ${src_path}/sinks/RingBufferSink.cpp
    )
endif()

if(CHOBO_LOG_ANDROID_SINK)
    chobo_source_group(PlatformSinks sources
        ${inc_path}/sinks/AndroidSink.h
        ${src_path}/sinks/AndroidSink.cpp
    )
endif()

if(CHOBO_LOG_IOS_SINK)
    chobo_source_group(PlatformSinks sources
        ${inc_path}/sinks/IOSSink.h
        ${src_path}/sinks/IOSSink.mm
    )
endif()

if(CHOBO_LOG_SHARED)
    add_library(chobo-log SHARED
        ${sources}
    )

    target_compile_definitions(chobo-log PUBLIC
        -DCHOBO_LOG_DYN_LINK
    )

    if(MSVC)
        target_compile_options(chobo-log PUBLIC
            "/wd4251"
        )
    endif()

    if (CMAKE_HOST_APPLE)
        set_target_properties(chobo-log PROPERTIES MACOSX_RPATH ON)
    endif()
else()
    add_library(chobo-log STATIC
        ${sources}
    )
endif()

if(MSVC AND CHOBO_LOG_COMMON_SINKS)
    target_compile_definitions(chobo-log PRIVATE
        # for gmtime
        -D_CRT_SECURE_NO_WARNINGS
    )
endif()

set_target_properties(chobo-log PROPERTIES FOLDER chobo)

target_compile_features(chobo-log PUBLIC
    cxx_alias_templates
    cxx_auto_type
    cxx_decltype
    cxx_delegating_constructors
    cxx_final
    cxx_lambdas
    cxx_override
    cxx_range_for
    cxx_right_angle_brackets
    cxx_rvalue_references
    cxx_static_assert
)

target_include_directories(chobo-log PUBLIC
    ${inc}
)

if(CHOBO_LOG_BUILD_EXAMPLES)
    add_executable(simple-example examples/Simple.cpp)
    target_link_libraries(simple-example chobo-log)
endif()

if(CHOBO_LOG_BUILD_TESTS)
    enable_testing()

    add_executable(test_core
        test/Core.cpp
        test/CoreScope.cpp
        test/CoreDisabled.cpp
        test/Scopes.cpp
        test/Scopes.h
        test/test_sink/TestSink.h
        test/test_sink/TestSink.cpp
    )
    target_link_libraries(test_core chobo-log)
    add_test(test_core test_core)
endif()

if(CHOBO_LOG_BUILD_PERF)
    add_subdirectory(perf)
endif()
