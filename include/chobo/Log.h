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

// main include file

#include "log/Config.h"

#include "log/Severity.h"

#if !defined(CHOBO_LOG_ENABLED)
#   define CHOBO_LOG_ENABLED 1
#endif

#if !defined(CHOBO_LOG_MIN_SEVERITY)
#   define CHOBO_LOG_MIN_SEVERITY CHOBO_LOG_SEVERITY_DEBUG
#endif

#include "log/Logger.h"
#include "log/Scope.h"
#include "log/PPCat.h"

#define _CHOBO_INTERNAL_LOG_NOOP(...)

#if CHOBO_LOG_ENABLED

#define _CHOBO_INTERNAL_LOG(scope, severity, ...) ::chobo::log::Logger<typename std::remove_reference<decltype(scope)>::type, ::chobo::log::Severity::severity>(scope, __FILE__, __LINE__) , __VA_ARGS__

#if CHOBO_LOG_SEVERITY_DEBUG >= CHOBO_LOG_MIN_SEVERITY
#   define _CHOBO_INTERNAL_LOG_Debug(scope, ...) _CHOBO_INTERNAL_LOG(scope, Debug, __VA_ARGS__)
#else
#   define _CHOBO_INTERNAL_LOG_Debug _CHOBO_INTERNAL_LOG_NOOP
_CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(::chobo::log::DefaultScope, Debug)
#endif

#if CHOBO_LOG_SEVERITY_INFO >= CHOBO_LOG_MIN_SEVERITY
#   define _CHOBO_INTERNAL_LOG_Info(scope, ...) _CHOBO_INTERNAL_LOG(scope, Info, __VA_ARGS__)
#else
#   define _CHOBO_INTERNAL_LOG_Info _CHOBO_INTERNAL_LOG_NOOP
_CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(::chobo::log::DefaultScope, Info)
#endif

#if CHOBO_LOG_SEVERITY_WARNING >= CHOBO_LOG_MIN_SEVERITY
#   define _CHOBO_INTERNAL_LOG_Warning(scope, ...) _CHOBO_INTERNAL_LOG(scope, Warning, __VA_ARGS__)
#else
#   define _CHOBO_INTERNAL_LOG_Warning _CHOBO_INTERNAL_LOG_NOOP
_CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(::chobo::log::DefaultScope, Warning)
#endif

#if CHOBO_LOG_SEVERITY_ERROR >= CHOBO_LOG_MIN_SEVERITY
#   define _CHOBO_INTERNAL_LOG_Error(scope, ...) _CHOBO_INTERNAL_LOG(scope, Error, __VA_ARGS__)
#else
#   define _CHOBO_INTERNAL_LOG_Error _CHOBO_INTERNAL_LOG_NOOP
_CHOBO_LOG_SCOPE_HARD_DISABLE_SEVERITY(::chobo::log::DefaultScope, ERROR)
#endif

#define _CHOBO_INTERNAL_LOG_Fatal(scope, ...) _CHOBO_INTERNAL_LOG(scope, Fatal, __VA_ARGS__)

#define CHOBO_LOG_SCOPE(scope, severity, ...) _CHOBO_LOG_CAT(_CHOBO_INTERNAL_LOG_, severity)(scope, __VA_ARGS__)
#define CHOBO_LOG_DEFAULT(severity, ...) CHOBO_LOG_SCOPE(::chobo::log::DefaultScope::Get(), severity, __VA_ARGS__)

#define CHOBO_LOG_ONCE_SCOPE(scope, severity, ...) \
    do { \
        static bool logged = false; \
        if(!logged) \
        { \
            CHOBO_LOG_SCOPE(scope, severity, __VA_ARGS__); \
            logged = true; \
        } \
    } while(false)

#define CHOBO_LOG_ONCE_DEFAULT(severity, ...) CHOBO_LOG_ONCE_SCOPE(::chobo::log::DefaultScope::Get(), severity, __VA_ARGS__)

#define CHOBO_LOG_SCOPE_STREAM(scope, severity) ::chobo::log::Logger<typename std::remove_reference<decltype(scope)>::type, ::chobo::log::Severity::severity>(scope, __FILE__, __LINE__)
#define CHOBO_LOG_DEFAULT_STREAM(severity) CHOBO_LOG_SCOPE_STREAM(::chobo::log::DefaultScope::Get(), severity)

#else

#define CHOBO_LOG_SCOPE(scope, severity, ...) _CHOBO_INTERNAL_LOG_NOOP()
#define CHOBO_LOG_DEFAULT(severity, ...) _CHOBO_INTERNAL_LOG_NOOP()

#define CHOBO_LOG_SCOPE_STREAM(scope, severity) ::chobo::log::NullLogger()
#define CHOBO_LOG_DEFAULT_STREAM(severity) ::chobo::log::NullLogger()

#define CHOBO_LOG_ONCE_SCOPE(scope, severity, ...) _CHOBO_INTERNAL_LOG_NOOP()
#define CHOBO_LOG_ONCE_DEFAULT(severity, ...) _CHOBO_INTERNAL_LOG_NOOP()

#endif
