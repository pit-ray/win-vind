#ifndef _OPTION_HPP
#define _OPTION_HPP

#include <memory>
#include <string>

namespace vind
{
    namespace opt
    {
        class Option {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;
            virtual void do_enable() const  = 0 ;
            virtual void do_disable() const = 0 ;
            virtual void do_process() const = 0 ;

        public:
            using SPtr = std::shared_ptr<Option> ;

            explicit Option() ;
            explicit Option(const std::string& name) ;
            explicit Option(std::string&& name="undefined option") ;
            virtual ~Option() noexcept ;

            Option(Option&&) noexcept ;
            Option& operator=(Option&&) noexcept ;

            Option(const Option&) = delete ;
            Option& operator=(const Option&) = delete ;

            const std::string& name() const noexcept ;

            void enable() ;
            void disable() ;

            bool is_enabled() const noexcept ;
            void process() const ;
        } ;
    }
}

#endif
