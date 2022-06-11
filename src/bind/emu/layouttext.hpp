#ifndef _EDI_LAYOUT_HPP
#define _EDI_LAYOUT_HPP

#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        class JoinNextLine : public ChangeBaseCreator<JoinNextLine> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit JoinNextLine() ;
            virtual ~JoinNextLine() noexcept ;

            JoinNextLine(JoinNextLine&&) ;
            JoinNextLine& operator=(JoinNextLine&&) ;
            JoinNextLine(const JoinNextLine&)             = delete ;
            JoinNextLine& operator=(const JoinNextLine&)  = delete ;
        } ;
    }
}

#endif
