#include "util/debug.hpp"

#include <chrono>

namespace vind
{
    namespace util {
        namespace debug {
#ifdef DEBUG
            static std::chrono::system_clock::time_point tp ;
            void bench_start() {
                tp = std::chrono::system_clock::now() ;
            } ;

            int bench_stop() {
                using namespace std::chrono ;
                auto delta = system_clock::now() - tp ;
                auto du = duration_cast<milliseconds>(delta) ;
                return static_cast<int>(du.count()) ;
            } ;
#endif
        }
    }
}
