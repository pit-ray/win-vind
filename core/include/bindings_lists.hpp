#ifndef _BINDINGS_LISTS_HPP
#define _BINDINGS_LISTS_HPP

#include "change_mode.hpp"
#include "click.hpp"
#include "clipboard.hpp"
#include "command_mode.hpp"
#include "dedicate_to_window_ctrl.hpp"
#include "easy_click.hpp"
#include "edi_change_mode.hpp"
#include "edi_edit.hpp"
#include "edi_jump_caret.hpp"
#include "edi_layout.hpp"
#include "edi_move_caret.hpp"
#include "edi_replace.hpp"
#include "external_app.hpp"
#include "filer.hpp"
#include "mkdir.hpp"
#include "jump_cursor.hpp"
#include "jump_keybrd.hpp"
#include "move_cursor.hpp"
#include "mywindow_ctrl.hpp"
#include "pager.hpp"
#include "resize_window.hpp"
#include "scroll.hpp"
#include "search_pattern.hpp"
#include "select.hpp"
#include "select_window.hpp"
#include "switch_vdesktop.hpp"
#include "tab_ctrl.hpp"
#include "undo.hpp"
#include "jump_actwin.hpp"

#include "arrange_window.hpp"
#include "close_window.hpp"
#include "exchange_window.hpp"
#include "min_max_window.hpp"
#include "open_window.hpp"
#include "reload_window.hpp"
#include "rotate_window.hpp"
#include "snap_window.hpp"
#include "split_open_window.hpp"
#include "switch_window.hpp"

namespace vind
{
    namespace BindingsLists {
        const auto get() {
            std::vector<BindedFunc::shp_t> tmp {
                ArrangeWindows::create(),
                BackwardUINavigation::create(),
                CBBackSpace::create(),
                CBCopy::create(),
                CBCut::create(),
                CBDelete::create(),
                CBPaste::create(),
                Change2EdiBOLInsert::create(),
                Change2EdiBkInsert::create(),
                Change2EdiEOLInsert::create(),
                Change2EdiInsert::create(),
                Change2EdiLineVisual::create(),
                Change2EdiNlInsertAbove::create(),
                Change2EdiNlInsertBelow::create(),
                Change2EdiNormal::create(),
                Change2EdiVisual::create(),
                Change2Insert::create(),
                Change2Normal::create(),
                Change2Visual::create(),
                ClickLeft::create(),
                ClickRight::create(),
                CloseCurrentTab::create(),
                CloseCurrentVDesktop::create(),
                CloseCurrentWindow::create(),
                CommandMode::create(),
                CreateNewVDesktop::create(),
                DecideFocusedUIObject::create(),
                DecreaseWindowHeight::create(),
                DecreaseWindowWidth::create(),
                DisableTargetingOfDedicate2Window::create(),
                DisableTargetingOfDedicate2Window::create(),
                EasyClickHover::create(),
                EasyClickLeft::create(),
                EasyClickMid::create(),
                EasyClickRight::create(),
                EdiCopyHighlightText::create(),
                EdiCopyMotion::create(),
                EdiDeleteCharsAndStartInsert::create(),
                EdiDeleteHighlightText::create(),
                EdiDeleteLinesAndStartInsert::create(),
                EdiDeleteMotion::create(),
                EdiDeleteMotionAndStartInsert::create(),
                EdiDeleteUntilEOLAndStartInsert::create(),
                EdiJumpCaret2BOL::create(),
                EdiJumpCaret2EOL::create(),
                EdiMoveCaretDown::create(),
                EdiMoveCaretLeft::create(),
                EdiMoveCaretRight::create(),
                EdiMoveCaretUp::create(),
                EdiNCopyLine::create(),
                EdiNDeleteAfter::create(),
                EdiNDeleteBefore::create(),
                EdiNDeleteLine::create(),
                EdiNDeleteLineUntilEOL::create(),
                EdiNDeleteLineUntilEOL::create(),
                EdiNJumpCaret2Line_DfBOF::create(),
                EdiNJumpCaret2Line_DfEOF::create(),
                EdiNMoveCaretWORDSBackward::create(),
                EdiNMoveCaretWORDSForward::create(),
                EdiNMoveCaretwordsBackward::create(),
                EdiNMoveCaretwordsForward::create(),
                EdiNPasteAfter::create(),
                EdiNPasteBefore::create(),
                EdiNRemoveEOL::create(),
                EdiNReplaceChar::create(),
                EdiNReplaceSequence::create(),
                EdiSwitchCharCase::create(),
                EnableTargetingOfDedicate2Window::create(),
                ExchangeWindowWithNextOne::create(),
                ExitConfigWindow::create(),
                ForwardUINavigation::create(),
                IncreaseWindowHeight::create(),
                IncreaseWindowWidth::create(),
                Jump2ActiveWindow::create(),
                Jump2Any::create(),
                Jump2Bottom::create(),
                Jump2Left::create(),
                Jump2Right::create(),
                Jump2Top::create(),
                Jump2XCenter::create(),
                Jump2YCenter::create(),
                MakeDir::create(),
                MaximizeCurrentWindow::create(),
                MinimizeCurrentWindow::create(),
                Move2NextPage::create(),
                Move2PrevPage::create(),
                MoveDown::create(),
                MoveLeft::create(),
                MoveRight::create(),
                MoveUp::create(),
                MyConfigWindowInsert::create(),
                MyConfigWindowNormal::create(),
                OpenNewCurWinWithHorizontalSplit::create(),
                OpenNewCurWinWithVerticalSplit::create(),
                OpenNewCurrentWindow::create(),
                OpenNewTab::create(),
                OpenOtherFile::create(),
                OpenStartMenu::create(),
                ReloadCurrentWindow::create(),
                ResizeWindowHeight::create(),
                ResizeWindowWidth::create(),
                RotateWindows::create(),
                RotateWindowsInReverse::create(),
                SCRedo::create(),
                SCUndo::create(),
                SaveOpenedFile::create(),
                ScrollDown::create(),
                ScrollLeft::create(),
                ScrollMidDown::create(),
                ScrollMidLeft::create(),
                ScrollMidRight::create(),
                ScrollMidUp::create(),
                ScrollPageDown::create(),
                ScrollPageUp::create(),
                ScrollRight::create(),
                ScrollUp::create(),
                SearchPattern::create(),
                SelectAll::create(),
                SelectLeftWindow::create(),
                SelectLowerWindow::create(),
                SelectRightWindow::create(),
                SelectUpperWindow::create(),
                ShowConfigWindow::create(),
                SnapCurrentWindow2Bottom::create(),
                SnapCurrentWindow2Left::create(),
                SnapCurrentWindow2Right::create(),
                SnapCurrentWindow2Top::create(),
                StartAnyApp::create(),
                StartExplorer::create(),
                StartShell::create(),
                Switch2LeftTab::create(),
                Switch2RightTab::create(),
                SwitchVDesktop2Left::create(),
                SwitchVDesktop2Right::create(),
                SwitchWindow::create(),
                TaskView::create()
            } ;
            return tmp ;
        }
    }
}
#endif
