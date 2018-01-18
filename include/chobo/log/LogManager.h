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
#include <vector>
#include <string>

// Logger manager class
// Singleton

namespace chobo {
namespace log
{

class Sink;
class BasicScope;

class CHOBO_LOG_API LogManager
{
public:
    static LogManager& GetInstance();

    bool IsSeverityEnabled(Severity::Type severity) { return severity >= m_minSeverity; }

    void AddGlobalSink(Sink& r);

    void RegisterScope(BasicScope& scope);
    void UnregisterScope(BasicScope& scope);

    // registered scope management
    // set global min serverity
    void SetGlobalMinSeverity(Severity::Type s);
    Severity::Type GetGlobalMinSeverity() const { return m_minSeverity; }

    void SetGlobalLoggingEnabled(bool set = true);

    std::mutex mutex; // useful if you want a global mutex for your logging ops
    std::string tag; // you can use this to tag all log ops in your software
    
private:
    LogManager();
    ~LogManager();

    Severity::Type m_minSeverity = Severity::Debug;
    std::vector<Sink*> m_sinks;
    std::vector<BasicScope*> m_scopes;
};

} } // namespace chobo::log
