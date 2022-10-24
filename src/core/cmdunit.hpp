#ifndef _CMDUNIT_HPP
#define _CMDUNIT_HPP

#include "keycode.hpp"

#include "bind/bindedfunc.hpp"

#include <initializer_list>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_set>


namespace vind
{
    namespace core
    {
        class MapSolver ;

        using CmdUnitSet = std::unordered_set<KeyCode> ;

        class CmdUnit {
        public:
            using SPtr = std::shared_ptr<CmdUnit> ;

        private:
            CmdUnitSet keycodes_ ;
            std::size_t id_ ;

            template <typename T>
            CmdUnitSet erased_diff(T&& rhs) const {
                auto diff = keycodes_ ;
                for(auto& k : rhs) {
                    diff.erase(k) ;
                }
                return diff ;
            }

            std::size_t compute_id(const CmdUnitSet& keys) const ;

        public:
            explicit CmdUnit() ;
            explicit CmdUnit(const CmdUnitSet& codes) ;
            explicit CmdUnit(CmdUnitSet&& codes) ;
            explicit CmdUnit(const KeyCode& code) ;
            explicit CmdUnit(KeyCode&& code) ;
            explicit CmdUnit(std::initializer_list<KeyCode>&& codes) ;

            template <typename InputIterator>
            explicit CmdUnit(InputIterator begin, InputIterator end)
            : CmdUnit(CmdUnitSet(begin, end))
            {}

            virtual ~CmdUnit() noexcept ;

            CmdUnit(CmdUnit&&) ;
            CmdUnit& operator=(CmdUnit&&) ;
            CmdUnit& operator=(CmdUnitSet&&) ;

            CmdUnit(const CmdUnit&) ;
            CmdUnit& operator=(const CmdUnit&) ;
            CmdUnit& operator=(const CmdUnitSet&) ;

            const CmdUnitSet& get() const & noexcept ;

            const CmdUnitSet& data() const & noexcept ;

            CmdUnitSet::const_iterator begin() const noexcept ;
            CmdUnitSet::const_iterator end() const noexcept ;

            CmdUnitSet::const_iterator cbegin() const noexcept ;
            CmdUnitSet::const_iterator cend() const noexcept ;

            virtual std::size_t id() const noexcept ;
            std::size_t size() const noexcept ;
            bool empty() const noexcept ;
            bool is_containing(KeyCode key) const ;

            bool operator==(const CmdUnit& rhs) const ;
            bool operator==(CmdUnit&& rhs) const ;
            bool operator==(const CmdUnitSet& rhsraw) const ;
            bool operator==(CmdUnitSet&& rhsraw) const ;

            bool operator!=(const CmdUnit& rhs) const ;
            bool operator!=(CmdUnit&& rhs) const ;
            bool operator!=(const CmdUnitSet& rhs) const ;
            bool operator!=(CmdUnitSet&& rhsw) const ;

            const CmdUnit operator-(const CmdUnit& rhs) const ;
            const CmdUnit operator-(CmdUnit&& rhs) const ;
            const CmdUnit operator-(const CmdUnitSet& rhs) const ;
            const CmdUnit operator-(CmdUnitSet&& rhs) const ;

            CmdUnit& operator-=(const CmdUnit& rhs) ;
            CmdUnit& operator-=(CmdUnit&& rhs) ;
            CmdUnit& operator-=(const CmdUnitSet& rhs) ;
            CmdUnit& operator-=(CmdUnitSet&& rhs) ;

            virtual void execute(
                std::uint16_t count=1,
                const std::string& args="") ;
        } ;

        class InternalCmdUnit : public CmdUnit {
        public:
            template <typename T>
            explicit InternalCmdUnit(T&& keycodes)
            : CmdUnit(std::forward<T>(keycodes))
            {}

            void execute(
                std::uint16_t count=1,
                const std::string& args="") override ;
        } ;

        class ExternalCmdUnit : public CmdUnit {
        public:
            template <typename T>
            explicit ExternalCmdUnit(T&& keycodes)
            : CmdUnit(std::forward<T>(keycodes))
            {}

            void execute(
                std::uint16_t count=1,
                const std::string& args="") override ;
        } ;

        class FunctionalCmdUnit : public CmdUnit {
        private:
            bind::BindedFunc::SPtr func_ ;

        public:
            template <typename T>
            explicit FunctionalCmdUnit(T&& function_ptr)
            : CmdUnit(),
              func_(std::forward<T>(function_ptr))
            {}

            bool has_function() const noexcept ;

            const bind::BindedFunc::SPtr& get_function() const ;

            std::size_t id() const noexcept override ;

            void execute(
                std::uint16_t count=1,
                const std::string& args="") override ;
        } ;

        std::ostream& operator<<(
            std::ostream& stream,
            const CmdUnit::SPtr& rhs) ;

        std::ostream& operator<<(
            std::ostream& stream,
            const CmdUnitSet& rhs) ;

        inline std::ostream& operator<<(
            std::ostream& stream,
            const CmdUnit& rhs) {
            return stream << rhs.get() ;
        }

        std::ostream& operator<<(
            std::ostream& stream,
            const std::vector<CmdUnit>& rhs) ;

        std::ostream& operator<<(
            std::ostream& stream,
            const std::vector<CmdUnitSet>& rhs) ;

        std::ostream& operator<<(
            std::ostream& stream,
            const std::vector<std::shared_ptr<CmdUnit>>& rhs) ;

        std::ostream& operator<<(
            std::ostream& stream,
            const std::vector<std::unique_ptr<CmdUnit>>& rhs) ;
    }
}

#endif
