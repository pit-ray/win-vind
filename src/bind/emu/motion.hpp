#ifndef _EDI_MOTION_HPP
#define _EDI_MOTION_HPP

#include "bind/bindedfunc.hpp"

namespace vind
{
    namespace bind
    {
        class YankWithMotion : public BindedFuncVoid<YankWithMotion> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess() ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit YankWithMotion() ;
            virtual ~YankWithMotion() noexcept ;

            YankWithMotion(YankWithMotion&&) ;
            YankWithMotion& operator=(YankWithMotion&&) ;
            YankWithMotion(const YankWithMotion&)            = delete ;
            YankWithMotion& operator=(const YankWithMotion&) = delete ;

            void reconstruct() override ;
        } ;


        class DeleteWithMotion : public BindedFuncVoid<DeleteWithMotion> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess() ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit DeleteWithMotion() ;

            virtual ~DeleteWithMotion() noexcept ;
            DeleteWithMotion(DeleteWithMotion&&) ;
            DeleteWithMotion& operator=(DeleteWithMotion&&) ;
            DeleteWithMotion(const DeleteWithMotion&)            = delete ;
            DeleteWithMotion& operator=(const DeleteWithMotion&) = delete ;

            void reconstruct() override ;
        } ;

        class ChangeWithMotion : public BindedFuncVoid<ChangeWithMotion> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess() ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit ChangeWithMotion() ;

            virtual ~ChangeWithMotion() noexcept ;
            ChangeWithMotion(ChangeWithMotion&&) ;
            ChangeWithMotion& operator=(ChangeWithMotion&&) ;
            ChangeWithMotion(const ChangeWithMotion&)            = delete ;
            ChangeWithMotion& operator=(const ChangeWithMotion&) = delete ;

            void reconstruct() override ;
        } ;
    }
}

#endif
