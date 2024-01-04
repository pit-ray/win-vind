#ifndef _FOCUSTEXTAREA_HPP
#define _FOCUSTEXTAREA_HPP

#include "bind/bindedfunc.hpp"

#include <memory>


namespace vind
{
    namespace bind
    {
        class FocusTextArea : public BindedFuncVoid<FocusTextArea> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            explicit FocusTextArea() ;
            virtual ~FocusTextArea() noexcept ;

            void sprocess(
                std::uint16_t count, const std::string& args) ;

            FocusTextArea(FocusTextArea&&) ;
            FocusTextArea& operator=(FocusTextArea&&) ;
            FocusTextArea(const FocusTextArea&) = delete ;
            FocusTextArea& operator=(const FocusTextArea&) = delete ;
        } ;
    }
}

#endif
