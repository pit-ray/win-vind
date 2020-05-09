#include "command.hpp"
#include "msg_logger.hpp"
#include "key_log.hpp"
#include "key_logger.hpp"
#include "virtual_key_fwd.hpp"

#include <iostream>
#include <mutex>
#include <vector>
#include <algorithm>


//for debug
#include "vkc_converter.hpp"

using namespace std ;

struct Command::Impl
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


Command::Command()
: pimpl(make_unique<Impl>())
{}

Command::~Command() noexcept = default ;

//move
Command::Command(Command&&) noexcept = default ;
Command& Command::operator=(Command&&) noexcept = default ;

//methods
void Command::set_command(const XMLParser::vvvc_t& command) noexcept
{
    if(!pimpl->vvvkc.empty()) return ; //only once registerable
    pimpl->vvvkc = command ;
}

bool Command::is_callable(const KeyLogger& logger) const noexcept
{
    lock_guard<mutex> lock(pimpl->mtx) ;

    //still not registering keys
    if(pimpl->vvvkc.empty()) {
        return false ;
    }

    //or-types

    for(const auto& vvkc : pimpl->vvvkc) {
        for(size_t i = 0 ; i < logger.size() ; i ++) {
            try {
                const auto keyvec = vvkc.at(i) ;
                const auto log = logger.at(i) ;
                if(std::all_of(keyvec.cbegin(), keyvec.cend(), [&log](const auto& key) {
                    if(key == VKC_OPTIONAL) {
                        throw VKC_OPTIONAL ;
                    }

                    return std::find(log.cbegin(), log.cend(), key) != log.cend() ;
                })) {
                    if(i == vvkc.size() - 1) {
                        if(i == logger.size() - 1) {
                            return true ;
                        }
                    }
                    continue ;
                }

                //logger's command is different from set command.
                break ;
            }
            catch(const out_of_range&) {
                continue ;
            }
            catch(const int& code) {
                return code == VKC_OPTIONAL ? true : false ;
            }
        }
    }
    return false ;
}

void Command::process(const string& cmd) const
{
    lock_guard <mutex> lock(pimpl->mtx) ;

    //preprocess
    if(!do_process(cmd)) {
        //error
        Logger::error_stream << "[Error] do_process is failed " \
        << "(" << name() << ")\n" ;
    }
    //postpcess
}