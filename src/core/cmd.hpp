#ifndef _CMD_HPP
#define _CMD_HPP

#include <memory>

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
            explicit Cmd(const std::string& strcmd) ;

            virtual ~Cmd() noexcept ;
        } ;
    }
}

#endif
