#include "picobench/picobench.hpp"

#include "spdlog/spdlog.h"

static void spd_log(picobench::state& s)
{
    namespace spd = spdlog;

    auto log = spdlog::create<spd::sinks::simple_file_sink_st>("file_logger", "spdlog.txt", true);
    log->set_pattern("%v");

    int i = 0;
    for (auto _ : s) {
        log->info("spdlog message #{}: Some other text here", i);
        ++i;
    }
    
    spdlog::drop_all();
}
PICOBENCH(spd_log);