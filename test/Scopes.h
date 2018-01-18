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

#include <chobo/log/Scope.h>
#include <chobo/log/Logger.h>

class TestScopeA : public chobo::log::GlobalScope
{
public:
    TestScopeA();
};

extern TestScopeA scopeA;

class TestScopeB : public chobo::log::LocalScope
{
public:
    TestScopeB();
};

extern TestScopeB scopeB;

class TestScopeDisabled : public chobo::log::LocalScope
{
public:
    TestScopeDisabled();
};

CHOBO_LOG_HARD_DISABLE_SCOPE_TYPE(TestScopeDisabled)

class TestScopeFatalOnly : public chobo::log::GlobalScope
{
public:
    TestScopeFatalOnly();
};

CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_FATAL(TestScopeFatalOnly)
