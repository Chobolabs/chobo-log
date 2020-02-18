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

#include "Severity.h"

#include <cstdint>
#include <type_traits>

// main logger

namespace chobo {
namespace log
{

template <typename Scope, Severity::Type severity>
class Logger
{
public:
    Logger(Scope& scope, const char* file, int line)
        : m_scope(scope)
        , m_isEnabled(scope.IsEnabled() && severity >= scope.GetMinSeverity())
    {
        if (m_isEnabled)
        {
            scope.Begin(severity, file, line);
        }
    }

    ~Logger()
    {
        if (m_isEnabled)
        {
            m_scope.End();
        }
    }

    template <typename T>
    Logger& Put(const T& t)
    {
        if (m_isEnabled)
        {
            m_scope.stream << t;
        }

        return *this;
    }

    template <typename T>
    Logger& operator<<(const T& t)
    {
        return Put(t);
    }

    template <typename T>
    Logger& operator,(const T& t)
    {
        return Put(t);
    }

    // functions to "catch" string literals so as to not generate excessive template instantiations
    Logger& operator<<(const char* str)
    {
        return Put(str);
    }

    template <typename T>
    Logger& operator,(const char* str)
    {
        return Put(str);
    }

private:
    Scope& m_scope;
    const bool m_isEnabled;
};

class CHOBO_LOG_API NullLogger
{
public:
    template <typename T>
    NullLogger& operator<<(const T&)
    {
        return *this;
    }

    template <typename T>
    NullLogger& operator,(const T&)
    {
        return *this;
    }

    // functions to "catch" string literals so as to not generate excessive template instantiations
    NullLogger& operator<<(const char*)
    {
        return *this;
    }

    NullLogger& operator,(const char*)
    {
        return *this;
    }
};

}
} // namespace chobo::log
