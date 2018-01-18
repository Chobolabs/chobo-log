//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

#include "test_sink/TestSink.h"
#include <chobo/Log.h>
#include <chobo/log/LogManager.h>
#include <chobo/log/ext/DelayedLogger.h>

#include <iomanip>

#include "Scopes.h"

TEST_SUITE("chobo-log");

TEST_CASE("severity")
{
    using namespace chobo::log;
    static_assert(Severity::Count == 5, "Tests support only five severities");
    static_assert(Severity::Debug == 0, "Debug");
    static_assert(Severity::Info == 1, "Info");
    static_assert(Severity::Warning == 2, "Warning");
    static_assert(Severity::Error == 3, "Error");
    static_assert(Severity::Fatal == 4, "Fatal");

    CHECK(Severity::ToString(Severity::Debug) == "Debug");
    CHECK(Severity::ToString(Severity::Info) == "Info");
    CHECK(Severity::ToString(Severity::Warning) == "Warning");
    CHECK(Severity::ToString(Severity::Error) == "Error");
    CHECK(Severity::ToString(Severity::Fatal) == "Fatal");
    CHECK(Severity::ToString(Severity::Count) == "?");
}

struct CustomIO
{
};

std::ostream& operator<<(std::ostream& out, const CustomIO& c)
{
    out << "CustomIO";
    return out;
}

TEST_CASE("default")
{
    auto& sink = TestSink::instance();

    auto& dscope = chobo::log::DefaultScope::Get();
    CHECK(dscope.name == "Default");
    CHECK(dscope.id == 0);
    CHECK(dscope.IsEnabled());
    CHECK(dscope.minSeverity == chobo::log::Severity::Debug);
    CHECK(dscope.userData == 0);

    CHOBO_LOG_DEFAULT(Debug, "hello");
    REQUIRE(sink.record.size() == 1);
    auto r = sink.pop();
    REQUIRE(sink.record.empty());
    
    auto now = std::chrono::system_clock::now();
    auto duration = r.timestamp - now;
    // can't be more than one
    CHECK(std::chrono::duration_cast<std::chrono::seconds>(duration).count() < 2);
    CHECK(r.file == __FILE__);
    CHECK(r.line == 64);
    CHECK(r.msg == "hello");
    CHECK(r.severity == chobo::log::Severity::Debug);
    CHECK(r.scope == &dscope);

    CHOBO_LOG_DEFAULT(Info, 123, 'a', false);
    r = sink.pop();
    duration = r.timestamp - now;
    // can't be more than one
    CHECK(std::chrono::duration_cast<std::chrono::seconds>(duration).count() < 2);
    CHECK(r.file == __FILE__);
    CHECK(r.line == 79);
    CHECK(r.severity == chobo::log::Severity::Info);
    CHECK(r.scope == &dscope);
    CHECK(r.msg == "123a0");

    CustomIO c;
    CHOBO_LOG_DEFAULT(Warning, "Custom: ", c);
    r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Warning);
    CHECK(r.msg == "Custom: CustomIO");

    CHOBO_LOG_DEFAULT_STREAM(Error) << "Stream: " << std::boolalpha << true;
    r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Error);
    CHECK(r.msg == "Stream: true");

    for (int i = 0; i < 10; ++i)
    {
        CHOBO_LOG_ONCE_DEFAULT(Fatal, 'f');
    }
    r = sink.pop();
    CHECK(sink.record.empty());
    CHECK(r.severity == chobo::log::Severity::Fatal);
    CHECK(r.msg == "f");

    {
        CHOBO_LOG_DEFAULT_DELAYED_STREAM(Info, info);
        info << 1;
        CHECK(sink.record.empty());
        CHOBO_LOG_DEFAULT(Debug, "internal");
        info << 2;
        CHECK(sink.record.size() == 1);
    }

    CHECK(sink.record.size() == 2);
    r = sink.pop();
    CHECK(r.msg == "internal");
    CHECK(r.severity == chobo::log::Severity::Debug);

    r = sink.pop();
    duration = r.timestamp - now;
    // can't be more than one
    CHECK(std::chrono::duration_cast<std::chrono::seconds>(duration).count() < 2);
    CHECK(r.file == __FILE__);
    CHECK(r.line == 111);
    CHECK(r.severity == chobo::log::Severity::Info);
    CHECK(r.scope == &dscope);
    CHECK(r.msg == "12");

    auto& logMan = chobo::log::LogManager::GetInstance();
    logMan.SetGlobalMinSeverity(chobo::log::Severity::Info);
    CHOBO_LOG_DEFAULT(Debug, 123);
    CHECK(sink.record.empty());

    logMan.SetGlobalMinSeverity(chobo::log::Severity::Debug);
    CHOBO_LOG_DEFAULT(Debug, 567);
    REQUIRE(!sink.record.empty());
    r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Debug);
    CHECK(r.msg == "567");
}

