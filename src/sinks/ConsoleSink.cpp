//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "../Src.h"

#include <chobo/log/sinks/ConsoleSink.h>
#include <chobo/log/Scope.h>

#include <cstdio>

using namespace chobo::log;
using namespace std;

namespace
{
FILE* get_output_stream(Severity::Type severity)
{
    switch (severity)
    {
    case Severity::Debug: return stdout;
    case Severity::Info: return stdout;
    case Severity::Warning: return stdout;
    case Severity::Error: return stderr;
    case Severity::Fatal: return stderr;
    default: return stderr; // warning suppressor
    }
}
}

void ConsoleSink::Record(
    ScopeData& scope,
    const chrono::time_point<chrono::system_clock>& timestamp,
    Severity::Type severity,
    const char* file,
    int line,
    const char* msg)
{
    auto out = get_output_stream(severity);
    FILERecorder::Record(scope, timestamp, severity, file, line, msg, out);
}
