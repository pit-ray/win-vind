#ifndef _LOGMAP_TEMPLATE_HPP
#define _LOGMAP_TEMPLATE_HPP

#include "bind/binded_func.hpp"
#include "core/mapdefs.hpp"


namespace vind
{
    namespace bind
    {
        class LogPoolMap : public BindedFunc {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void do_process() const override ;
            void do_process(core::NTypeLogger& parent_lgr) const override ;
            void do_process(const core::CharLogger& parent_lgr) const override ;

            static std::size_t id_ ;

        public:
            explicit LogPoolMap(const Command& cmd) ;
            virtual ~LogPoolMap() noexcept ;

            static std::unique_ptr<BindedFunc> create(const Command& cmd) ;

            LogPoolMap(LogPoolMap&&) ;
            LogPoolMap& operator=(LogPoolMap&&) ;

            LogPoolMap(const LogPoolMap&) = delete ;
            LogPoolMap& operator=(const LogPoolMap&) = delete ;
        } ;


        class KeyCodeMap : public BindedFunc {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void do_process() const override ;
            void do_process(core::NTypeLogger& parent_lgr) const override ;
            void do_process(const core::CharLogger& parent_lgr) const override ;

            static std::size_t id_ ;

        public:
            explicit KeyCodeMap(const Command& cmd) ;
            virtual ~KeyCodeMap() noexcept ;

            static std::unique_ptr<BindedFunc> create(const Command& cmd) ;

            KeyCodeMap(KeyCodeMap&&) ;
            KeyCodeMap& operator=(KeyCodeMap&&) ;

            KeyCodeMap(const KeyCodeMap&) = delete ;
            KeyCodeMap& operator=(const KeyCodeMap&) = delete ;
        } ;
    }
}

#endif
