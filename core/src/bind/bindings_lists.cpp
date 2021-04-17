#include "bind/bindings_lists.hpp"

#include "bind/base/binded_func.hpp"

#include "bind/ctrl/dedicate_to_window_ctrl.hpp"
#include "bind/ctrl/mywindow_ctrl.hpp"

#include "bind/emu/edi_change_mode.hpp"
#include "bind/emu/edi_edit.hpp"
#include "bind/emu/edi_jump_caret.hpp"
#include "bind/emu/edi_layout.hpp"
#include "bind/emu/edi_motion.hpp"
#include "bind/emu/edi_move_caret.hpp"
#include "bind/emu/edi_replace.hpp"

#include "bind/file/filer.hpp"
#include "bind/file/mkdir.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/mode/command_mode.hpp"

#include "bind/mouse/click.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "bind/mouse/jump_cursor.hpp"
#include "bind/mouse/jump_keybrd.hpp"
#include "bind/mouse/move_cursor.hpp"
#include "bind/mouse/scroll.hpp"
#include "bind/mouse/select.hpp"

#include "bind/proc/external_app.hpp"
#include "bind/proc/open_window.hpp"

#include "bind/uia/easy_click.hpp"

#include "bind/hotkey/clipboard.hpp"
#include "bind/hotkey/pager.hpp"
#include "bind/hotkey/search_pattern.hpp"
#include "bind/hotkey/switch_vdesktop.hpp"
#include "bind/hotkey/tab_ctrl.hpp"
#include "bind/hotkey/undo.hpp"

#include "bind/window/arrange_window.hpp"
#include "bind/window/close_window.hpp"
#include "bind/window/exchange_window.hpp"
#include "bind/window/min_max_window.hpp"
#include "bind/window/reload_window.hpp"
#include "bind/window/resize_window.hpp"
#include "bind/window/rotate_window.hpp"
#include "bind/window/select_window.hpp"
#include "bind/window/snap_window.hpp"
#include "bind/window/split_open_window.hpp"
#include "bind/window/switch_window.hpp"

namespace vind
{
    namespace bindingslists {
        const std::vector<BindedFunc::SPtr> get() {
            std::vector<BindedFunc::SPtr> tmp {
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
