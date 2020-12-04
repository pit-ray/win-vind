#ifndef _BINDED_FUNC_HPP
#define _BINDED_FUNC_HPP

#include <memory>
#include <string>

#include "key_matcher.hpp"
#include "mode_manager.hpp"

class BindedFunc
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    virtual void do_process(
            const bool first_call,
            const unsigned int repeat_num,
            const KeyLogger* parent_vkclgr,
            const KeyLogger* const parent_charlgr) const = 0 ;

public:
    using shp_t = std::shared_ptr<BindedFunc> ;

    explicit BindedFunc() ;
    virtual ~BindedFunc() noexcept ;

    BindedFunc(BindedFunc&&) ;
    BindedFunc& operator=(BindedFunc&&) ;
    BindedFunc(const BindedFunc&)            = delete ;
    BindedFunc& operator=(const BindedFunc&) = delete ;

    virtual const std::string name() const noexcept = 0 ;

    void process(
            const bool first_call=true,
            const unsigned int repeat_num=1,
            const KeyLogger* parent_vkclgr=nullptr,
            const KeyLogger* const parent_charlgr=nullptr) const ;

    void register_matcher(
            const ModeManager::Mode mode,
            const KeyMatcher::shp_t matcher) const ;
    void register_matcher(
            const unsigned char mode,
            const KeyMatcher::shp_t matcher) const ;

    //[return] matched num in latest log
    unsigned int validate_if_match(
            const KeyLogger& logger,
            ModeManager::Mode mode=ModeManager::get_mode()) const ;

    bool is_callable() const noexcept ;
    virtual bool is_for_moving_caret() const noexcept ;
} ;
#endif
