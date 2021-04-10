#ifndef _BINDED_FUNC_HPP
#define _BINDED_FUNC_HPP

#include <memory>
#include <string>

#include "bind/base/bindings_matcher.hpp"
#include "bind/base/mode.hpp"
#include "bind/base/keycode_logger.hpp"
#include "bind/base/char_logger.hpp"

namespace vind
{
    class BindedFunc {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        virtual void do_process(
                bool first_call,
                unsigned int repeat_num,
                KeycodeLogger* parent_keycodelgr,
                const CharLogger* const parent_charlgr) const = 0 ;

    public:
        using SPtr = std::shared_ptr<BindedFunc> ;

        explicit BindedFunc() ;
        virtual ~BindedFunc() noexcept ;

        BindedFunc(BindedFunc&&) ;
        BindedFunc& operator=(BindedFunc&&) ;
        BindedFunc(const BindedFunc&)            = delete ;
        BindedFunc& operator=(const BindedFunc&) = delete ;

        virtual const std::string name() const noexcept = 0 ;

        void process(
                bool first_call=true,
                unsigned int repeat_num=1,
                KeycodeLogger* parent_keycodelgr=nullptr,
                const CharLogger* const parent_charlgr=nullptr) const ;

        void register_matcher(
                const mode::Mode mode,
                const BindingsMatcher::SPtr matcher) const ;
        void register_matcher(
                const unsigned char mode,
                const BindingsMatcher::SPtr matcher) const ;

        //[return] matched num in latest log
        unsigned int validate_if_match(
                const KeyLoggerBase& pc_lgr,
                mode::Mode mode=mode::get_global_mode()) const ;

        //[return] matched num in latest log
        //The cost for computing is bigger than validate_if_match.
        unsigned int validate_if_fullmatch(
                const KeyLoggerBase& pc_lgr,
                mode::Mode mode=mode::get_global_mode()) const ;

        bool is_callable() const noexcept ;
        virtual bool is_for_moving_caret() const noexcept ;

        bool is_matched_syskey_in_combined_bindings() const noexcept ;

        virtual void load_config() ;
    } ;
}
#endif
