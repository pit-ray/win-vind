#ifndef _OPTION_HPP
#define _OPTION_HPP

#include <string>
#include <memory>

namespace vind
{
    class Option ;

    namespace op {
        using SPtr = std::shared_ptr<Option> ;
    }

    class Option {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;
        virtual void do_enable() const  = 0 ;
        virtual void do_disable() const = 0 ;
        virtual void do_process() const = 0 ;

    public:
        explicit Option() ;
        virtual ~Option() noexcept ;

        Option(Option&&) noexcept ;
        Option& operator=(Option&&) noexcept ;

        Option(const Option&) = delete ;
        Option& operator=(const Option&) = delete ;

        virtual const std::string name() const noexcept =  0 ;

        void enable() ;
        void disable() ;

        bool is_enabled() const noexcept ;
        void process() const ;
    } ;
}

#endif
