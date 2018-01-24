#include "picobench/picobench.hpp"

#include <chobo/Log.h>
#include <chobo/log/Sink.h>

#include <cstdio>

class SimpleSink : public chobo::log::Sink
{
public:
    SimpleSink(const char* filename)
    {
        m_fp = fopen(filename, "w");
    }

    ~SimpleSink()
    {
        fclose(m_fp);
    }

    virtual void Record(
        chobo::log::ScopeData&,
        const std::chrono::time_point<std::chrono::system_clock>&,
        chobo::log::Severity::Type,
        const char*,
        int,
        const char* msg) override
    {
        fprintf(m_fp, "%s\n", msg);
    }

private:
    FILE* m_fp;
};

static void chobo_log(picobench::state& s)
{
    SimpleSink sink("chobo-log.txt");
    chobo::log::BasicScope scope("test");
    scope.AddSink(sink);
    
    int i = 0;
    for (auto _ : s) {
        CHOBO_LOG_SCOPE(scope, Info, "chobo-log message # ", i, ": Some other text here");
        ++i;
    }
}
PICOBENCH(chobo_log).baseline();