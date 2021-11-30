#ifndef _LOGGER_PARSER_MGR_HPP
#define _LOGGER_PARSER_MGR_HPP

#include "logger_parser.hpp"

#include <memory>


namespace vind
{
    namespace core
    {
        class KeyLoggerBase ;

        class LoggerParserManager {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            LoggerParserManager() ;
            explicit LoggerParserManager(
                    const std::vector<LoggerParser::SPtr>& parsers) ;
            explicit LoggerParserManager(
                    std::vector<LoggerParser::SPtr>&& parsers) ;

            virtual ~LoggerParserManager() noexcept ;

            LoggerParserManager(LoggerParserManager&&) ;
            LoggerParserManager& operator=(LoggerParserManager&&) ;

            LoggerParser::SPtr transition_parser_states_in_batch(
                    const KeyLoggerBase& lgr) ;

            void search_unrejected_parser(
                    std::vector<LoggerParser::SPtr>& results) ;

            LoggerParser::SPtr find_rejected_with_ready_parser() ;

            LoggerParser::SPtr find_waiting_parser() ;

            LoggerParser::SPtr find_accepted_parser() ;

            LoggerParser::SPtr find_parser_with_transition(
                    const KeyLog& log,
                    std::size_t low_priority_func_id=0) ;

            void backward_parser_states(std::size_t n) ;

            void reset_parser_states() ;

            LoggerParserManager(const LoggerParserManager&) = delete ;
            LoggerParserManager& operator=(const LoggerParserManager&) = delete ;
        } ;
    }
}

#endif
