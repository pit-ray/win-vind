#ifndef _BLOCKSTYLE_CARET_HPP
#define _BLOCKSTYLE_CARET_HPP

#include "option.hpp"


namespace vind
{
    namespace opt
    {
        class BlockStyleCaret : public OptionCreator<BlockStyleCaret> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

            void do_enable() override ;
            void do_disable() override ;
            void do_process() override ;

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
}


#endif
