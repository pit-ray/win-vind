#ifndef _EASY_CLICK_HPP
#define _EASY_CLICK_HPP

#include "bind/bindedfunc.hpp"

#include <windows.h>

#include <memory>

#include "core/keycodedef.hpp"

namespace vind
{
    namespace bind
    {
        class EasyClickLeft : public BindedFuncVoid<EasyClickLeft> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit EasyClickLeft() ;
            virtual ~EasyClickLeft() noexcept ;

            EasyClickLeft(const EasyClickLeft&)            = delete ;
            EasyClickLeft& operator=(const EasyClickLeft&) = delete ;

            EasyClickLeft(EasyClickLeft&&) ;
            EasyClickLeft& operator=(EasyClickLeft&&) ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            void reconstruct() override ;
        } ;

        class EasyClickRight : public BindedFuncVoid<EasyClickRight> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit EasyClickRight() ;
            virtual ~EasyClickRight() noexcept ;

            EasyClickRight(const EasyClickRight&)            = delete ;
            EasyClickRight& operator=(const EasyClickRight&) = delete ;

            EasyClickRight(EasyClickRight&&) ;
            EasyClickRight& operator=(EasyClickRight&&) ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            void reconstruct() override ;
        } ;

        class EasyClickMid : public BindedFuncVoid<EasyClickMid> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit EasyClickMid() ;
            virtual ~EasyClickMid() noexcept ;

            EasyClickMid(const EasyClickMid&)            = delete ;
            EasyClickMid& operator=(const EasyClickMid&) = delete ;

            EasyClickMid(EasyClickMid&&) ;
            EasyClickMid& operator=(EasyClickMid&&) ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            void reconstruct() override ;
        } ;

        class EasyClickHover : public BindedFuncVoid<EasyClickHover> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit EasyClickHover() ;
            virtual ~EasyClickHover() noexcept ;

            EasyClickHover(const EasyClickHover&)            = delete ;
            EasyClickHover& operator=(const EasyClickHover&) = delete ;

            EasyClickHover(EasyClickHover&&) ;
            EasyClickHover& operator=(EasyClickHover&&) ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            void reconstruct() override ;
        } ;
    }
}

#endif
