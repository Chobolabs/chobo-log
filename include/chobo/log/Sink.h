//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#pragma once

#include "Config.h"

#include <chrono>
#include <cstdint>

#include "Severity.h"

namespace chobo
{
namespace log
{

struct ScopeData;

class CHOBO_LOG_API Sink
{
public:
    virtual ~Sink() {}

    // called by the manager when a new session begins
    virtual void NewSession() {}

    virtual void Record(
        ScopeData& scope,
        const std::chrono::time_point<std::chrono::system_clock>& timestamp,
        Severity::Type severity,
        const char* file,
        int line,
        const char* msg) = 0;

    // Utils
    static const char* GetFileOfPath(const char* path);
private:
};

}
}