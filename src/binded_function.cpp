#include "binded_function.hpp"
#include "msg_logger.hpp"
#include "key_log.hpp"
#include "key_logger.hpp"
#include "virtual_key_fwd.hpp"

#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>

using namespace std ;

struct BindedFunction::Impl
{
    //this vector is stored, "|" > "->" > "+" > "unsigned char"
    XMLParser::vvvc_t vvvkc ;

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
    Impl(Impl&&) = default ;
    Impl& operator=(Impl&&) = default ;

    //copy
    Impl(const Impl&) = delete ;
    Impl& operator=(const Impl&) = delete ;
} ;


BindedFunction::BindedFunction()
: pimpl(make_unique<Impl>())
{}

BindedFunction::~BindedFunction() noexcept = default ;

//move
BindedFunction::BindedFunction(BindedFunction&&) noexcept = default ;
BindedFunction& BindedFunction::operator=(BindedFunction&&) noexcept = default ;

//methods
void BindedFunction::set_command(const XMLParser::vvvc_t& command) noexcept
{
    if(!pimpl->vvvkc.empty()) return ; //only once registerable
    pimpl->vvvkc = command ;
}

size_t BindedFunction::existed_num_and_update(const KeyLog& log, const size_t seq_index) const noexcept
{
    lock_guard<mutex> lock(pimpl->mtx) ;

    pimpl->callable = false ;

    //still not registering keys
    if(pimpl->vvvkc.empty()) {
        return 0 ;
    }

    if(seq_index == 0) {
        pimpl->exist_code = true ;
    }

    //not needed searching.
    if(!pimpl->exist_code) {
        return 0 ;
    }

    size_t max_matched_num{0} ;
    auto at_least_exist = false ;

    //or-types

    //pvkc is pointer of code-vec.
    //I use vvkc.at in order to get code-vec.
    //However, [at] is exceptable.
    //I want try-identifier to be only-one, thus, use pointer.
    const XMLParser::vc_t* pvkc = nullptr ;
    for(const auto& vvkc : pimpl->vvvkc) {
        size_t matched_num{0} ;

        try {pvkc = &vvkc.at(seq_index) ;}
        catch(const out_of_range&) {
            continue ;
        }

        //sequence-types
        for(const auto& kc : *pvkc) {
            if(find(log.cbegin(), log.cend(), kc) != log.cend()) {
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

bool BindedFunction::is_callable() const noexcept
{
    return pimpl->callable ;
}

void BindedFunction::process(const bool first_call) const
{
    lock_guard <mutex> lock(pimpl->mtx) ;

    //preprocess
    if(!do_process(first_call)) {
        //error
        Logger::error_stream << "[Error] do_process is failed " \
        << "(" << name() << ")\n" ;
    }
    //postpcess
}