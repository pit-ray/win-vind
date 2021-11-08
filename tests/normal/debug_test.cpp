#include <doctest.h>

#include "util/debug.cpp"

#ifdef DEBUG
using namespace vind::util ;
TEST_CASE("(util::debug) Benchmark Return Test") {
    bench_start() ;
    for(int i = 0 ; i < 10000000 ; i ++) ;
    CHECK_GT(bench_stop(), 0) ;
}
#endif
