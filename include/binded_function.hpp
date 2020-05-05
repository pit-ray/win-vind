#ifndef _BINDED_FUNCTION_HPP
#define _BINDED_FUNCTION_HPP

#include <memory>
#include <string>
#include "xml_parser.hpp"

class BindedFunction ;

namespace bf
{
    using shp_t = std::shared_ptr<BindedFunction> ;
}

class KeyLog ;
class KeyLogger ;

//base
class BindedFunction
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;

    virtual bool do_process(const bool first_call) const = 0 ;

public:
    explicit BindedFunction() ;
    virtual ~BindedFunction() ;

    BindedFunction(BindedFunction&&) noexcept ;
    BindedFunction& operator=(BindedFunction&&) noexcept ;

    BindedFunction(const BindedFunction&) = delete ;
    BindedFunction& operator=(const BindedFunction&) = delete ;

    virtual const std::string name() const noexcept = 0 ;

    void process(const bool first_call=true) const ;

    void set_command(const XMLParser::vvvc_t& command) noexcept ;

    std::size_t existed_num_and_update(const KeyLog& log, const std::size_t seq_index=0) const noexcept ;

    bool is_callable() const noexcept ;
} ;

#endif