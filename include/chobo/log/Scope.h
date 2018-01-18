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

#include <mutex>
#include <ostream>
#include <cstdint>
#include <vector>
#include <string>

namespace chobo {
namespace log
{

class Sink;

struct CHOBO_LOG_API ScopeData
{
    explicit ScopeData(const char* name, int id = -1);

    const char* const name;
    const int id;

    Severity::Type minSeverity = Severity::Debug;

    intptr_t userData = 0;
};

class CHOBO_LOG_API BasicScope : public ScopeData
{
public:
    explicit BasicScope(const char* name, int id = -1, size_t initialBufSize = 1024);
    ~BasicScope();

    static void SetGlobalTag(const std::string& tag) { m_globalTag = tag; }
    static const std::string& GetGlobalTag() { return m_globalTag; }

    void AddSink(Sink& r);

    void Begin(Severity::Type severity, const char* file, int line);

    Severity::Type GetMinSeverity() const { return minSeverity >= m_globalMinSeverity ? minSeverity : m_globalMinSeverity; }
    
    // only valid between a begin/end session
    std::ostream stream;
    
    void End();

    void SetEnabled(bool set = true) 
    {
        m_disableLevel =
            set ?
                m_disableLevel & ~Disable_Scope
            :
                m_disableLevel | Disable_Scope;
    }

    bool IsEnabled() const { return !m_disableLevel; }

protected:
    friend class LogManager;    
    static std::string m_globalTag;

    Severity::Type m_globalMinSeverity = Severity::Debug; // set by manager

    std::vector<Sink*> m_sinks;

    // session data
    Severity::Type m_severity = Severity::Count;
    const char* m_file = nullptr;
    int m_line = 0;    

    // various disablers flip various bits of this integer
    enum DisableLevel
    {
        Disable_Scope = 1,
        Disable_Manager = 2
    };
    int m_disableLevel = 0;
};

class CHOBO_LOG_API LocalScope : public BasicScope
{
public:
    explicit LocalScope(const char* name, int id = -1, size_t initialBufSize = 1024);
    ~LocalScope();
};

class CHOBO_LOG_API SyncScope : public BasicScope
{
public:
    explicit SyncScope(const char* name, int id = -1, size_t initialBufSize = 1024)
        : BasicScope(name, id, initialBufSize)
    {}

    // hiding base implementation with lock/unlock of mutex
    void Begin(Severity::Type severity, const char* file, int line)
    {
        m_logMutex.lock();
        BasicScope::Begin(severity, file, line);
    }

    void End()
    {
        BasicScope::End();
        m_logMutex.unlock();
    }

protected:
    std::mutex m_logMutex;
};

class CHOBO_LOG_API GlobalScope : public SyncScope
{
public:
    explicit GlobalScope(const char* name, int id = -1, size_t initialBufSize = 1024);
    ~GlobalScope();
};

class CHOBO_LOG_API DefaultScope : public GlobalScope
{
public:
    static DefaultScope& Get();
    
    // also construction happens here
    static DefaultScope& GetInstance();

private:
    DefaultScope();
    static DefaultScope& m_instance;
};

}
}

// hard disable scope severity macros
#define _CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(Scope, severity) \
    namespace chobo { \
    namespace log { \
        template <> \
        class Logger<Scope, Severity::severity> : public NullLogger \
        { \
        public: Logger(Scope&, const char*, int) {} \
        }; \
    } \
    }

#define CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_INFO(Scope) \
    _CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(Scope, Debug)

#define CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_WARNING(Scope) \
    CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_INFO(Scope) \
    _CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(Scope, Info)

#define CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_ERROR(Scope) \
    CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_WARNING(Scope) \
    _CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(Scope, Warning)

#define CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_FATAL(Scope) \
    CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_ERROR(Scope) \
    _CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(Scope, Error)

#define CHOBO_LOG_HARD_DISABLE_SCOPE_TYPE(Scope) \
    namespace chobo { \
    namespace log { \
        template <Severity::Type severity> \
        class Logger<Scope, severity> : public NullLogger \
        { \
            public: Logger(Scope&, const char*, int) {} \
        }; \
    } \
    }
