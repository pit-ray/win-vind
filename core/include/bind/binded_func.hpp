#ifndef _BINDED_FUNC_HPP
#define _BINDED_FUNC_HPP

#include <memory>
#include <string>

#include "mode.hpp"

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
        const std::size_t& id() const noexcept ;

        static std::size_t name_to_id(const std::string& name) noexcept {
            return std::hash<std::string>()(name) ;
        }
        static std::size_t name_to_id(std::string&& name) noexcept {
            return std::hash<std::string>()(std::move(name)) ;
        }

        void process() const ;
        void process(NTypeLogger& parent_lgr) const ;
        void process(const CharLogger& parent_lgr) const ;

        virtual bool is_for_moving_caret() const noexcept ;
        virtual void reconstruct() ;

        bool operator==(const BindedFunc& rhs) const noexcept ;
        bool operator==(BindedFunc&& rhs) const noexcept ;
        bool operator!=(const BindedFunc& rhs) const noexcept ;
        bool operator!=(BindedFunc&& rhs) const noexcept ;
    } ;
}
#endif
