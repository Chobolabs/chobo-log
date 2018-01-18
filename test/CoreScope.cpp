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

#include <chobo/Log.h>

#include "Scopes.h"
#include "test_sink/TestSink.h"

TEST_SUITE("chobo-log");

namespace
{
static void LogSomething()
{
    CHOBO_LOG_SCOPE(scopeA, Debug, 123);
}

class MultiSink : public chobo::log::Sink
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
        ++counter;
    }

    int counter = 0;
} sinkChecker;

}

TEST_CASE("scope a")
{
    sinkChecker.counter = 0;
    scopeA.AddSink(sinkChecker);

    CHECK(scopeA.name == "Test Scope A");
    CHECK(scopeA.id == 1);
    CHECK(scopeA.IsEnabled());
    CHECK(scopeA.minSeverity == chobo::log::Severity::Debug);
    CHECK(scopeA.userData == 0);

    LogSomething();

    auto& sink = TestSink::instance();
    REQUIRE(sink.record.size() == 1);
    auto r = sink.pop();

    auto now = std::chrono::system_clock::now();
    auto duration = r.timestamp - now;
    // can't be more than one
    CHECK(std::chrono::duration_cast<std::chrono::seconds>(duration).count() < 2);
    CHECK(r.file == __FILE__);
    CHECK(r.line == 23);
    CHECK(r.msg == "123");
    CHECK(r.severity == chobo::log::Severity::Debug);
    CHECK(r.scope == &scopeA);

    scopeA.SetEnabled(false);

    LogSomething();

    CHECK(sink.record.empty());

    scopeA.SetEnabled();

    LogSomething();

    REQUIRE(sink.record.size() == 1);
    CHECK(sinkChecker.counter == 2);
    sink.record.clear();
}


TEST_CASE("local basic scope")
{
    sinkChecker.counter = 0;
    chobo::log::BasicScope scope("basic", 321);
    scope.AddSink(sinkChecker);

    CHECK(scope.name == "basic");
    CHECK(scope.id == 321);
    CHECK(scope.IsEnabled());
    CHECK(scope.minSeverity == chobo::log::Severity::Debug);
    CHECK(scope.userData == 0);

    CHOBO_LOG_SCOPE(scope, Debug, 123);

    scope.SetEnabled(false);

    CHOBO_LOG_SCOPE(scope, Info, 123);

    auto& sink = TestSink::instance();
    CHECK(sink.record.empty());

    CHOBO_LOG_SCOPE(scope, Fatal, 123);

    LogSomething();

    REQUIRE(sink.record.size() == 1);
    CHECK(sinkChecker.counter == 2);
    sink.record.clear();
}