#ifndef _LOGGER_PARSER_HPP
#define _LOGGER_PARSER_HPP

#include <memory>
#include <string>
#include <vector>

#include "bind/bindings_def.hpp"

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

        // Parse a command (e.g. <c-w>s) and create new CommandList.
        void append_binding(const std::string& command) ;

        // Parse commands and create new CommandList.
        void append_binding_list(const std::vector<std::string>& list) ;
        void append_binding_list(std::vector<std::string>&& list) ;

        void share_parsed_binding_list(
                const std::shared_ptr<CommandList>& cmdlist) ;

        // Delete current CommandList and set another command.
        void reset_binding(const std::string& command) ;
        void reset_binding(std::string&& command) ;

        void reset_binding_list(const std::vector<std::string>& list) ;
        void reset_binding_list(std::vector<std::string>&& list) ;

        // Delete current CommandList
        void reset_binding_list() ;

        void unbind_function() noexcept ;

        void bind_function(const std::shared_ptr<BindedFunc>& func) ;
        void bind_function(std::shared_ptr<BindedFunc>&& func) ;

        bool has_function() const noexcept ;
        bool has_bindings() const noexcept ;

        const std::shared_ptr<BindedFunc>& get_func() const noexcept ;

        // Transitions the automation state and returns the number of matched keycodes.
        // For example, if inputs are {KEYCODE_F, KEYCODE_LCTRL} under bindings {KEYCODE_F},
        // it returns one.
        //
        // You can refer to the state transition diagram at "devdocs/generator/logger_parser"
        //
        unsigned char validate_if_match(const KeyLog& log) ;

        std::size_t state_stack_size() const noexcept ;

        void reset_state() noexcept ;

        // Return state to previous one.
        void backward_state(std::size_t n) ;

        // Check the curent state of automaton
        bool is_accepted() const noexcept ;
        bool is_rejected() const noexcept ;
        bool is_waiting() const noexcept ;
        bool is_rejected_with_ready() const noexcept ;
    } ;
}

#endif
