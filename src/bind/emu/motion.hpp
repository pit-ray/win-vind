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
            void sprocess(
                std::uint16_t count, const std::string& args) ;

            explicit YankWithMotion() ;
            virtual ~YankWithMotion() noexcept ;

            YankWithMotion(YankWithMotion&&) ;
            YankWithMotion& operator=(YankWithMotion&&) ;
            YankWithMotion(const YankWithMotion&)            = delete ;
            YankWithMotion& operator=(const YankWithMotion&) = delete ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;


        class DeleteWithMotion : public BindedFuncVoid<DeleteWithMotion> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(
                std::uint16_t count, const std::string& args) ;

            explicit DeleteWithMotion() ;

            virtual ~DeleteWithMotion() noexcept ;
            DeleteWithMotion(DeleteWithMotion&&) ;
            DeleteWithMotion& operator=(DeleteWithMotion&&) ;
            DeleteWithMotion(const DeleteWithMotion&)            = delete ;
            DeleteWithMotion& operator=(const DeleteWithMotion&) = delete ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;

        class ChangeWithMotion : public BindedFuncVoid<ChangeWithMotion> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(
                std::uint16_t count, const std::string& args) ;

            explicit ChangeWithMotion() ;

            virtual ~ChangeWithMotion() noexcept ;
            ChangeWithMotion(ChangeWithMotion&&) ;
            ChangeWithMotion& operator=(ChangeWithMotion&&) ;
            ChangeWithMotion(const ChangeWithMotion&)            = delete ;
            ChangeWithMotion& operator=(const ChangeWithMotion&) = delete ;

            bool is_mode_modifiable() const noexcept override {
                return true ;
            }
        } ;
    }
}

#endif