TEST_CASE("scope")
{
    auto& sink = TestSink::instance();

    chobo::log::LocalScope scope("myscope", 54);
    scope.userData = 32;

    CHECK(scope.name == "myscope");
    CHECK(scope.id == 54);
    CHECK(scope.IsEnabled());
    CHECK(scope.minSeverity == chobo::log::Severity::Debug);
    CHECK(scope.userData == 32);

    CHOBO_LOG_SCOPE(scope, Debug, "hello");
    REQUIRE(sink.record.size() == 1);
    auto r = sink.pop();
    REQUIRE(sink.record.empty());

    auto now = std::chrono::system_clock::now();
    auto duration = r.timestamp - now;
    // can't be more than one
    CHECK(std::chrono::duration_cast<std::chrono::seconds>(duration).count() < 2);
    CHECK(r.file == __FILE__);
    CHECK(r.line == 160);
    CHECK(r.msg == "hello");
    CHECK(r.severity == chobo::log::Severity::Debug);
    CHECK(r.scope == &scope);

    CHOBO_LOG_SCOPE(scope, Info, 123, 'a', false);
    r = sink.pop();
    duration = r.timestamp - now;
    // can't be more than one
    CHECK(std::chrono::duration_cast<std::chrono::seconds>(duration).count() < 2);
    CHECK(r.file == __FILE__);
    CHECK(r.line == 175);
    CHECK(r.severity == chobo::log::Severity::Info);
    CHECK(r.scope == &scope);
    CHECK(r.msg == "123a0");

    CustomIO c;
    CHOBO_LOG_SCOPE(scope, Warning, "Custom: ", c);
    r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Warning);
    CHECK(r.msg == "Custom: CustomIO");

    CHOBO_LOG_SCOPE_STREAM(scope, Error) << "Stream: " << std::boolalpha << true;
    r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Error);
    CHECK(r.msg == "Stream: true");

    for (int i = 0; i < 10; ++i)
    {
        CHOBO_LOG_ONCE_SCOPE(scope, Fatal, 'f');
    }
    r = sink.pop();
    CHECK(sink.record.empty());
    CHECK(r.severity == chobo::log::Severity::Fatal);
    CHECK(r.msg == "f");

    {
        CHOBO_LOG_SCOPE_DELAYED_STREAM(scope, Info, info);
        info << 1;
        CHECK(sink.record.empty());
        CHOBO_LOG_SCOPE(scope, Debug, "internal");
        info << 2;
        CHECK(sink.record.size() == 1);
    }

    CHECK(sink.record.size() == 2);
    r = sink.pop();
    CHECK(r.msg == "internal");
    CHECK(r.severity == chobo::log::Severity::Debug);

    r = sink.pop();
    duration = r.timestamp - now;
    // can't be more than one
    CHECK(std::chrono::duration_cast<std::chrono::seconds>(duration).count() < 2);
    CHECK(r.file == __FILE__);
    CHECK(r.line == 207);
    CHECK(r.severity == chobo::log::Severity::Info);
    CHECK(r.scope == &scope);
    CHECK(r.msg == "12");

    auto& logMan = chobo::log::LogManager::GetInstance();
    logMan.SetGlobalMinSeverity(chobo::log::Severity::Info);
    CHOBO_LOG_SCOPE(scope, Debug, 123);
    CHECK(sink.record.empty());

    logMan.SetGlobalMinSeverity(chobo::log::Severity::Debug);
    CHOBO_LOG_SCOPE(scope, Debug, 567);
    REQUIRE(!sink.record.empty());
    r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Debug);
    CHECK(r.msg == "567");

    scope.minSeverity = chobo::log::Severity::Error;
    CHOBO_LOG_SCOPE(scope, Info, "hi");
    CHECK(sink.record.empty());
    
    scope.minSeverity = chobo::log::Severity::Debug;
    CHOBO_LOG_SCOPE(scope, Info, "bi");
    REQUIRE(!sink.record.empty());
    r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Info);
    CHECK(r.msg == "bi");

    scope.SetEnabled(false);
    CHOBO_LOG_SCOPE(scope, Fatal, "o_O");
    CHECK(sink.record.empty());

    scope.SetEnabled(true);
    CHOBO_LOG_SCOPE(scope, Fatal, "oVO");
    REQUIRE(!sink.record.empty());
    r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Fatal);
    CHECK(r.msg == "oVO");
}

