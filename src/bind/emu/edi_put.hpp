#ifndef _EDI_PUT_HPP
#define _EDI_PUT_HPP

#include "bind/binded_func_creator.hpp"

namespace vind
{
    namespace bind
    {
        //Paste
        class PutAfter : public BindedFuncCreator<PutAfter> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit PutAfter() ;
            virtual ~PutAfter() noexcept ;

            PutAfter(PutAfter&&) ;
            PutAfter& operator=(PutAfter&&) ;
            PutAfter(const PutAfter&)            = delete ;
            PutAfter& operator=(const PutAfter&) = delete ;

            bool is_for_changing_text() const noexcept override {
                return true ;
            }
        } ;

        class PutBefore : public BindedFuncCreator<PutBefore> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit PutBefore() ;
            virtual ~PutBefore() noexcept ;

            PutBefore(PutBefore&&) ;
            PutBefore& operator=(PutBefore&&) ;
            PutBefore(const PutBefore&)            = delete ;
            PutBefore& operator=(const PutBefore&) = delete ;

            bool is_for_changing_text() const noexcept override {
                return true ;
            }
        } ;
    }
}

#endif
