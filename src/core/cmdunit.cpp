#include "cmdunit.hpp"

#include "cmdparser.hpp"
#include "keycode.hpp"

#include "bind/bindedfunc.hpp"
#include "bind/bindinglist.hpp"
#include "util/def.hpp"


namespace vind
{
    namespace core
    {
        CmdUnit::CmdUnit()
        : keycodes_()
        {}

        CmdUnit::CmdUnit(const Data& codes)
        : keycodes_(codes)
        {}

        CmdUnit::CmdUnit(Data&& codes)
        : keycodes_(std::move(codes))
        {}

        CmdUnit::CmdUnit(std::initializer_list<KeyCode>&& codes)
        : keycodes_(codes)
        {}

        CmdUnit::~CmdUnit() noexcept = default ;

        CmdUnit::CmdUnit(const CmdUnit& rhs)
        : keycodes_(rhs.keycodes_)
        {}

        CmdUnit::CmdUnit(CmdUnit&&) = default ;
        CmdUnit& CmdUnit::operator=(CmdUnit&&) = default ;

        CmdUnit& CmdUnit::operator=(const CmdUnit::Data& rhs) {
            keycodes_ = rhs ;
            return *this ;
        }

        const CmdUnit::Data& CmdUnit::get() const & noexcept {
            return keycodes_ ;
        }

        const CmdUnit::Data& CmdUnit::data() const & noexcept {
            return keycodes_ ;
        }

        CmdUnit::Data::const_iterator CmdUnit::begin() const noexcept {
            return keycodes_.begin() ;
        }

        CmdUnit::Data::const_iterator CmdUnit::end() const noexcept {
            return keycodes_.end() ;
        }

        CmdUnit::Data::const_iterator CmdUnit::cbegin() const noexcept {
            return keycodes_.cbegin() ;
        }

        CmdUnit::Data::const_iterator CmdUnit::cend() const noexcept {
            return keycodes_.cend() ;
        }

        std::size_t CmdUnit::size() const noexcept {
            return keycodes_.size() ;
        }

        bool CmdUnit::empty() const noexcept {
            return keycodes_.empty() ;
        }

        bool CmdUnit::is_containing(KeyCode key) const
        {
            return keycodes_.find(key) != keycodes_.end() ;
        }

        bool CmdUnit::operator==(const CmdUnit& rhs) const {
            return keycodes_ == rhs.keycodes_ ;
        }
        bool CmdUnit::operator==(CmdUnit&& rhs) const {
            return keycodes_ == rhs.keycodes_ ;
        }
        bool CmdUnit::operator==(const CmdUnit::Data& rhs) const {
            return keycodes_ == rhs ;
        }
        bool CmdUnit::operator==(CmdUnit::Data&& rhs) const {
            return keycodes_ == rhs ;
        }

        bool CmdUnit::operator!=(const CmdUnit& rhs) const {
            return keycodes_ != rhs.keycodes_ ;
        }
        bool CmdUnit::operator!=(CmdUnit&& rhs) const {
            return keycodes_ != rhs.keycodes_ ;
        }
        bool CmdUnit::operator!=(const CmdUnit::Data& rhs) const {
            return keycodes_ != rhs ;
        }
        bool CmdUnit::operator!=(CmdUnit::Data&& rhs) const {
            return keycodes_ != rhs ;
        }

        const CmdUnit CmdUnit::operator-(const CmdUnit& rhs) const {
            return CmdUnit(erased_diff(rhs.keycodes_)) ;
        }
        const CmdUnit CmdUnit::operator-(CmdUnit&& rhs) const {
            return CmdUnit(erased_diff(std::move(rhs.keycodes_))) ;
        }
        const CmdUnit CmdUnit::operator-(const CmdUnit::Data& rhs) const {
            return CmdUnit(erased_diff(rhs)) ;
        }
        const CmdUnit CmdUnit::operator-(CmdUnit::Data&& rhs) const {
            return CmdUnit(erased_diff(std::move(rhs))) ;
        }

        CmdUnit& CmdUnit::operator-=(const CmdUnit& rhs) {
            for(const auto& k : rhs) {
                keycodes_.erase(k) ;
            }
            return *this ;
        }
        CmdUnit& CmdUnit::operator-=(CmdUnit&& rhs) {
            for(const auto& k : rhs) {
                keycodes_.erase(k) ;
            }
            return *this ;
        }
        CmdUnit& CmdUnit::operator-=(const CmdUnit::Data& rhs) {
            for(const auto& k : rhs) {
                keycodes_.erase(k) ;
            }
            return *this ;
        }
        CmdUnit& CmdUnit::operator-=(CmdUnit::Data&& rhs) {
            for(const auto& k : rhs) {
                keycodes_.erase(k) ;
            }
            return *this ;
        }

        void CmdUnit::execute() {
        }

        void InternalCmdUnit::execute() {
        }

        void ExternalCmdUnit::execute() {
            // SendInput
        }

        bool FunctionalCmdUnit::has_function() const noexcept {
            return func_ != nullptr ;
        }

        const std::shared_ptr<bind::BindedFunc>& FunctionalCmdUnit::get_function() const {
            return func_ ;
        }

        void FunctionalCmdUnit::execute() {
            if(!has_function()) {
                throw RUNTIME_EXCEPT("Does not have an associated function.") ;
            }
            func_->process() ;
        }


        std::unique_ptr<CmdUnit> create_cmdunit(const std::string& strcommand) {
            auto ptr = bind::ref_global_funcs_bynames(strcommand) ;
            if(ptr) {
                return std::make_unique<FunctionalCmdUnit>(std::move(ptr)) ;
            }

            auto cmd = parse_combined_command(strcommand) ;
            return std::make_unique<InternalCmdUnit>(InternalCmdUnit::Data(cmd.begin(), cmd.end())) ;
        }
    }
}
