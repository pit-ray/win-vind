#include "cmdunit.hpp"

#include "cmdparser.hpp"
#include "keycode.hpp"

#include "bind/binded_func.hpp"


namespace vind
{
    namespace core
    {
        struct CmdUnit::Impl {
            bool external_ ;

            std::vector<KeyCode> keys_ ;

            bind::BindedFunc::SPtr func_ ;

            template <typename String>
            Impl(bool external)
            : external_(external),
              keys_()
            {}
        } ;

        CmdUnit::CmdUnit()
        : CmdUnit("")
        {}

        CmdUnit::CmdUnit(std::string&& strkeyset, bool external)
        : pimpl(std::make_unique<Impl>(external))
        {
            pimpl->key = parse_combined_command(std::move(strkeyset))
        }

        CmdUnit::CmdUnit(const std::string& strcmd, bool external)
        : CmdUnit(std::make_unique<Impl>(external))
        {
            pimpl->key = parse_combined_command(strkeyset)
        }

        CmdUnit::~CmdUnit() noexcept = default ;
        CmdUnit::CmdUnit(CmdUnit&&) = default ;
        CmdUnit& CmdUnit::operator=(CmdUnit&&) = default ;


        CmdUnit::Data::const_iterator CmdUnit::begin() const noexcept {
            return pimpl->keys_.cbegin() ;
        }
        CmdUnit::Data::const_iterator end() const noexcept {
            return pimpl->keys_.cend() ;
        }

        CmdUnit::Data::const_iterator cbegin() const noexcept {
            return pimpl->keys_.cbegin() ;
        }
        CmdUnit::Data::const_iterator cend() const noexcept {
            return pimpl->keys_.cend() ;
        }

        std::size_t CmdUnit::size() const noexcept {
            return pimpl->keys.size() ;
        }

        std::size_t CmdUnit::length() const noexcept {
            return pimpl->keys.size() ;
        }

        bool CmdUnit::for_external() const noexcept {
            return pimpl->external_ ;
        }

        bool CmdUnit::for_internal() const noexcept {
            return !pimpl->external_ ;
        }

        void CmdUnit::execute() {

        }
    }
}
