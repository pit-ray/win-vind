#include "bindings_lists.hpp"

#include "bind/binded_func.hpp"

#include "ctrl/dedicate_to_window_ctrl.hpp"
#include "ctrl/mywindow_ctrl.hpp"

#include "emu/edi_change_mode.hpp"
#include "emu/edi_edit.hpp"
#include "emu/edi_jump_caret.hpp"
#include "emu/edi_layout.hpp"
#include "emu/edi_move_caret.hpp"
#include "emu/edi_replace.hpp"

#include "file/filer.hpp"
#include "file/mkdir.hpp"

#include "mode/change_mode.hpp"
#include "mode/command_mode.hpp"

#include "mouse/click.hpp"
#include "mouse/jump_actwin.hpp"
#include "mouse/jump_cursor.hpp"
#include "mouse/jump_keybrd.hpp"
#include "mouse/move_cursor.hpp"
#include "mouse/scroll.hpp"
#include "mouse/select.hpp"

#include "proc/external_app.hpp"
#include "proc/open_window.hpp"

#include "uia/easy_click.hpp"

#include "hotkey/clipboard.hpp"
#include "hotkey/pager.hpp"
#include "hotkey/search_pattern.hpp"
#include "hotkey/switch_vdesktop.hpp"
#include "hotkey/tab_ctrl.hpp"
#include "hotkey/undo.hpp"

#include "window/arrange_window.hpp"
#include "window/close_window.hpp"
#include "window/exchange_window.hpp"
#include "window/min_max_window.hpp"
#include "window/reload_window.hpp"
#include "window/resize_window.hpp"
#include "window/rotate_window.hpp"
#include "window/select_window.hpp"
#include "window/snap_window.hpp"
#include "window/split_open_window.hpp"
#include "window/switch_window.hpp"

namespace vind
{
    namespace bindingslists {
        const std::vector<BindedFunc::shp_t> get() {
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
