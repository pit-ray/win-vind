#ifndef _EDI_PUT_HPP
#define _EDI_PUT_HPP

#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        //Paste
        struct PutAfter : public ChangeBaseCreator<PutAfter> {
            explicit PutAfter() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;

        struct PutBefore : public ChangeBaseCreator<PutBefore> {
            explicit PutBefore() ;
            static void sprocess(
                std::uint16_t count, const std::string& args) ;
        } ;
    }
}

#endif
