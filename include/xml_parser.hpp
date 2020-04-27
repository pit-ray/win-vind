#ifndef _XML_PARSER_HPP
#define _XML_PARSER_HPP

#include <unordered_map>
#include <string>
#include <vector>

namespace XMLParser
{
    using vc_t = std::vector<unsigned char> ;
    using vvc_t = std::vector<vc_t> ;
    using vvvc_t = std::vector<vvc_t> ;
    using bind_map_t = std::unordered_map<std::string, vvvc_t> ;
    const bind_map_t load_bind_map(const std::string& filename) noexcept ;

    //this function must be called after load_bind_map
    const std::vector<unsigned char> get_ignore_alone() noexcept ;

    const bind_map_t load_command_map(const std::string& filename) noexcept ;
} ;

#endif