TEST_CASE("hard disabled scope")
{
    TestScopeDisabled scope;

    CHECK(scope.name == "Disabled");
    CHECK(scope.id == 3);
    CHECK(scope.IsEnabled());
    CHECK(scope.minSeverity == chobo::log::Severity::Debug);
    CHECK(scope.userData == 0);

    CHOBO_LOG_SCOPE(scope, Debug, "hello");
    CHOBO_LOG_SCOPE(scope, Info, 123, 'a', false);
    CHOBO_LOG_SCOPE(scope, Warning, "Custom: ");
    CHOBO_LOG_SCOPE_STREAM(scope, Error) << "Stream: " << std::boolalpha << true;
    for (int i = 0; i < 10; ++i)
    {
        CHOBO_LOG_ONCE_SCOPE(scope, Fatal, 'f');
    }
    {
        CHOBO_LOG_SCOPE_DELAYED_STREAM(scope, Info, info);
        info << 1;
        CHOBO_LOG_SCOPE(scope, Debug, "intenal");
        info << 2;
    }

    auto& sink = TestSink::instance();
    CHECK(sink.record.empty());
}

TEST_CASE("fatal-only scope")
{
    TestScopeFatalOnly scope;

    CHECK(scope.name == "FatalOnly");
    CHECK(scope.id == 4);
    CHECK(scope.IsEnabled());
    CHECK(scope.minSeverity == chobo::log::Severity::Debug);
    CHECK(scope.userData == 0);

    CHOBO_LOG_SCOPE(scope, Debug, "hello");
    CHOBO_LOG_SCOPE(scope, Info, 123, 'a', false);
    CHOBO_LOG_SCOPE(scope, Warning, "Custom: ");
    CHOBO_LOG_SCOPE_STREAM(scope, Error) << "Stream: " << std::boolalpha << true;
    for (int i = 0; i < 10; ++i)
    {
        CHOBO_LOG_ONCE_SCOPE(scope, Fatal, 'f');
    }
    {
        CHOBO_LOG_SCOPE_DELAYED_STREAM(scope, Info, info);
        info << 1;
        CHOBO_LOG_SCOPE(scope, Debug, "intenal");
        info << 2;
    }

    auto& sink = TestSink::instance();
    REQUIRE(sink.record.size() == 1);
    auto r = sink.pop();
    CHECK(r.severity == chobo::log::Severity::Fatal);
    CHECK(r.msg == "f");
}

TEST_CASE("enable/disable")
{
    chobo::log::LocalScope scope("myscope2", 154);
    CHOBO_LOG_DEFAULT(Debug, 123);    
    CHOBO_LOG_SCOPE(scope, Info, 123);
    auto& sink = TestSink::instance();
    CHECK(sink.record.size() == 2);
    sink.record.clear();

    scope.SetEnabled(false);
    CHOBO_LOG_DEFAULT(Debug, 123);
    CHOBO_LOG_SCOPE(scope, Info, 123);
    
    CHECK(sink.record.size() == 1);
    sink.record.clear();
    
    chobo::log::LogManager::GetInstance().SetGlobalLoggingEnabled(false);
    
    scope.SetEnabled(true);
    CHOBO_LOG_DEFAULT(Debug, 123);
    CHOBO_LOG_SCOPE(scope, Info, 123);
    chobo::log::DefaultScope::Get().SetEnabled(false);
    CHOBO_LOG_DEFAULT(Debug, 123);
    CHOBO_LOG_SCOPE(scope, Info, 123);

    chobo::log::LogManager::GetInstance().SetGlobalLoggingEnabled();

    CHECK(sink.record.empty());

    CHOBO_LOG_DEFAULT(Debug, 123);
    CHOBO_LOG_SCOPE(scope, Info, 123);

    CHECK(sink.record.size() == 1);
    auto r = sink.pop();
    CHECK(r.scope == &scope);
    chobo::log::DefaultScope::Get().SetEnabled(true);
}

int main(int argc, char** argv)
{
    chobo::log::LogManager::GetInstance().AddGlobalSink(TestSink::instance());

    doctest::Context context; // initialize

    context.applyCommandLine(argc, argv);

    return context.run();
}