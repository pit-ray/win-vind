#ifndef _FUNC_FINDER_HPP
#define _FUNC_FINDER_HPP

#include <memory>
#include <string>

#include "key_logger_base.hpp"
#include "logger_parser.hpp"
#include "mode.hpp"

#include "bind/binded_func.hpp"

namespace vind
{
    namespace core
    {
        class FuncFinder {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit FuncFinder() ;
            explicit FuncFinder(const KeyLoggerBase& lgr) ;
            virtual ~FuncFinder() noexcept ;

            FuncFinder(const FuncFinder&) ;
            FuncFinder& operator=(const FuncFinder&) ;

            FuncFinder(FuncFinder&&) ;
            FuncFinder& operator=(FuncFinder&&) ;

            LoggerParser::SPtr transition_parser_states_in_batch(
                    const KeyLoggerBase& lgr,
                    Mode mode=get_global_mode()) ;

            void reconstruct_funcset() ;

            void search_unrejected_parser(
                    std::vector<LoggerParser::SPtr>& results,
                    Mode mode=get_global_mode()) ;

            LoggerParser::SPtr find_rejected_with_ready_parser(
                    Mode mode=get_global_mode()) ;

            LoggerParser::SPtr find_waiting_parser(
                    Mode mode=get_global_mode()) ;

            LoggerParser::SPtr find_accepted_parser(
                    Mode mode=get_global_mode()) ;

            LoggerParser::SPtr find_parser_with_transition(
                    const KeyLog& log,
                    std::size_t low_priority_func_id=0,
                    Mode mode=get_global_mode()) ;

            void backward_parser_states(
                    std::size_t n,
                    Mode mode=get_global_mode()) ;

            void reset_parser_states(
                    Mode mode=get_global_mode()) ;

            static bind::BindedFunc::SPtr find_func_byname(const std::string& name) ;
            static void load_global_bindings() ;
        } ;
    }
}

#endif
