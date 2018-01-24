#include "picobench/picobench.hpp"

#include <fstream>

static void fstream(picobench::state& s)
{
    std::ofstream fout("fstream.txt");
    int i = 0;
    for (auto _ : s) {
        fout << "***fstream message # " << i << ": Some other text here\n";
        ++i;
    }
}
PICOBENCH(fstream);