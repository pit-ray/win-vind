#ifndef _EDI_DELETE_HPP
#define _EDI_DELETE_HPP

#include "bind/bindedfunc.hpp"
#include "changebase.hpp"


namespace vind
{
    namespace bind
    {
        //Delete
        struct DeleteHighlightText : public BindedFuncVoid<DeleteHighlightText> {
            explicit DeleteHighlightText() ;
            static void sprocess() ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger& parent_lgr) ;
        } ;

        class DeleteLine : public ChangeBaseCreator<DeleteLine> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit DeleteLine() ;
            virtual ~DeleteLine() noexcept ;

            DeleteLine(DeleteLine&&) ;
            DeleteLine& operator=(DeleteLine&&) ;
            DeleteLine(const DeleteLine&)            = delete ;
            DeleteLine& operator=(const DeleteLine&) = delete ;
        } ;

        struct DeleteLineUntilEOL : public ChangeBaseCreator<DeleteLineUntilEOL> {
            explicit DeleteLineUntilEOL() ;
            static void sprocess(unsigned int count=1) ;
            static void sprocess(core::NTypeLogger& parent_lgr) ;
            static void sprocess(const core::CharLogger&) ;
        } ;


        class DeleteAfter : public ChangeBaseCreator<DeleteAfter> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit DeleteAfter() ;
            virtual ~DeleteAfter() noexcept ;

            DeleteAfter(DeleteAfter&&) ;
            DeleteAfter& operator=(DeleteAfter&&) ;
            DeleteAfter(const DeleteAfter&)            = delete ;
            DeleteAfter& operator=(const DeleteAfter&) = delete ;
        } ;

        class DeleteBefore : public ChangeBaseCreator<DeleteBefore> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) ;
            void sprocess(core::NTypeLogger& parent_lgr) ;
            void sprocess(const core::CharLogger& parent_lgr) ;

            explicit DeleteBefore() ;
            virtual ~DeleteBefore() noexcept ;

            DeleteBefore(DeleteBefore&&) ;
            DeleteBefore& operator=(DeleteBefore&&) ;
            DeleteBefore(const DeleteBefore&)            = delete ;
            DeleteBefore& operator=(const DeleteBefore&) = delete ;
        } ;
    }
}

#endif
