#include "search.hpp"

#include "core/inputgate.hpp"
#include "util/debug.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace bind
    {
        //SearchPattern
        SearchPattern::SearchPattern()
        : BindedFuncVoid("search_pattern")
        {}
        void SearchPattern::sprocess(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            core::InputGate::get_instance().pushup(KEYCODE_F3) ;
        }
    }
}
