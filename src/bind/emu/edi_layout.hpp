#ifndef _EDI_LAYOUT_HPP
#define _EDI_LAYOUT_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        class JoinNextLine : public BindedFuncCreator<JoinNextLine> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit JoinNextLine() ;
            virtual ~JoinNextLine() noexcept ;

            JoinNextLine(JoinNextLine&&) ;
            JoinNextLine& operator=(JoinNextLine&&) ;
            JoinNextLine(const JoinNextLine&)             = delete ;
            JoinNextLine& operator=(const JoinNextLine&)  = delete ;

            bool is_for_changing_text() const noexcept override {
                return true ;
            }
        } ;
    }
}

#endif
