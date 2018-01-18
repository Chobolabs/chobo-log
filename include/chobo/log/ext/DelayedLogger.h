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
#include "../Logger.h"

#include <sstream>

// delayed logger extension

namespace chobo {
namespace log
{

template <typename Scope, Severity::Type severity>
class DelayedLogger
{
public:
    DelayedLogger(Scope& scope, const char* file, int line)
        : m_scope(scope)
        , m_file(file)
        , m_line(line)
        , m_isEnabled(scope.IsEnabled() && severity >= scope.GetMinSeverity())
    {
    }

    ~DelayedLogger()
    {
        if (m_isEnabled )
        {
            std::string str = m_stream.str();
            if (!str.empty())
            {
                Logger<Scope, severity>(m_scope, m_file, m_line) << str;
            }
        }
    }

    template <typename T>
    DelayedLogger& Put(const T& t)
    {
        if (m_isEnabled)
        {
            m_stream << t;
        }

        return *this;
    }

    template <typename T>
    DelayedLogger& operator<<(const T& t)
    {
        return Put(t);
    }

    // functions to "catch" string literals so as to not generate excessive template instantiations
    DelayedLogger& operator<<(const char* str)
    {
        return Put(str);
    }

private:
    Scope& m_scope;
    const char* m_file;
    int m_line;
    const bool m_isEnabled;
    std::ostringstream m_stream;
};

}
}

#if CHOBO_LOG_ENABLED
#define CHOBO_LOG_SCOPE_DELAYED_STREAM(scope, severity, var) ::chobo::log::DelayedLogger<typename std::remove_reference<decltype(scope)>::type, ::chobo::log::Severity::severity> var(scope, __FILE__, __LINE__)
#else
#define CHOBO_LOG_SCOPE_DELAYED_STREAM(scope, severity, var) ::chobo::log::NullLogger var
#endif

#define CHOBO_LOG_DEFAULT_DELAYED_STREAM(severity, var) CHOBO_LOG_SCOPE_DELAYED_STREAM(::chobo::log::DefaultScope::Get(), severity, var)