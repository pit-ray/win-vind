#ifndef _BINDED_FUNC_HPP
#define _BINDED_FUNC_HPP

#include <memory>
#include <string>

#include "bind/base/mode.hpp"

namespace vind
{
    class NTypeLogger ;
    class CharLogger ;

    class BindedFunc {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        virtual void error_process(const std::exception& e) const ;

        virtual void do_process() const = 0 ;
        virtual void do_process(NTypeLogger& parent_lgr) const = 0 ;
        virtual void do_process(const CharLogger& parent_lgr) const = 0 ;

    public:
        using SPtr = std::shared_ptr<BindedFunc> ;

        explicit BindedFunc() ;
        explicit BindedFunc(const std::string& name) ;
        explicit BindedFunc(std::string&& name) ;
        virtual ~BindedFunc() noexcept ;

        BindedFunc(BindedFunc&&) ;
        BindedFunc& operator=(BindedFunc&&) ;
        BindedFunc(const BindedFunc&)            = delete ;
        BindedFunc& operator=(const BindedFunc&) = delete ;

        const std::string& name() const noexcept ;
        std::size_t id() const noexcept ;

        void process() const ;
        void process(NTypeLogger& parent_lgr) const ;
        void process(const CharLogger& parent_lgr) const ;

        virtual bool is_for_moving_caret() const noexcept ;
        virtual void load_config() ;
    } ;
}
#endif
