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
#include <chobo/log/sinks/AndroidSink.h>
#include <chobo/log/LogManager.h>

#include <iostream>
#include <iomanip>

#include <android/log.h>

using namespace chobo::log;
using namespace std;

namespace
{

android_LogPriority ChoboToAndroid(Severity::Type severity)
{
    switch (severity)
    {
    case Severity::Debug: return ANDROID_LOG_DEBUG;
    case Severity::Info: return ANDROID_LOG_INFO;
    case Severity::Warning: return ANDROID_LOG_WARN;
    case Severity::Error: return ANDROID_LOG_ERROR;
    case Severity::Fatal: return ANDROID_LOG_FATAL;

    // warning suppressor
    case Severity::Count:  return ANDROID_LOG_SILENT;
    }
}

}

void AndroidSink::Record(
    ScopeData& scope,
    const chrono::time_point<chrono::system_clock>& timestamp,
    Severity::Type severity,
    const char* file,
    int line,
    const char* msg)
{
    __android_log_print(ChoboToAndroid(severity), LogManager::GetInstance().tag.c_str(),
        "%s", msg);
}
