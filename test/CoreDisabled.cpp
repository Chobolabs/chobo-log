//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "doctest/doctest.h"

#define CHOBO_LOG_ENABLED 0
#include <chobo/Log.h>

#include "Scopes.h"
#include "test_sink/TestSink.h"
#include <chobo/log/ext/DelayedLogger.h>

TEST_SUITE("chobo-log");

namespace
{

class SinkChecker : public chobo::log::Sink
{
public:
    virtual void Record(
        chobo::log::ScopeData&,
        const std::chrono::time_point<std::chrono::system_clock>&,
        chobo::log::Severity::Type,
        const char*,
        int,
        const char*) override
    {
        CHECK(false);
    }

} sinkChecker;

int counter = 0;
int SomeValue()
{
    return ++counter;
}

}

TEST_CASE("disabled")
{
    CHOBO_LOG_SCOPE(scopeA, Debug, "hello", SomeValue());
    CHOBO_LOG_SCOPE(scopeA, Info, SomeValue());
    CHOBO_LOG_SCOPE(scopeA, Warning, SomeValue(), SomeValue());
    CHOBO_LOG_SCOPE_STREAM(scopeA, Error) << "Stream: " << 123 << true;
    for (int i = 0; i < 10; ++i)
    {
        CHOBO_LOG_ONCE_SCOPE(scopeA, Fatal, 'f');
    }
    {
        CHOBO_LOG_SCOPE_DELAYED_STREAM(scopeA, Info, info);
        info << 1;
        CHOBO_LOG_SCOPE(scopeA, Debug, SomeValue());
        info << 2;
    }

    scopeB.AddSink(sinkChecker);

    CHOBO_LOG_SCOPE(scopeB, Debug, "hello", SomeValue());
    CHOBO_LOG_SCOPE(scopeB, Info, SomeValue());
    CHOBO_LOG_SCOPE(scopeB, Warning, SomeValue(), SomeValue());
    CHOBO_LOG_SCOPE_STREAM(scopeB, Error) << "Stream: " << 123 << true;
    for (int i = 0; i < 10; ++i)
    {
        CHOBO_LOG_ONCE_SCOPE(scopeB, Fatal, 'f');
    }
    {
        CHOBO_LOG_SCOPE_DELAYED_STREAM(scopeB, Info, info);
        info << 1;
        CHOBO_LOG_SCOPE(scopeB, Debug, SomeValue());
        info << 2;
    }

    CHOBO_LOG_DEFAULT(Debug, "hello", SomeValue());
    CHOBO_LOG_DEFAULT(Info, SomeValue());
    CHOBO_LOG_DEFAULT(Warning, SomeValue(), SomeValue());
    CHOBO_LOG_DEFAULT_STREAM(Error) << "Stream: " << 123 << true;
    for (int i = 0; i < 10; ++i)
    {
        CHOBO_LOG_ONCE_DEFAULT(Fatal, 'f');
    }
    {
        CHOBO_LOG_DEFAULT_DELAYED_STREAM(Info, info);
        info << 1;
        CHOBO_LOG_DEFAULT(Debug, SomeValue());
        info << 2;
    }

    CHECK(counter == 0);

    auto& sink = TestSink::instance();
    CHECK(sink.record.empty());
}
