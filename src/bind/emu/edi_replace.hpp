#ifndef _EDI_REPLACE_HPP
#define _EDI_REPLACE_HPP

#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        class ReplaceChar : public ChangeBaseCreator<ReplaceChar> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit ReplaceChar() ;
            virtual ~ReplaceChar() noexcept ;

            void sprocess() const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            ReplaceChar(ReplaceChar&&) ;
            ReplaceChar& operator=(ReplaceChar&&) ;

            ReplaceChar(const ReplaceChar&) = delete ;
            ReplaceChar& operator=(const ReplaceChar&) = delete ;
        } ;

        class ReplaceSequence : public ChangeBaseCreator<ReplaceSequence> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit ReplaceSequence() ;
            virtual ~ReplaceSequence() noexcept ;

            void sprocess(unsigned int repeat_num=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            ReplaceSequence(ReplaceSequence&&) ;
            ReplaceSequence& operator=(ReplaceSequence&&) ;

            ReplaceSequence(const ReplaceSequence&) = delete ;
            ReplaceSequence& operator=(const ReplaceSequence&) = delete ;
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
