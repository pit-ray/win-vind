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
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit DeleteLine() ;
            virtual ~DeleteLine() noexcept ;

            DeleteLine(DeleteLine&&) ;
            DeleteLine& operator=(DeleteLine&&) ;
            DeleteLine(const DeleteLine&)            = delete ;
            DeleteLine& operator=(const DeleteLine&) = delete ;
        } ;


        class DeleteLineUntilEOL : public ChangeBaseCreator<DeleteLineUntilEOL> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

            explicit DeleteLineUntilEOL() ;
            virtual ~DeleteLineUntilEOL() noexcept ;

            DeleteLineUntilEOL(DeleteLineUntilEOL&&) ;
            DeleteLineUntilEOL& operator=(DeleteLineUntilEOL&&) ;
            DeleteLineUntilEOL(const DeleteLineUntilEOL&)            = delete ;
            DeleteLineUntilEOL& operator=(const DeleteLineUntilEOL&) = delete ;
        } ;

        class DeleteAfter : public ChangeBaseCreator<DeleteAfter> {
        private:
            struct Impl ;
            std::unique_ptr<Impl> pimpl ;

        public:
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

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
            void sprocess(unsigned int count=1) const ;
            void sprocess(core::NTypeLogger& parent_lgr) const ;
            void sprocess(const core::CharLogger& parent_lgr) const ;

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
