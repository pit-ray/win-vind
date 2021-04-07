#ifndef _LOGGER_PARSER_HPP
#define _LOGGER_PARSER_HPP

#include <memory>
#include <vector>
#include <string>

namespace vind
{
    class BindedFunc ;
    class KeyLog ;

    class LoggerParser {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

    public:
        using SPtr = std::shared_ptr<LoggerParser> ;

        explicit LoggerParser() ;
        explicit LoggerParser(const std::shared_ptr<BindedFunc>& func) ;
        explicit LoggerParser(std::shared_ptr<BindedFunc>&& func) ;

        virtual ~LoggerParser() noexcept ;

        LoggerParser(const LoggerParser&) ;
        LoggerParser& operator=(const LoggerParser&) ;
        LoggerParser(LoggerParser&&) ;
        LoggerParser& operator=(LoggerParser&&) ;

        void append_binding(std::string command) ;

        void append_binding_list(const std::vector<std::string>& list) ;
        void append_binding_list(std::vector<std::string>&& list) ;

        void reset_binding(const std::string& command) ;
        void reset_binding(std::string&& command) ;

        void reset_binding_list(const std::vector<std::string>& list) ;
        void reset_binding_list(std::vector<std::string>&& list) ;

        void unbind_function() noexcept ;

        void bind_function(const std::shared_ptr<BindedFunc>& func) ;
        void bind_function(std::shared_ptr<BindedFunc>&& func) ;

        bool has_function() const noexcept ;

        const std::shared_ptr<BindedFunc>& get_func() const noexcept ;

        unsigned char validate_if_match(const KeyLog& log) ;

        std::size_t state_stack_size() const noexcept ;

        void reset_state() noexcept ;
        void undo_state(std::size_t n) ;

        bool is_accepted() const noexcept ;
        bool is_rejected() const noexcept ;
        bool is_waiting() const noexcept ;
        bool is_rejected_with_headsys_ready() const noexcept ;
    } ;
}

#endif
