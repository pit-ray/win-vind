#ifndef _RC_PARSER_HPP
#define _RC_PARSER_HPP

#include "mode.hpp"

#include <string>
#include <utility>

namespace vind
{
    namespace core {
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


        enum class RunCommandsIndex : unsigned char {
            UNDEFINED       = 0,

            SET             = 0b0000'00001,
            COMMAND,
            DELCOMMAND,
            COMCLEAR,

            SOURCE,

            AUTOCMD,
            AUTOCMD_REMOVE,

            MASK_MODE       = 0b0000'1111,

            MASK_MAP        = 0b0001'0000,
            MAP_IN          = MASK_MAP | static_cast<unsigned char>(Mode::INSERT),
            MAP_GN          = MASK_MAP | static_cast<unsigned char>(Mode::GUI_NORMAL),
            MAP_GV          = MASK_MAP | static_cast<unsigned char>(Mode::GUI_VISUAL),
            MAP_EN          = MASK_MAP | static_cast<unsigned char>(Mode::EDI_NORMAL),
            MAP_EV          = MASK_MAP | static_cast<unsigned char>(Mode::EDI_VISUAL),
            MAP_RS          = MASK_MAP | static_cast<unsigned char>(Mode::RESIDENT),
            MAP_CM          = MASK_MAP | static_cast<unsigned char>(Mode::COMMAND),

            MASK_NOREMAP    = 0b0010'0000,
            NOREMAP_IN      = MASK_NOREMAP | static_cast<unsigned char>(Mode::INSERT),
            NOREMAP_GN      = MASK_NOREMAP | static_cast<unsigned char>(Mode::GUI_NORMAL),
            NOREMAP_GV      = MASK_NOREMAP | static_cast<unsigned char>(Mode::GUI_VISUAL),
            NOREMAP_EN      = MASK_NOREMAP | static_cast<unsigned char>(Mode::EDI_NORMAL),
            NOREMAP_EV      = MASK_NOREMAP | static_cast<unsigned char>(Mode::EDI_VISUAL),
            NOREMAP_RS      = MASK_NOREMAP | static_cast<unsigned char>(Mode::RESIDENT),
            NOREMAP_CM      = MASK_NOREMAP | static_cast<unsigned char>(Mode::COMMAND),

            MASK_UNMAP     = 0b0100'0000,
            UNMAP_IN       = MASK_UNMAP | static_cast<unsigned char>(Mode::INSERT),
            UNMAP_GN       = MASK_UNMAP | static_cast<unsigned char>(Mode::GUI_NORMAL),
            UNMAP_GV       = MASK_UNMAP | static_cast<unsigned char>(Mode::GUI_VISUAL),
            UNMAP_EN       = MASK_UNMAP | static_cast<unsigned char>(Mode::EDI_NORMAL),
            UNMAP_EV       = MASK_UNMAP | static_cast<unsigned char>(Mode::EDI_VISUAL),
            UNMAP_RS       = MASK_UNMAP | static_cast<unsigned char>(Mode::RESIDENT),
            UNMAP_CM       = MASK_UNMAP | static_cast<unsigned char>(Mode::COMMAND),

            MASK_MAPCLEAR  = 0b1000'0000,
            MAPCLEAR_IN    = MASK_MAPCLEAR | static_cast<unsigned char>(Mode::INSERT),
            MAPCLEAR_GN    = MASK_MAPCLEAR | static_cast<unsigned char>(Mode::GUI_NORMAL),
            MAPCLEAR_GV    = MASK_MAPCLEAR | static_cast<unsigned char>(Mode::GUI_VISUAL),
            MAPCLEAR_EN    = MASK_MAPCLEAR | static_cast<unsigned char>(Mode::EDI_NORMAL),
            MAPCLEAR_EV    = MASK_MAPCLEAR | static_cast<unsigned char>(Mode::EDI_VISUAL),
            MAPCLEAR_RS    = MASK_MAPCLEAR | static_cast<unsigned char>(Mode::RESIDENT),
            MAPCLEAR_CM    = MASK_MAPCLEAR | static_cast<unsigned char>(Mode::COMMAND),
        } ;

        RunCommandsIndex parse_run_command(const std::string& strcmd) ;
    }
}

#endif
