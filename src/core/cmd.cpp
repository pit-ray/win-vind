#include "cmd.hpp"

#include "cmdunit.hpp"


namespace vind
{
    namespace core
    {
        struct Cmd::Impl {
            std::vector<CmdUnit> keysets_ ;
        } ;


        Cmd::Cmd()
        : Cmd("")
        {}

        Cmd::Cmd(std::string&& strcmd)
        : pimpl(std::make_unique<Impl>())
        {}

        Cmd::Cmd(const std::string& strcmd)
        : Cmd(std::make_unique<Impl>())
        {}

        Cmd::~Cmd() noexcept = default ;
        Cmd::Cmd(Cmd&&) = default ;
        Cmd& Cmd::operator=(Cmd&&) = default ;

        std::size_t Cmd::size() const noexcept {
            return pimpl->keyset_.size() ;
        }

        std::size_t Cmd::length() const noexcept {
            return pimpl->keyset_.size() ;
        }

        void Cmd::execute() {

        }
    }
}
