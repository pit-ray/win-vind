#include "key_binding.hpp"
#include "msg_logger.hpp"
#include "key_log.hpp"
#include "key_logger.hpp"
#include "virtual_key_fwd.hpp"

#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>

using namespace std ;

struct KeyBinding::Impl
{
    //this vector is stored, "|" > "->" > "+" > "unsigned char"
    BindingsLoader::vvvc_t vvvkc ;


    bool callable ;
    bool exist_code ;
    std::mutex mtx ;

    explicit Impl() noexcept
    : vvvkc(),
      callable(false),
      exist_code(true),
      mtx()
    {}

    ~Impl() noexcept {
        vvvkc.clear() ;
    }

    //move
    Impl(Impl&&) noexcept = default ;
    Impl& operator=(Impl&&) noexcept = default ;

    //copy
    Impl(const Impl&) = delete ;
    Impl& operator=(const Impl&) = delete ;
} ;


KeyBinding::KeyBinding()
: pimpl(make_unique<Impl>())
{}

KeyBinding::~KeyBinding() noexcept = default ;

//move
KeyBinding::KeyBinding(KeyBinding&&) noexcept = default ;
KeyBinding& KeyBinding::operator=(KeyBinding&&) noexcept = default ;

//methods
void KeyBinding::set_command(const BindingsLoader::vvvc_t& command) noexcept
{
    if(!pimpl->vvvkc.empty()) return ; //only once registerable
    pimpl->vvvkc = command ;
}

std::size_t KeyBinding::matched_num(const KeyLog& log, const std::size_t seq_index) const noexcept
{
    lock_guard<mutex> lock(pimpl->mtx) ;

    
    pimpl->callable = false ;
    if(pimpl->vvvkc.empty()) { //not registered
        return 0 ;
    }
    if(seq_index == 0) { //initialize
        pimpl->exist_code = true ;
    }
    if(!pimpl->exist_code) { //not needed searching.
        return 0 ;
    }
    std::size_t max_matched_num{0} ;
    auto at_least_exist = false ;

    const BindingsLoader::vc_t* pvkc = nullptr ;
    for(const auto& vvkc : pimpl->vvvkc) {
        std::size_t matched_num{0} ;

        try {pvkc = &vvkc.at(seq_index) ;}
        catch(out_of_range&) {continue ;}

        for(const auto& kc : *pvkc) { //same time pressing
            if(log.is_containing(kc)) {
                matched_num ++ ;
            }
        }
        if(matched_num != pvkc->size()) {
            continue ;
        }
        at_least_exist = true ;

        if(max_matched_num < matched_num) {
            max_matched_num = matched_num ;
        }
        if(seq_index == (vvkc.size() - 1)) {
            pimpl->callable = true ;
        }
    }

    pimpl->exist_code = at_least_exist ;
    return max_matched_num ;
}

bool KeyBinding::is_callable() const noexcept
{
    return pimpl->callable ;
}

void KeyBinding::process(const bool first_call) const
{
    if(!do_process(first_call)) {
        ERROR_PRINT("do_process is failed (" + name() + ")") ;
    }
}
