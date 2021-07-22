#ifndef _ASYNC_UIA_CACHE_BUILDER_HPP
#define _ASYNC_UIA_CACHE_BUILDER_HPP

#include "option_creator.hpp"

namespace vind
{
    class AsyncUIACacheBuilder : public OptionCreator<AsyncUIACacheBuilder>
    {
    private:
        void do_enable() const override ;
        void do_disable() const override ;
        void do_process() const override ;

    public:
        static std::string sname() noexcept ;

    } ;
}

#endif
