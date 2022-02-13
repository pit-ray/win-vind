#include "lgrparsermgr.hpp"

#include "bind/bindedfunc.hpp"
#include "keylgrbase.hpp"
#include "lgrparser.hpp"


namespace vind
{
    namespace core
    {
        struct LoggerParserManager::Impl {
            std::vector<LoggerParser::SPtr> parsers_ ;

            Impl()
            : parsers_()
            {}

            template <typename T>
            Impl(T&& parsers)
            : parsers_(std::forward<T>(parsers))
            {}
        } ;

        LoggerParserManager::LoggerParserManager()
        : pimpl(std::make_unique<Impl>())
        {}

        LoggerParserManager::LoggerParserManager(
                const std::vector<LoggerParser::SPtr>& parsers)
        : pimpl(std::make_unique<Impl>(parsers))
        {}

        LoggerParserManager::LoggerParserManager(
                std::vector<LoggerParser::SPtr>&& parsers)
        : pimpl(std::make_unique<Impl>(std::move(parsers)))
        {}

        LoggerParserManager::~LoggerParserManager() noexcept = default ;
        LoggerParserManager::LoggerParserManager(LoggerParserManager&&) = default ;
        LoggerParserManager& LoggerParserManager::operator=(LoggerParserManager&&) = default ;

        LoggerParser::SPtr
        LoggerParserManager::transition_parser_states_in_batch(
                const KeyLoggerBase& lgr) {
            LoggerParser::SPtr ptr = nullptr ;
            for(const auto& log : lgr) {
                ptr = find_parser_with_transition(log, 0) ;
            }
            return ptr ;
        }

        void LoggerParserManager::search_unrejected_parser(
                std::vector<LoggerParser::SPtr>& results) {

            if(!results.empty()) {
                results.clear() ;
            }

            for(auto& parser : pimpl->parsers_) {
                if(!parser->is_rejected()) {
                    results.push_back(parser) ;
                }
            }
        }

        LoggerParser::SPtr LoggerParserManager::find_rejected_with_ready_parser() {
            for(auto& parser : pimpl->parsers_) {
                if(parser->is_rejected_with_ready()) {
                    return parser ;
                }
            }
            return nullptr ;
        }

        LoggerParser::SPtr LoggerParserManager::find_waiting_parser() {
            for(auto& parser : pimpl->parsers_) {
                if(parser->is_waiting()) {
                    return parser ;
                }
            }
            return nullptr ;
        }

        LoggerParser::SPtr LoggerParserManager::find_accepted_parser() {
            for(auto& parser : pimpl->parsers_) {
                if(parser->is_accepted()) {
                    return parser ;
                }
            }
            return nullptr ;
        }

        LoggerParser::SPtr LoggerParserManager::find_parser_with_transition(
                const KeyLog& log,
                std::size_t low_priority_func_id) {
            LoggerParser::SPtr ptr = nullptr ;
            LoggerParser::SPtr low_priority_parser = nullptr ;
            unsigned char mostnum = 0 ;

            for(auto& parser : pimpl->parsers_) {
                auto num = parser->validate_if_match(log) ;

                if(num <= 0) {
                    continue ;
                }

                if(parser->is_rejected()) {
                    continue ;
                }

                if(mostnum < num) {
                    if(parser->get_func()->id() == low_priority_func_id) {
                        low_priority_parser = parser ;
                    }
                    else {
                        ptr = parser ;
                        mostnum = num ;
                    }
                }
                else if(mostnum == num && !ptr->is_accepted()) {
                    if(parser->is_accepted()) {
                        ptr = parser ;
                    }
                    else if(ptr->is_rejected_with_ready() && parser->is_waiting()) {
                        ptr = parser ;
                    }
                }
            }

            if(ptr) {
                return ptr ;
            }
            if(low_priority_parser && low_priority_parser->is_accepted()) {
                return low_priority_parser ;
            }
            return nullptr ;
        }

        void LoggerParserManager::backward_parser_states(std::size_t n) {
            for(auto& parser : pimpl->parsers_) {
                parser->backward_state(n) ;
            }
        }

        void LoggerParserManager::reset_parser_states() {
            for(auto& parser : pimpl->parsers_) {
                parser->reset_state() ;
            }
        }
    }
}
