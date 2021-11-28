#include "logmap_template.hpp"

#ifdef DEBUG
#include "core/bindings_parser.hpp"
#endif


namespace vind
{
    namespace bind
    {
        std::size_t LogPoolMap::id_ = 0 ;

        struct LogPoolMap::Impl {
            KeyLoggerBase::Data pool_ ;

            explicit Impl(
                    Command::const_iterator begin_itr,
                    Command::const_iterator end_itr)
            : pool_(begin_itr, end_itr)
            {}
        } ;

        LogPoolMap::LogPoolMap(const Command& cmd)
        : BindedFunc("logpoolmap_" + std::to_string(id_)),
          pimpl(std::make_unique<Impl>(cmd.cbegin(), cmd.cend()))
        {
            id_ ++ ;
        }

        LogPoolMap::~LogPoolMap() noexcept = default ;
        LogPoolMap::LogPoolMap(LogPoolMap&&) = default ;
        LogPoolMap& LogPoolMap::operator=(LogPoolMap&&) = default ;

        void LogPoolMap::do_process() const {
            // add log to parent_lgr
            std::cout << core::print(pimpl->pool_) << std::endl ;
        }

        void LogPoolMap::do_process(core::NTypeLogger& parent_lgr) const {
            do_process() ;
        }

        void LogPoolMap::do_process(const core::CharLogger& parent_lgr) const {
            do_process() ;
        }

        std::unique_ptr<BindedFunc> create(const Command& cmd) {
            return std::make_unique<LogPoolMap>(cmd) ;
        }


        std::size_t KeyCodeMap::id_ = 0 ;

        struct KeyCodeMap::Impl {
            Command cmd_ ;

            explicit Impl(const Command& cmd)
            : pool_(cmd)
            {}
        } ;

        KeyCodeMap::KeyCodeMap(const Command& cmd)
        : BindedFunc("keycodemap_" + std::to_string(id_)),
          pimpl(std::make_unique<Impl>(cmd))
        {
            id_ ++ ;
        }

        KeyCodeMap::~KeyCodeMap() noexcept = default ;
        KeyCodeMap::KeyCodeMap(KeyCodeMap&&) = default ;
        KeyCodeMap& KeyCodeMap::operator=(KeyCodeMap&&) = default ;

        void KeyCodeMap::do_process() const {
            // push up command
            std::cout << core::print(pimpl->cmd_) << std::endl ;
        }

        void KeyCodeMap::do_process(core::NTypeLogger& parent_lgr) const {
            do_process() ;
        }

        void KeyCodeMap::do_process(const core::CharLogger& parent_lgr) const {
            do_process() ;
        }

        std::unique_ptr<BindedFunc> create(const Command& cmd) {
            return std::make_unique<KeyCodeMap>(cmd) ;
        }
    }
}
