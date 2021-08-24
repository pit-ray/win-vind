#ifndef _BLOCKSTYLE_CARET_HPP
#define _BLOCKSTYLE_CARET_HPP

#include "option_creator.hpp"


namespace vind
{
    class BlockStyleCaret : public OptionCreator<BlockStyleCaret>
    {
    private:
        struct Impl ;
        std::unique_ptr<Impl> pimpl ;

        void do_enable() const override ;
        void do_disable() const override ;
        void do_process() const override ;

    public:
        explicit BlockStyleCaret() ;

        virtual ~BlockStyleCaret() noexcept ;

        BlockStyleCaret(BlockStyleCaret&&) ;
        BlockStyleCaret& operator=(BlockStyleCaret&&) ;

        BlockStyleCaret(const BlockStyleCaret&)            = delete ;
        BlockStyleCaret& operator=(const BlockStyleCaret&) = delete ;

        void restore_caret_style(const std::string& mode) const ;

        void make_caret_block_style(const std::string& mode) const ;
    } ;
}


#endif
