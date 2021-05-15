#ifndef _RC_PARSER_HPP
#define _RC_PARSER_HPP

#include <string>
#include <utility>

namespace vind
{
    namespace rcparser {
        //
        // It removes comments begining with double quote.
        // Example:
        //  input:  abc "str""comment
        //  output: abc "str"
        //
        void remove_dbquote_comment(std::string& line) ;

        //
        // It parses command and its argments from a line.
        // Example:
        //  input: set option_name = hoge
        //  output:
        //      first:  set
        //      second: option_name = hoge
        //
        std::pair<std::string, std::string> divide_cmd_and_args(
                const std::string& line) ;

        // 
        // It parses arguments as single (Remove the spaces on both ends).
        // Example:
        //  input:  abcd  
        //  output: abcd
        //
        std::string extract_single_arg(
                const std::string& args) ;

        // 
        // It parses arguments as double.
        // Example:
        //  input:  foo bar
        //  output:
        //      first : foo
        //      second: bar
        //
        std::pair<std::string, std::string> extract_double_args(
                const std::string& args) ;

        //
        // It parses argument as key-value syntax.
        // Example:
        //  input : foo = bar
        //  output:
        //      first : foo
        //      second: bar
        //
        std::pair<std::string, std::string> divide_key_and_value(
                const std::string& args,
                std::string delimiter="=") ;
    }
}

#endif
