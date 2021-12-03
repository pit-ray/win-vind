#include "logpooler.hpp"

#include "key_absorber.hpp"
#include "key_log.hpp"
#include "mapgate.hpp"

#include <queue>


#if defined(DEBUG)

#include "key_logger_base.hpp"

#include <iostream>

#endif


namespace vind
{
    namespace core
    {
        struct LogPooler::Impl {
            std::queue<KeyLog> pool_{} ;
        } ;

        LogPooler::LogPooler()
        : pimpl(std::make_unique<Impl>())
        {}

        LogPooler::~LogPooler() noexcept = default ;

        LogPooler& LogPooler::get_instance() {
            static LogPooler instance ;
            return instance ;
        }

        void LogPooler::push_log(const KeyLog& log) {
            pimpl->pool_.push(log) ;
        }

        void LogPooler::push_log(KeyLog&& log) {
            pimpl->pool_.push(std::move(log)) ;
        }

        KeyLog LogPooler::pop_log() {
            KeyLog log ;
            if(pimpl->pool_.empty()) {
                log = get_pressed_list() ;

                auto logs = MapGate::get_instance().map_logger(log) ;
                if(!logs.empty()) {
                    auto itr = std::make_move_iterator(logs.begin()) ;
                    log = std::move(*itr) ;
                    itr ++ ;

                    //
                    // To simulate the input, make a state transition with an empty log.
                    // This is to make the logger recognize that it is a key release,
                    // not a long pressing of the key.
                    //
                    while(itr != std::make_move_iterator(logs.end())) {
                        pimpl->pool_.emplace() ;
                        pimpl->pool_.push(std::move(*itr)) ;
                        itr ++ ;
                    } 
                }
            }
            else {
                log = std::move(pimpl->pool_.front()) ;
                pimpl->pool_.pop() ;
            }

            return log ;
        }
    }
}
