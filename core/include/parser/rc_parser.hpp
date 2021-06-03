#ifndef _RC_PARSER_HPP
#define _RC_PARSER_HPP

#include "mode.hpp"

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

        // 
        // It splits the string at the last occurrence of the keyword.
        // Example:
        //  input :
        //      str: nnoremap
        //      key: n
        //
        //  output:
        //      first : n
        //      second: noremap
        //
        std::pair<std::string, std::string> divide_prefix_and_cmd(
                const std::string& str,
                const std::string& split_keyword="n") ;


        mode::Mode parse_mode_prefix(const std::string& prefix) ;

        enum RunCommandsIndex : unsigned char {
            UNDEFINED       = 0,

            SET             = 0b0000'00001,

            MASK_MAP        = 0b0001'0000,
            MAP_IN          = MASK_MAP | (mode::Mode::INSERT     & mode::Mode::MASK_INDEX),
            MAP_GN          = MASK_MAP | (mode::Mode::GUI_NORMAL & mode::Mode::MASK_INDEX),
            MAP_GV          = MASK_MAP | (mode::Mode::GUI_VISUAL & mode::Mode::MASK_INDEX), 
            MAP_EN          = MASK_MAP | (mode::Mode::EDI_NORMAL & mode::Mode::MASK_INDEX),
            MAP_EV          = MASK_MAP | (mode::Mode::EDI_VISUAL & mode::Mode::MASK_INDEX),

            MASK_NOREMAP    = 0b0010'0000,
            NOREMAP_IN      = MASK_NOREMAP | (mode::Mode::INSERT     & mode::Mode::MASK_INDEX),
            NOREMAP_GN      = MASK_NOREMAP | (mode::Mode::GUI_NORMAL & mode::Mode::MASK_INDEX),
            NOREMAP_GV      = MASK_NOREMAP | (mode::Mode::GUI_VISUAL & mode::Mode::MASK_INDEX), 
            NOREMAP_EN      = MASK_NOREMAP | (mode::Mode::EDI_NORMAL & mode::Mode::MASK_INDEX),
            NOREMAP_EV      = MASK_NOREMAP | (mode::Mode::EDI_VISUAL & mode::Mode::MASK_INDEX),

            MASK_UNMAP     = 0b0100'0000,
            UNMAP_IN       = MASK_UNMAP | (mode::Mode::INSERT     & mode::Mode::MASK_INDEX),
            UNMAP_GN       = MASK_UNMAP | (mode::Mode::GUI_NORMAL & mode::Mode::MASK_INDEX),
            UNMAP_GV       = MASK_UNMAP | (mode::Mode::GUI_VISUAL & mode::Mode::MASK_INDEX), 
            UNMAP_EN       = MASK_UNMAP | (mode::Mode::EDI_NORMAL & mode::Mode::MASK_INDEX),
            UNMAP_EV       = MASK_UNMAP | (mode::Mode::EDI_VISUAL & mode::Mode::MASK_INDEX),

            MASK_MAPCLEAR  = 0b1000'0000,
            MAPCLEAR_IN    = MASK_MAPCLEAR | (mode::Mode::INSERT     & mode::Mode::MASK_INDEX),
            MAPCLEAR_GN    = MASK_MAPCLEAR | (mode::Mode::GUI_NORMAL & mode::Mode::MASK_INDEX),
            MAPCLEAR_GV    = MASK_MAPCLEAR | (mode::Mode::GUI_VISUAL & mode::Mode::MASK_INDEX), 
            MAPCLEAR_EN    = MASK_MAPCLEAR | (mode::Mode::EDI_NORMAL & mode::Mode::MASK_INDEX),
            MAPCLEAR_EV    = MASK_MAPCLEAR | (mode::Mode::EDI_VISUAL & mode::Mode::MASK_INDEX),

            COMMAND        = MASK_MAP      | (mode::Mode::COMMAND & mode::Mode::MASK_INDEX),
            DELCOMMAND     = MASK_UNMAP    | (mode::Mode::COMMAND & mode::Mode::MASK_INDEX),
            COMCLEAR       = MASK_MAPCLEAR | (mode::Mode::COMMAND & mode::Mode::MASK_INDEX),
        } ;

        RunCommandsIndex parse_run_command(const std::string& strcmd) ;
    }
}

#endif
