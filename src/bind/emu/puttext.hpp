#ifndef _EDI_PUT_HPP
#define _EDI_PUT_HPP

#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        //Paste
        class PutAfter : public ChangeBaseCreator<PutAfter> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit PutAfter() ;
            virtual ~PutAfter() noexcept ;

            PutAfter(PutAfter&&) ;
            PutAfter& operator=(PutAfter&&) ;
            PutAfter(const PutAfter&)            = delete ;
            PutAfter& operator=(const PutAfter&) = delete ;
        } ;

        class PutBefore : public ChangeBaseCreator<PutBefore> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit PutBefore() ;
            virtual ~PutBefore() noexcept ;

            PutBefore(PutBefore&&) ;
            PutBefore& operator=(PutBefore&&) ;
            PutBefore(const PutBefore&)            = delete ;
            PutBefore& operator=(const PutBefore&) = delete ;
        } ;
    }
}

#endif
