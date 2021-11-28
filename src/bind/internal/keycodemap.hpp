#ifndef _KEYCODEMAP_HPP
#define _KEYCODEMAP_HPP

#include "bind/binded_func_creator.hpp"

#include "core/key_logger_base.hpp"
#include "core/mapdefs.hpp"
#include "core/mode.hpp"

#include <queue>
#include <unordered_map>


namespace vind
{
    namespace bind
    {
        class KeycodeMap : public BindedFuncCreator<KeycodeMap> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            static MapArray<std::unordered_map<Command, KeyLoggerBase::Data> logmap_ ;
            static MapArray<std::array<KeyLoggerBase::Data, 256>> syncmap_ ;

        public:
            explicit KeycodeMap() ;
            virtual ~KeycodeMap() noexcept ;

            void sprocess() const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            void reconstruct() override ;

            static void do_sync_map(
                    KeyCode key,
                    bool state,
                    Mode mode=core::get_global_mode()) ;

            static KeyLog do_log_map(
                    const KeyLog& log,
                    Mode mode=core::get_global_mode()) ;

            KeycodeMap(KeycodeMap&&) ;
            KeycodeMap& operator=(KeycodeMap&&) ;
            KeycodeMap(const KeycodeMap&) = delete ;
            KeycodeMap& operator=(const KeycodeMap&) = delete ;
        } ;
    }
}

#endif
