# ifndef _VCMDLINE_HPP
#define _VCMDLINE_HPP

#include "core/errlogger.hpp"
#include "message.hpp"
#include "option.hpp"

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

            void do_enable() override ;
            void do_disable() override ;
            void do_process() override ;

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
                    core::Logger::get_instance().message(msg_) ;
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

            void update_drawing_coordinates() ;

            VCmdLine(const VCmdLine&)            = delete ;
            VCmdLine& operator=(const VCmdLine&) = delete ;
        } ;
    }
}

#endif
