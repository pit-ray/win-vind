#include "cmdunit.hpp"

#include "cmdparser.hpp"
#include "inputgate.hpp"
#include "keycode.hpp"

#include "bind/bindedfunc.hpp"
#include "bind/bindinglist.hpp"
#include "util/def.hpp"

#include <algorithm>
#include <memory>


namespace vind
{
    namespace core
    {
        CmdUnit::CmdUnit()
        : keycodes_()
        {}

        CmdUnit::CmdUnit(const CmdUnitSet& codes)
        : keycodes_(codes)
        {}

        CmdUnit::CmdUnit(CmdUnitSet&& codes)
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

        CmdUnit& CmdUnit::operator=(const CmdUnit&) = default ;
        CmdUnit& CmdUnit::operator=(const CmdUnitSet& rhs) {
            keycodes_ = rhs ;
            return *this ;
        }

        const CmdUnitSet& CmdUnit::get() const & noexcept {
            return keycodes_ ;
        }

        const CmdUnitSet& CmdUnit::data() const & noexcept {
            return keycodes_ ;
        }

        CmdUnitSet::const_iterator CmdUnit::begin() const noexcept {
            return keycodes_.begin() ;
        }

        CmdUnitSet::const_iterator CmdUnit::end() const noexcept {
            return keycodes_.end() ;
        }

        CmdUnitSet::const_iterator CmdUnit::cbegin() const noexcept {
            return keycodes_.cbegin() ;
        }

        CmdUnitSet::const_iterator CmdUnit::cend() const noexcept {
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
        bool CmdUnit::operator==(const CmdUnitSet& rhs) const {
            return keycodes_ == rhs ;
        }
        bool CmdUnit::operator==(CmdUnitSet&& rhs) const {
            return keycodes_ == rhs ;
        }

        bool CmdUnit::operator!=(const CmdUnit& rhs) const {
            return keycodes_ != rhs.keycodes_ ;
        }
        bool CmdUnit::operator!=(CmdUnit&& rhs) const {
            return keycodes_ != rhs.keycodes_ ;
        }
        bool CmdUnit::operator!=(const CmdUnitSet& rhs) const {
            return keycodes_ != rhs ;
        }
        bool CmdUnit::operator!=(CmdUnitSet&& rhs) const {
            return keycodes_ != rhs ;
        }

        const CmdUnit CmdUnit::operator-(const CmdUnit& rhs) const {
            return CmdUnit(erased_diff(rhs.keycodes_)) ;
        }
        const CmdUnit CmdUnit::operator-(CmdUnit&& rhs) const {
            return CmdUnit(erased_diff(std::move(rhs.keycodes_))) ;
        }
        const CmdUnit CmdUnit::operator-(const CmdUnitSet& rhs) const {
            return CmdUnit(erased_diff(rhs)) ;
        }
        const CmdUnit CmdUnit::operator-(CmdUnitSet&& rhs) const {
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
        CmdUnit& CmdUnit::operator-=(const CmdUnitSet& rhs) {
            for(const auto& k : rhs) {
                keycodes_.erase(k) ;
            }
            return *this ;
        }
        CmdUnit& CmdUnit::operator-=(CmdUnitSet&& rhs) {
            for(const auto& k : rhs) {
                keycodes_.erase(k) ;
            }
            return *this ;
        }

        void CmdUnit::execute(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            return ;
        }

        void InternalCmdUnit::execute(
                std::uint16_t UNUSED(count),
                const std::string& UNUSED(args)) {
            return ;
        }

        void ExternalCmdUnit::execute(
                std::uint16_t count,
                const std::string& args) {
            std::cout << "executed(EX)\n" ;
            // To reproduce the keystroke, should consider the order for pressing.
            // The value of keycode are designed as sortable object for this purpose.
            std::vector<KeyCode> sequential(data().begin(), data().end()) ;
            std::sort(sequential.begin(), sequential.end()) ;
            for(decltype(count) i = 0 ; i < count ; i ++) {
                InputGate::get_instance().pushup(sequential.begin(), sequential.end()) ;
            }
        }

        bool FunctionalCmdUnit::has_function() const noexcept {
            return func_ != nullptr ;
        }

        const bind::BindedFunc::SPtr& FunctionalCmdUnit::get_function() const {
            return func_ ;
        }

        void FunctionalCmdUnit::execute(
                std::uint16_t count,
                const std::string& args) {
            if(!has_function()) {
                throw RUNTIME_EXCEPT("Does not have an associated function.") ;
            }
            func_->process(count) ;
        }

        std::ostream& operator<<(std::ostream& stream, const CmdUnit::SPtr& rhs) {
            if(auto func_cmdunit = std::dynamic_pointer_cast<FunctionalCmdUnit>(rhs)) {
                stream << "<" << func_cmdunit->get_function()->name() << ">" ;
                return stream ;
            }

            if(rhs->empty()) {
                return stream ;
            }

            if(rhs->size() == 1) {
                const auto& rhs_f = *(rhs->begin()) ;
                if(rhs_f.is_major_system()) {
                    stream << "<" << rhs_f << ">" ;
                }
                else {
                    stream << rhs_f ;
                }

                return stream;
            }

            std::vector<KeyCode> sorted(rhs->begin(), rhs->end()) ;
            std::sort(sorted.begin(), sorted.end()) ;

            stream << "<" ;
            for(auto itr = sorted.begin() ; itr != sorted.end() ; itr ++) {
                if(itr != sorted.begin()) {
                    stream << "-" ;
                }
                stream << *itr ;
            }
            stream << ">" ;
            return stream;
        }

        std::ostream& operator<<(std::ostream& stream, const CmdUnitSet& rhs) {
            stream << std::make_shared<CmdUnit>(rhs) ;
            return stream ;
        }

        std::ostream& operator<<(
            std::ostream& stream,
            const std::vector<CmdUnit>& rhs) {
            for(const auto& keyset : rhs) {
                stream << keyset ;
            }
            return stream ;
        }

        std::ostream& operator<<(
            std::ostream& stream,
            const std::vector<CmdUnitSet>& rhs) {
            for(const auto& keyset : rhs) {
                stream << keyset ;
            }
            return stream ;
        }

        std::ostream& operator<<(
            std::ostream& stream,
            const std::vector<CmdUnit::SPtr>& rhs) {
            for(const auto& keyset : rhs) {
                stream << keyset ;
            }
            return stream ;
        }

        std::ostream& operator<<(
            std::ostream& stream,
            const std::vector<std::unique_ptr<CmdUnit>>& rhs) {
            for(const auto& keyset : rhs) {
                stream << *keyset ;
            }
            return stream ;
        }
    }
}
