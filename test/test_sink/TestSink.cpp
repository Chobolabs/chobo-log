//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "TestSink.h"

void TestSink::Record(
    chobo::log::ScopeData& scope,
    const std::chrono::time_point<std::chrono::system_clock>& timestamp,
    chobo::log::Severity::Type severity,
    const char* file,
    int line,
    const char* msg)
{
    record.push_back({ &scope, timestamp, severity, file, line, msg });
}

TestSink& TestSink::instance()
{
    static TestSink sink;
    return sink;
}