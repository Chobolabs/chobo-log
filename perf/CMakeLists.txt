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

include_directories(
    temp/picobench-dev/include
    temp/spdlog-0.16.3/include
)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_executable(perf_execution
    execution/Config.h
    execution/Execution.cpp
    execution/bench-fstream.cpp
    execution/bench-chobo-log.cpp
    execution/bench-spdlog.cpp
)

target_link_libraries(perf_execution chobo-log)
