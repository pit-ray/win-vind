#ifndef _CMDUNIT_HPP
#define _CMDUNIT_HPP

#include "keycode.hpp"

#include <initializer_list>
#include <memory>
#include <string>
#include <unordered_set>


namespace vind
{
    namespace bind
    {
        class BindedFunc ;
    }

    namespace core
    {
        class CmdUnit {
        public:
            using Data = std::unordered_set<KeyCode> ;

        private:
            Data keycodes_ ;

            template <typename T>
            Data erased_diff(T&& rhs) const {
                auto diff = keycodes_ ;
                for(auto& k : rhs) {
                    diff.erase(k) ;
                }
                return diff ;
            }

        public:
            explicit CmdUnit() ;
            explicit CmdUnit(const Data& codes) ;
            explicit CmdUnit(Data&& codes) ;
            explicit CmdUnit(std::initializer_list<KeyCode>&& codes) ;

            template <typename InputIterator>
            explicit CmdUnit(InputIterator begin, InputIterator end)
            : CmdUnit(Data(begin, end))
            {}

            virtual ~CmdUnit() noexcept ;

            CmdUnit(CmdUnit&&) ;
            CmdUnit& operator=(CmdUnit&&) ;
            CmdUnit& operator=(CmdUnit::Data&&) ;

            CmdUnit(const CmdUnit&) ;
            CmdUnit& operator=(const CmdUnit&) ;
            CmdUnit& operator=(const CmdUnit::Data&) ;

            const Data& get() const & noexcept ;

            const Data& data() const & noexcept ;

            Data::const_iterator begin() const noexcept ;
            Data::const_iterator end() const noexcept ;

            Data::const_iterator cbegin() const noexcept ;
            Data::const_iterator cend() const noexcept ;

            std::size_t size() const noexcept ;
            bool empty() const noexcept ;
            bool is_containing(KeyCode key) const ;

            bool operator==(const CmdUnit& rhs) const ;
            bool operator==(CmdUnit&& rhs) const ;
            bool operator==(const Data& rhsraw) const ;
            bool operator==(Data&& rhsraw) const ;

            bool operator!=(const CmdUnit& rhs) const ;
            bool operator!=(CmdUnit&& rhs) const ;
            bool operator!=(const Data& rhs) const ;
            bool operator!=(Data&& rhsw) const ;

            const CmdUnit operator-(const CmdUnit& rhs) const ;
            const CmdUnit operator-(CmdUnit&& rhs) const ;
            const CmdUnit operator-(const Data& rhs) const ;
            const CmdUnit operator-(Data&& rhs) const ;

            CmdUnit& operator-=(const CmdUnit& rhs) ;
            CmdUnit& operator-=(CmdUnit&& rhs) ;
            CmdUnit& operator-=(const Data& rhs) ;
            CmdUnit& operator-=(Data&& rhs) ;

            virtual void execute() ;
        } ;

        class InternalCmdUnit : public CmdUnit {
        public:
            template <typename T>
            explicit InternalCmdUnit(T&& keycodes)
            : CmdUnit(std::forward<T>(keycodes))
            {}

            void execute() override ;
        } ;

        class ExternalCmdUnit : public CmdUnit {
        public:
            template <typename T>
            explicit ExternalCmdUnit(T&& keycodes)
            : CmdUnit(std::forward<T>(keycodes))
            {}

            void execute() override ;
        } ;

        class FunctionalCmdUnit : public CmdUnit {
        private:
            std::shared_ptr<bind::BindedFunc> func_ ;

        public:
            template <typename T>
            explicit FunctionalCmdUnit(T&& function_ptr)
            : CmdUnit(),
              func_(std::forward<T>(function_ptr))
            {}

            bool has_function() const noexcept ;

            const std::shared_ptr<bind::BindedFunc>& get_function() const ;

            void execute() override ;
        } ;

        std::unique_ptr<CmdUnit> create_cmdunit(const std::string& strcommand) ;
    }
}

#endif
