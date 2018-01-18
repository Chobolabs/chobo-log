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

// log severities

#define _CHOBO_LOG_ITERATE_SEVERITIES(MACRO) \
    MACRO(Debug) \
    MACRO(Info) \
    MACRO(Warning) \
    MACRO(Error) \
    MACRO(Fatal)

#define _CHOBO_LOG_ENUM(elem) elem,

#define _CHOBO_LOG_CASE_STR(elem) \
    case elem: return #elem;

namespace chobo {
namespace log
{

struct CHOBO_LOG_API Severity
{
    enum Type
    {
        _CHOBO_LOG_ITERATE_SEVERITIES(_CHOBO_LOG_ENUM)

        Count
    };

    static const char* ToString(Type t)
    {
        switch (t)
        {
            _CHOBO_LOG_ITERATE_SEVERITIES(_CHOBO_LOG_CASE_STR)
        default: return "?";
        }
    }
};

} } // namespace chobo::log

#define CHOBO_LOG_SEVERITY_DEBUG 0
#define CHOBO_LOG_SEVERITY_INFO 1
#define CHOBO_LOG_SEVERITY_WARNING 2
#define CHOBO_LOG_SEVERITY_ERROR 3
#define CHOBO_LOG_SEVERITY_FATAL 4
