#ifndef _EDI_LAYOUT_HPP
#define _EDI_LAYOUT_HPP

#include "bind/binded_func_with_creator.hpp"

namespace vind
{
    class EdiNRemoveEOL : public BindedFuncWithCreator<EdiNRemoveEOL> {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        void sprocess(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* const parent_vkclgr,
                const CharLogger* const parent_charlgr) const ;
        static const std::string sname() noexcept ;

        explicit EdiNRemoveEOL() ;
        virtual ~EdiNRemoveEOL() noexcept ;

        EdiNRemoveEOL(EdiNRemoveEOL&&) ;
        EdiNRemoveEOL& operator=(EdiNRemoveEOL&&) ;
        EdiNRemoveEOL(const EdiNRemoveEOL&)             = delete ;
        EdiNRemoveEOL& operator=(const EdiNRemoveEOL&)  = delete ;
    } ;
}

#endif
