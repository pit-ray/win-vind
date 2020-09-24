#ifndef _KEY_BINDER_LIST_HPP
#define _KEY_BINDER_LIST_HPP

#include <vector>

#include "move_cursor.hpp"
#include "jump_cursor.hpp"
#include "scroll.hpp"
#include "click.hpp"
#include "change_mode.hpp"
#include "undo.hpp"
#include "clipboard.hpp"
#include "switch_vdesktop.hpp"
#include "search_pattern.hpp"
#include "select.hpp"

#include "edi_change_mode.hpp"
#include "edi_move_caret.hpp"
#include "edi_jump_caret.hpp"
#include "edi_edit.hpp"
#include "edi_replace.hpp"
#include "edi_layout.hpp"

#include "window_ctrl.hpp"
#include "pager.hpp"
#include "filer.hpp"
#include "external_app.hpp"
#include "mywindow_ctrl.hpp"

#include "dedicate_to_window_ctrl.hpp"

namespace KeyBinderList {

    const auto normal() {
        std::vector<kbg::shp_t> tmp {
            Change2Normal::create_with_cache(),
            Change2Insert::create(),
            Change2Visual::create(),
            Change2Editor::create(),
            Change2Command::create_with_cache(),

            SelectAll::create(),

            MoveLeft::create_with_cache(),
            MoveRight::create_with_cache(),
            MoveUp::create_with_cache(),
            MoveDown::create_with_cache(),

            Jump2Left::create_with_cache(),
            Jump2Right::create_with_cache(),
            Jump2Top::create_with_cache(),
            Jump2Bottom::create_with_cache(),
            Jump2XCenter::create_with_cache(),
            Jump2YCenter::create_with_cache(),
            Jump2Any::create_with_cache(),
            Jump2ActiveWindow::create_with_cache(),

            ScrollLeft::create_with_cache(),
            ScrollRight::create_with_cache(),
            ScrollUp::create_with_cache(),
            ScrollDown::create_with_cache(),
            ScrollMidLeft::create_with_cache(),
            ScrollMidRight::create_with_cache(),
            ScrollMidUp::create_with_cache(),
            ScrollMidDown::create_with_cache(),
            ScrollPageUp::create_with_cache(),
            ScrollPageDown::create_with_cache(),

            ClickLeft::create_with_cache(),
            ClickRight::create_with_cache(),

            CBCopy::create_with_cache(),
            CBPaste::create_with_cache(),
            CBCut::create_with_cache(),
            CBDelete::create_with_cache(),
            CBBackSpace::create_with_cache(),

            SCRedo::create_with_cache(),
            SCUndo::create_with_cache(),

            SwitchVDesktop2Left::create(),
            SwitchVDesktop2Right::create(),

            CreateNewVDesktop::create(),
            CloseCurrentVDesktop::create(),
            TaskView::KeyBindingWithCreator::create(),

            Switch2LeftTab::create_with_cache(),
            Switch2RightTab::create_with_cache(),

            SearchPattern::create(),

            CloseCurrentWindow::KeyBindingWithCreator::create(),
            SwitchWindow::KeyBindingWithCreator::create(),
            MaximizeCurrentWindow::KeyBindingWithCreator::create(),
            MinimizeCurrentWindow::KeyBindingWithCreator::create(),
            SnapCurrentWindow2Left::KeyBindingWithCreator::create(),
            SnapCurrentWindow2Right::KeyBindingWithCreator::create(),
            Move2NextPage::create(),
            Move2PrevPage::create(),

            OpenNewCurrentWindow::KeyBindingWithCreator::create(),
            EnableTargetingOfDedicate2Window::KeyBindingWithCreator::create_with_cache(),
            DisableTargetingOfDedicate2Window::KeyBindingWithCreator::create_with_cache()
        } ;
        return tmp ;
    }

    const auto insert() {
        std::vector<kbg::shp_t> tmp {
            Change2Normal::create_with_cache()
        } ;
        return tmp ;
    }

    const auto visual() {
        std::vector<kbg::shp_t> tmp {
            Change2Normal::create_with_cache(),

            MoveLeft::create_with_cache(),
            MoveRight::create_with_cache(),
            MoveUp::create_with_cache(),
            MoveDown::create_with_cache(),

            Jump2Left::create_with_cache(),
            Jump2Right::create_with_cache(),
            Jump2Top::create_with_cache(),
            Jump2Bottom::create_with_cache(),
            Jump2XCenter::create_with_cache(),
            Jump2YCenter::create_with_cache(),
            Jump2Any::create_with_cache(),
            Jump2ActiveWindow::create_with_cache(),

            ScrollLeft::create_with_cache(),
            ScrollRight::create_with_cache(),
            ScrollUp::create_with_cache(),
            ScrollDown::create_with_cache(),
            ScrollMidLeft::create_with_cache(),
            ScrollMidRight::create_with_cache(),
            ScrollMidUp::create_with_cache(),
            ScrollMidDown::create_with_cache(),
            ScrollPageUp::create_with_cache(),
            ScrollPageDown::create_with_cache(),

            ClickLeft::create_with_cache(),
            ClickRight::create_with_cache(),

            CBCopy::create_with_cache(),
            CBPaste::create_with_cache(),
            CBCut::create_with_cache(),
            CBDelete::create_with_cache(),
            CBBackSpace::create_with_cache()
        } ;
        return tmp ;
    }

