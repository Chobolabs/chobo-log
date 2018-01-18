//
// chobo-log
// Copyright (c) 2016-2018 Chobolabs Inc.
// http://www.chobolabs.com/
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include <iostream>

#define CHOBO_LOG_MIN_SEVERITY CHOBO_LOG_SEVERITY_DEBUG

#include <chobo/Log.h>
#include <chobo/log/LogManager.h>
#include <chobo/log/sinks/ConsoleSink.h>
#include <chobo/log/ext/DelayedLogger.h>

using namespace std;

class RenderingScope : public chobo::log::GlobalScope
{
public:
    RenderingScope() : GlobalScope("Rendering", 1) {}
} Rendering;

//CHOBO_LOG_HARD_DISABLE_SCOPE_TYPE(RenderingScope);

//CHOBO_LOG_SCOPE_HARD_MIN_SEVERITY_ERROR(RenderingScope);

int main()
{
    unique_ptr<chobo::log::Sink> sink(new chobo::log::ConsoleSink);
    chobo::log::LogManager::GetInstance().AddGlobalSink(*sink);

    CHOBO_LOG_DEFAULT_DELAYED_STREAM(Error, err);
    err << "Something\n";

    int F = 12321;
    CHOBO_LOG_DEFAULT(Debug, "Couldn't open file: ", F, " because it's silly. Error code: ", 13);

    CHOBO_LOG_DEFAULT_STREAM(Error) << "!!! Error" << 34;

    CHOBO_LOG_SCOPE(Rendering, Fatal, "Fatal error", 123);

    CHOBO_LOG_SCOPE(Rendering, Debug, "Debug stuff ", 1123);

    err << "Something else!\n";

    for (int i = 0; i < 10; ++i)
    {
        CHOBO_LOG_ONCE_DEFAULT(Debug, "I'm in yer loop");
    }

    err << "Something totally different";

    {
        auto log = CHOBO_LOG_DEFAULT_STREAM(Info);
        log << "info " << 123;
        log << " -- more info";
    }

    return 0;
}