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

#include "../Config.h"
#include "../Sink.h"

namespace chobo
{
namespace log
{

class CHOBO_LOG_API ConsoleSink : public Sink
{
public:
    virtual void Record(
        ScopeData& scope,
        const std::chrono::time_point<std::chrono::system_clock>& timestamp,
        Severity::Type severity,
        const char* file,
        int line,
        const char* msg) override;
};

}
}
