#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        struct ReplaceChar : public ChangeBaseCreator<ReplaceChar> {
            explicit ReplaceChar() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct ReplaceSequence : public ChangeBaseCreator<ReplaceSequence> {
            explicit ReplaceSequence() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        struct SwitchCharCase : public ChangeBaseCreator<SwitchCharCase> {
            explicit SwitchCharCase() ;
            static void sprocess(unsigned int repeat_num=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;
    }
}

#endif
