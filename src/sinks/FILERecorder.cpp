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
#include <chobo/log/sinks/FILERecorder.h>
#include <chobo/log/Scope.h>

#include <cstdio>

using namespace chobo::log;
using namespace std;

void FILERecorder::Record(
    ScopeData& scope,
    const chrono::time_point<chrono::system_clock>& timestamp,
    Severity::Type severity,
    const char* /*file*/,
    int /*line*/,
    const char* msg,
    FILE* fp)
{
    auto tt = chrono::system_clock::to_time_t(timestamp);
    auto time = gmtime(&tt);

    char tbuf[80];
    strftime(tbuf, 80, "%H:%M:%S", time);

    fprintf(fp, "[%s] %s %s: %s\n", tbuf, scope.name, Severity::ToString(severity), msg);
}
