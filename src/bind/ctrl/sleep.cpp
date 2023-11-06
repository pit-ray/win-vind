#include "sleep.hpp"

#include <windows.h>

#include "util/debug.hpp"
#include "util/def.hpp"
#include "util/string.hpp"


namespace vind
{
    namespace bind
    {
        Sleep::Sleep()
        : BindedFuncVoid("sleep")
        {}
        void Sleep::sprocess(
                std::uint16_t count,
                const std::string& args) {
            if(!args.empty()) {
                count = util::extract_num<std::uint16_t>(args) ;

                if(args.back() != 'm') {
                    count *= 1000 ;  // convert the seconds into miliseconds
                }
            }
            else {
                count *= 1000 ;  // assume the input is always seconds unit.
            }
            ::Sleep(static_cast<DWORD>(count)) ;
        }
    }
}
