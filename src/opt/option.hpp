#ifndef _OPTION_HPP
#define _OPTION_HPP

#include "util/string.hpp"

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

            Option(const Option&)            = delete ;
            Option& operator=(const Option&) = delete ;

            const std::string& name() const noexcept ;

            void enable() ;
            void disable() ;

            bool is_enabled() const noexcept ;
            void process() const ;
        } ;


        //use Curiously Recurring Template Pattern (CRTP)
        //derived class must implement sprocess and sname.
        //if derived class does not use variable of member, sprocess prefers static function.
        //else, sprocess is constant function.
        template <typename Derived>
        class OptionCreator : public Option {
        public:
            template <typename String>
            explicit OptionCreator(String&& name)
            : Option(util::A2a(std::forward<String>(name)))
            {}

            static std::unique_ptr<Option> create() {
                return std::make_unique<Derived>() ;
            }

            static std::shared_ptr<Option> create_with_cache() {
                static std::weak_ptr<Derived> cache ;

                auto pobj = cache.lock() ;

                if(!pobj) {
                    pobj = std::make_shared<Derived>() ;
                    cache = pobj ;
                }

                return pobj ;
            }
        } ;
    }
}

#endif
