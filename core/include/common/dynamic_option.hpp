#ifndef _DYNAMIC_OPTION_HPP
#define _DYNAMIC_OPTION_HPP

#include <string>
#include <memory>

class DynamicOption ;

namespace op
{
    using shp_t = std::shared_ptr<DynamicOption> ;
}

class DynamicOption
{
private:
    struct Impl ;
    std::unique_ptr<Impl> pimpl ;
    virtual void do_enable() const  = 0 ;
    virtual void do_disable() const = 0 ;
    virtual void do_process() const = 0 ;

public:
    explicit DynamicOption() ;
    virtual ~DynamicOption() noexcept ;

    DynamicOption(DynamicOption&&) noexcept ;
    DynamicOption& operator=(DynamicOption&&) noexcept ;

    DynamicOption(const DynamicOption&) = delete ;
    DynamicOption& operator=(const DynamicOption&) = delete ;

    virtual const std::string name() const noexcept =  0 ;

    void enable() ;
    void disable() ;

    bool is_enabled() const noexcept ;
    void process() const ;
} ;

#endif
