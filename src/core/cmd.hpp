#ifndef _CMD_HPP
#define _CMD_HPP

#include <memory>
#include <string>


namespace vind
{
    namespace core
    {
        // Integration for internal and external commands.
        class Cmd {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit Cmd() ;
            explicit Cmd(std::string&& strcmd) ;
            explicit Cmd(const std::string& strcmd) ;

            virtual ~Cmd() noexcept ;

            Cmd(const Cmd&) = delete ;
            Cmd& operator=(const Cmd&) = delete ;

            Cmd(Cmd&&) ;
            Cmd& operator=(Cmd&&) ;

            std::size_t size() const noexcept ;
            std::size_t length() const noexcept ;

            void execute() ;
        } ;
    }
}

#endif
