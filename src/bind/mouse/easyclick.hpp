#ifndef _EASYCLICK_HPP
#define _EASYCLICK_HPP

#include "bind/bindedfunc.hpp"

#include <memory>


namespace vind
{
    namespace bind
    {
        class EasyClick : public BindedFuncVoid<EasyClick> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit EasyClick() ;
            virtual ~EasyClick() noexcept ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            EasyClick(EasyClick&&) ;
            EasyClick& operator=(EasyClick&&) ;
            EasyClick(const EasyClick&)            = delete ;
            EasyClick& operator=(const EasyClick&) = delete ;

            void reconstruct() override ;
        } ;


        class EasyClickAll : public BindedFuncVoid<EasyClickAll> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit EasyClickAll() ;
            virtual ~EasyClickAll() noexcept ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            EasyClickAll(EasyClickAll&&) ;
            EasyClickAll& operator=(EasyClickAll&&) ;
            EasyClickAll(const EasyClickAll&)            = delete ;
            EasyClickAll& operator=(const EasyClickAll&) = delete ;

            void reconstruct() override ;
        } ;
    }
}

#endif
