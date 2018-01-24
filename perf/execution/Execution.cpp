#define PICOBENCH_IMPLEMENT
#include "picobench/picobench.hpp"

#include <iostream>

int main()
{
    picobench::runner r;

//#if defined(NDEBUG)
    //r.set_default_samples(2);
    r.set_default_state_iterations({ 50000 });

    auto report = r.run_benchmarks();

    report.to_text(std::cout);

    return 0;
}
