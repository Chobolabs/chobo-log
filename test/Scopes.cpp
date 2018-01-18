//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Scopes.h"

TestScopeA scopeA;
TestScopeB scopeB;

TestScopeA::TestScopeA()
    : chobo::log::GlobalScope("Test Scope A", 1)
{}

TestScopeB::TestScopeB()
    : chobo::log::LocalScope("Test Scope B", 2)
{}

TestScopeDisabled::TestScopeDisabled()
    : chobo::log::LocalScope("Disabled", 3)
{}

TestScopeFatalOnly::TestScopeFatalOnly()
    : chobo::log::GlobalScope("FatalOnly", 4)
{}