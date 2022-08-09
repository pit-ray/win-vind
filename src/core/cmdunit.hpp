#ifndef _CMDUNIT_HPP
#define _CMDUNIT_HPP

#include "keycode.hpp"

#include <memory>
#include <string>


namespace vind
{
    namespace core
    {
        class CmdUnit {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            using Data = std::unordered_set<KeyCode> ;
            explicit CmdUnit() ;
            explicit CmdUnit(std::string&& strkeyset, bool external=false) ;
            explicit CmdUnit(const std::string& strkeyset, bool external=false) ;

            virtual ~CmdUnit() noexcept ;

            CmdUnit(const CmdUnit&) = delete ;
            CmdUnit& operator=(const CmdUnit&) = delete ;

            CmdUnit(CmdUnit&&) ;
            CmdUnit& operator=(CmdUnit&&) ;

            Data::const_iterator begin() const noexcept ;
            Data::const_iterator end() const noexcept ;

            Data::const_iterator cbegin() const noexcept ;
            Data::const_iterator cend() const noexcept ;

            std::size_t size() const noexcept ;
            std::size_t length() const noexcept ;

            bool for_external() const noexcept ;
            bool for_internal() const noexcept ;

            void execute() ;
        } ;

        class InternalCmdUnit : public CmdUnit {
        private:

        public:
            explicit InternalCmdUnit() ;
            explicit InternalCmdUnit(std::string&& strkeyset, bool external=false) ;
            explicit InternalCmdUnit(const std::string& strkeyset, bool external=false) ;

            virtual ~InternalCmdUnit() noexcept ;
        } ;

        class ExternalCmdUnit : public CmdUnit {
        private:

        public:
            explicit ExternalCmdUnit() ;
            explicit ExternalCmdUnit(std::string&& strkeyset, bool external=false) ;
            explicit ExternalCmdUnit(const std::string& strkeyset, bool external=false) ;

            virtual ~ExternalCmdUnit() noexcept ;
        } ;
    }
}

#endif
