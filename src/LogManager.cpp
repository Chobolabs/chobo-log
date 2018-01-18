//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Src.h"
#include <chobo/log/LogManager.h>
#include <chobo/log/Sink.h>
#include <chobo/log/Scope.h>

#include <algorithm>
#include <cassert>

using namespace chobo::log;
using namespace std;

#define scoped_lock(mut) std::lock_guard<std::mutex> __lock(mut)

// used to synchronize lifetimes of global objects
static bool LogManager_Destroyed = false;

LogManager& LogManager::GetInstance()
{
    static LogManager manager;
    return manager;
}

LogManager::LogManager()
    : tag("Chobo")
{
}

LogManager::~LogManager()
{
    LogManager_Destroyed = true;
}

void LogManager::SetGlobalMinSeverity(Severity::Type s)
{
    scoped_lock(mutex);
    for (auto scope : m_scopes)
    {
        scope->m_globalMinSeverity = s;
    }
}

void LogManager::SetGlobalLoggingEnabled(bool set)
{
    scoped_lock(mutex);
    for (auto scope : m_scopes)
    {
        scope->m_disableLevel =
            set ?
                scope->m_disableLevel & ~BasicScope::Disable_Manager
            :
                scope->m_disableLevel | BasicScope::Disable_Manager;
    }
}

void LogManager::AddGlobalSink(Sink& r)
{
    scoped_lock(mutex);
    m_sinks.push_back(&r);
    for (auto scope : m_scopes)
    {
        scope->AddSink(r);
    }
}

void LogManager::RegisterScope(BasicScope& scope)
{
    scoped_lock(mutex);
    scope.m_sinks = m_sinks;
    m_scopes.push_back(&scope);
}

void LogManager::UnregisterScope(BasicScope& scope)
{    
    if (LogManager_Destroyed)
    {
        // We're here because a global scope class wants to unregister itself from the manager
        // However the manager is already destoyed
        // This can only happen at module shutdown, so everything will soon be destroyed anyway
        // So... we just don't care about that
        return;
    }

    scoped_lock(mutex);
    auto f = std::find(m_scopes.begin(), m_scopes.end(), &scope);
    assert(f != m_scopes.end());
    m_scopes.erase(f);
}
