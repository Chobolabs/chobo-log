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
#include <chobo/log/LogManager.h>
#include <chobo/log/Scope.h>

#include <iostream>
#include <iomanip>

using namespace chobo::log;
using namespace std;

namespace
{
ostream& get_output_stream(Severity::Type severity)
{
    switch (severity)
    {
    case Severity::Debug: return cout;
    case Severity::Info: return cout;
    case Severity::Warning: return cout;
    case Severity::Error: return cerr;
    case Severity::Fatal: return cerr;
    default: return cout; // warning suppressor
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
    auto tt = chrono::system_clock::to_time_t(timestamp);
    auto time = gmtime(&tt);

    char tbuf[80];
    strftime(tbuf, 80, "%H:%M:%S", time);

    ostream& out = get_output_stream(severity);

    out << "[" << tbuf << "] ";

    //out << "(" << file << ":" << line << ") ";
    out << scope.name << " " << Severity::ToString(severity) << ": " << msg;

    out << endl;
}
