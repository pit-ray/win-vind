# ifndef _VIRTUAL_CMD_LINE_HPP
#define _VIRTUAL_CMD_LINE_HPP

#include "option_creator.hpp"

namespace vind
{
    class VirtualCmdLine : public OptionCreator<VirtualCmdLine>
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void do_enable() const override ;
        void do_disable() const override ;
        void do_process() const override ;

    public:
        explicit VirtualCmdLine() ;

        static void cout(std::string&& str) ;
        static void cout(const std::string& str) ;
        static void msgout(const std::string& str) ;

        static void refresh() ;
        static void clear() ;
        static void reset() ;

        virtual ~VirtualCmdLine() noexcept ;
        VirtualCmdLine(VirtualCmdLine&&) ;
        VirtualCmdLine& operator=(VirtualCmdLine&&) ;
        VirtualCmdLine(const VirtualCmdLine&)            = delete ;
        VirtualCmdLine& operator=(const VirtualCmdLine&) = delete ;
    } ;
}
#endif
