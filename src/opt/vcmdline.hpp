# ifndef _VCMDLINE_HPP
#define _VCMDLINE_HPP

#include "core/errlogger.hpp"
#include "message.hpp"
#include "option_creator.hpp"

#include <chrono>


namespace vind
{
    namespace opt
    {
        struct GeneralMessage : public Message {
            template <typename T>
            GeneralMessage(T&& str)
            : Message(std::forward<T>(str), MessageFlag::FADING)
            {}
        } ;


        struct ErrorMessage : public Message {
            template <typename T>
            ErrorMessage(T&& str)
            : Message(
                    std::forward<T>(str),
                    static_cast<MessageFlag>(
                        MessageFlag::FADING | MessageFlag::DUMPING | 1))
            {}
        } ;

        struct StaticMessage : public Message {
            template <typename T>
            StaticMessage(T&& str)
            : Message(
                    std::forward<T>(str),
                    static_cast<MessageFlag>(2))
            {}
        } ;


        class VCmdLine : public OptionCreator<VCmdLine>
        {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void update_drawing() const ;

            void do_enable() const override ;
            void do_disable() const override ;
            void do_process() const override ;

            static Message msg_ ;

        public:
            explicit VCmdLine() ;

            virtual ~VCmdLine() noexcept ;

            VCmdLine(VCmdLine&&) ;
            VCmdLine& operator=(VCmdLine&&) ;

            template <typename T>
            static void print(T&& str) {
                if(str.empty()) {
                    return ;
                }

                if(msg_.priority() > str.priority()) {
                    return ;
                }

                auto must_refresh = msg_.size() > str.size() ;

                msg_ = std::forward<T>(str) ;

                if(msg_.dumpable()) {
                    PRINT_MSG(msg_) ;
                }

                if(must_refresh) {
                    refresh() ;
                }
            }

            // Remove old string pixels
            static void refresh() ;

            // Clear the buffer of string
            static void clear() ;

            // Reset the buffer and pixels
            static void reset() ;

            VCmdLine(const VCmdLine&)            = delete ;
            VCmdLine& operator=(const VCmdLine&) = delete ;
        } ;
    }
}

#endif
