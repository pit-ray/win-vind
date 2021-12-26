#ifndef _DEBUG_HPP
#define _DEBUG_HPP

#if defined(DEBUG)
namespace vind
{
    namespace util {
        void bench_start() ;
        int bench_stop() ;
    }
}

#include <iostream>

#endif

#endif
