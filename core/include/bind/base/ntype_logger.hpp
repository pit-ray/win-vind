#ifndef _NTYPE_LOGGER_HPP
#define _NTYPE_LOGGER_HPP

#include "key_logger_base.hpp"

#define NTYPE_HEAD_NUM(result)  ((result) == -1)
#define NTYPE_EMPTY(result)     ((result) == 0)
#define NTYPE_LOGGED(result)    ((result) > 0)

namespace vind
{
    class NTypeLogger : public KeyLoggerBase {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        virtual bool parse_head_number(KeyLog& log) ;

        virtual int logging_first_log(const KeyLog& log) ;

        virtual int logging_pressing_log(const KeyLog& log) ;

        virtual int logging_sequence_log(const KeyLog& log) ;

        virtual int do_logging_state(const KeyLog& log) ;

        virtual bool is_changed() const noexcept override ;

    public:
        explicit NTypeLogger() ;
        virtual ~NTypeLogger() noexcept ;

        NTypeLogger(const NTypeLogger&) ;
        NTypeLogger& operator=(const NTypeLogger&) ;

        NTypeLogger(NTypeLogger&&) ;
        NTypeLogger& operator=(NTypeLogger&&) ;

        virtual int logging_state() override ;
        virtual unsigned int get_head_num() const noexcept ;

        virtual bool is_long_pressing() const noexcept ;

        virtual void clear() noexcept override ;

        virtual void accept() noexcept ;
        virtual void ignore() noexcept ;

        virtual void update() override ;
    } ;
}

#endif