    const auto editor_normal() {
        std::vector<kbg::shp_t> tmp {
            Change2Normal::create_with_cache(),
            Change2Command::create_with_cache(),
            Change2EdiInsert::create(),
            Change2EdiBOLInsert::create(),
            Change2EdiBkInsert::create(),
            Change2EdiEOLInsert::create(),
            Change2EdiNlInsertBelow::create(),
            Change2EdiNlInsertAbove::create(),
            Change2EdiVisual::create(),
            Change2EdiLineVisual::create(),

            EdiNCopyLine::create(),
            EdiNPasteAfter::create_with_cache(),
            EdiNPasteBefore::create_with_cache(),
            EdiNDeleteLine::create_with_cache(),
            EdiNDeleteLineUntilEOL::create_with_cache(),
            EdiNDeleteAfter::create_with_cache(),
            EdiNDeleteBefore::create_with_cache(),

            EdiNRemoveEOL::create_with_cache(),

            EdiNReplaceChar::create(),
            EdiNReplaceSequence::create(),

            SCRedo::create_with_cache(),
            SCUndo::create_with_cache(),

            ScrollLeft::create_with_cache(),
            ScrollRight::create_with_cache(),
            ScrollUp::create_with_cache(),
            ScrollDown::create_with_cache(),
            ScrollMidLeft::create_with_cache(),
            ScrollMidRight::create_with_cache(),
            ScrollMidUp::create_with_cache(),
            ScrollMidDown::create_with_cache(),
            ScrollPageUp::create_with_cache(),
            ScrollPageDown::create_with_cache(),

            Switch2LeftTab::create_with_cache(),
            Switch2RightTab::create_with_cache(),

            EdiMoveCaretLeft::create_with_cache(),
            EdiMoveCaretRight::create_with_cache(),
            EdiMoveCaretUp::create_with_cache(),
            EdiMoveCaretDown::create_with_cache(),

            EdiNMoveCaretWORDSForward::create_with_cache(),
            EdiNMoveCaretWORDSBackward::create_with_cache(),
            EdiNMoveCaretwordsForward::create_with_cache(),
            EdiNMoveCaretwordsBackward::create_with_cache(),

            EdiJumpCaret2BOL::create_with_cache(),
            EdiJumpCaret2EOL::create_with_cache(),
            EdiNJumpCaret2Line_DfBOF::create_with_cache(),
            EdiNJumpCaret2Line_DfEOF::create_with_cache(),

            EnableTargetingOfDedicate2Window::KeyBindingWithCreator::create_with_cache(),
            DisableTargetingOfDedicate2Window::KeyBindingWithCreator::create_with_cache()
        } ;
        return tmp ;
    }

    const auto editor_insert() {
        std::vector<kbg::shp_t> tmp {
            Change2Normal::create_with_cache(),
            Change2EdiNormal::create_with_cache()
        } ;
        return tmp ;
    }

    const auto editor_visual() {
        std::vector<kbg::shp_t> tmp {
            Change2Normal::create_with_cache(),
            Change2EdiNormal::create_with_cache(),

            EdiCopyHighlightText::create(),

            EdiNPasteAfter::create_with_cache(),
            EdiNPasteBefore::create_with_cache(),

            EdiDeleteHighlightText::create(),
            EdiNDeleteLineUntilEOL::create_with_cache(),
            EdiNDeleteAfter::create_with_cache(),
            EdiNDeleteBefore::create_with_cache(),

            EdiNRemoveEOL::create_with_cache(),

            SCRedo::create_with_cache(),
            SCUndo::create_with_cache(),

            ScrollLeft::create_with_cache(),
            ScrollRight::create_with_cache(),
            ScrollUp::create_with_cache(),
            ScrollDown::create_with_cache(),
            ScrollMidLeft::create_with_cache(),
            ScrollMidRight::create_with_cache(),
            ScrollMidUp::create_with_cache(),
            ScrollMidDown::create_with_cache(),
            ScrollPageUp::create_with_cache(),
            ScrollPageDown::create_with_cache(),

            EdiMoveCaretLeft::create_with_cache(),
            EdiMoveCaretRight::create_with_cache(),
            EdiMoveCaretUp::create_with_cache(),
            EdiMoveCaretDown::create_with_cache(),

            EdiNMoveCaretWORDSForward::create_with_cache(),
            EdiNMoveCaretWORDSBackward::create_with_cache(),
            EdiNMoveCaretwordsForward::create_with_cache(),
            EdiNMoveCaretwordsBackward::create_with_cache(),

            EdiJumpCaret2BOL::create_with_cache(),
            EdiJumpCaret2EOL::create_with_cache(),
            EdiNJumpCaret2Line_DfBOF::create_with_cache(),
            EdiNJumpCaret2Line_DfEOF::create_with_cache()
        } ;
        return tmp ;
    }


    const auto command() {
        std::vector<cmd::shp_t> tmp {
            SaveOpenedFile::create(),
            CloseOpenedFile::create(),
            CloseCurrentWindow::CommandWithCreator::create(),
            SwitchWindow::CommandWithCreator::create(),
            MaximizeCurrentWindow::CommandWithCreator::create(),
            MinimizeCurrentWindow::CommandWithCreator::create(),
            SnapCurrentWindow2Left::CommandWithCreator::create(),
            SnapCurrentWindow2Right::CommandWithCreator::create(),
            OpenNewCurrentWindow::CommandWithCreator::create(),
            ReloadCurrentWindow::create(),
            StartShell::create(),
            StartAnyApp::create(),
            ShowConfigWindow::create(),
            ExitConfigWindow::create(),
            OpenOtherFile::create(),
            MakeDir::create(),
            EnableTargetingOfDedicate2Window::CommandWithCreator::create(),
            DisableTargetingOfDedicate2Window::CommandWithCreator::create(),
            TaskView::CommandWithCreator::create()
        } ;
        return tmp ;
    }
}
#endif
