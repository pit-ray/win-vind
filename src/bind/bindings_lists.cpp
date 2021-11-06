#include "bindings_lists.hpp"

#include "binded_func.hpp"

#include "ctrl/dedicate_to_window_ctrl.hpp"
#include "ctrl/mywindow_ctrl.hpp"

#include "emu/edi_change.hpp"
#include "emu/edi_change_mode.hpp"
#include "emu/edi_delete.hpp"
#include "emu/edi_dot.hpp"
#include "emu/edi_jump_caret.hpp"
#include "emu/edi_layout.hpp"
#include "emu/edi_motion.hpp"
#include "emu/edi_move_caret.hpp"
#include "emu/edi_put.hpp"
#include "emu/edi_replace.hpp"
#include "emu/edi_yank.hpp"

#include "file/filer.hpp"
#include "file/mkdir.hpp"

#include "mode/change_mode.hpp"
#include "mode/command_mode.hpp"
#include "mode/instant_mode.hpp"

#include "mouse/click.hpp"
#include "mouse/jump_actwin.hpp"
#include "mouse/jump_cursor.hpp"
#include "mouse/jump_keybrd.hpp"
#include "mouse/move_cursor.hpp"
#include "mouse/scroll.hpp"
#include "mouse/select.hpp"

#include "proc/execute.hpp"
#include "proc/external_app.hpp"
#include "proc/open_window.hpp"

#include "easyclick/easy_click.hpp"

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
#include "window/teleport_vdesktop.hpp"
#include "window/window_resizer.hpp"

#include "syscmd/command.hpp"
#include "syscmd/map.hpp"
#include "syscmd/set.hpp"
#include "syscmd/source.hpp"

namespace vind
{
    namespace bindingslists {
        const std::vector<BindedFunc::SPtr>& get() {
            static const std::vector<BindedFunc::SPtr> tmp {
                ArrangeWindows::create(),
                BackwardUINavigation::create(),
                Change2EdiBkInsert::create(),
                ChangeChar::create(),
                ChangeHighlightText::create(),
                ChangeLine::create(),
                ChangeUntilEOL::create(),
                ChangeWithMotion::create(),
                ClickLeft::create(),
                ClickRight::create(),
                CloseCurrentTab::create(),
                CloseCurrentVDesktop::create(),
                CloseCurrentWindow::create(),
                CreateNewVDesktop::create(),
                DecideFocusedUIObject::create(),
                DecreaseWindowHeight::create(),
                DecreaseWindowWidth::create(),
                DeleteAfter::create(),
                DeleteBefore::create(),
                DeleteHighlightText::create(),
                DeleteLine::create(),
                DeleteLineUntilEOL::create(),
                DeleteLineUntilEOL::create(),
                DeleteWithMotion::create(),
                DisableTargetingOfDedicate2Window::create(),
                DisableTargetingOfDedicate2Window::create(),
                EasyClickHover::create(),
                EasyClickLeft::create(),
                EasyClickMid::create(),
                EasyClickRight::create(),
                EnableTargetingOfDedicate2Window::create(),
                ExchangeWindowWithNearest::create(),
                Execute::create(),
                ExitConfigGUI::create(),
                ForwardUINavigation::create(),
                GotoNextPage::create(),
                GotoPrevPage::create(),
                HotkeyBackspace::create(),
                HotkeyCopy::create(),
                HotkeyCut::create(),
                HotkeyDelete::create(),
                HotkeyPaste::create(),
                IncreaseWindowHeight::create(),
                IncreaseWindowWidth::create(),
                JoinNextLine::create(),
                JumpCaretToBOF::create(),
                JumpCaretToBOL::create(),
                JumpCaretToEOF::create(),
                JumpCaretToEOL::create(),
                JumpToActiveWindow::create(),
                JumpToBottom::create(),
                JumpToHorizontalCenter::create(),
                JumpToLeft::create(),
                JumpToRight::create(),
                JumpToTop::create(),
                JumpToVerticalCenter::create(),
                JumpWithKeybrdLayout::create(),
                MakeDir::create(),
                MaximizeCurrentWindow::create(),
                MinimizeCurrentWindow::create(),
                MoveCaretDown::create(),
                MoveCaretLeft::create(),
                MoveCaretNonBlankWordBackward::create(),
                MoveCaretNonBlankWordForward::create(),
                MoveCaretRight::create(),
                MoveCaretUp::create(),
                MoveCaretWordBackward::create(),
                MoveCaretWordForward::create(),
                MoveCursorDown::create(),
                MoveCursorLeft::create(),
                MoveCursorRight::create(),
                MoveCursorUp::create(),
                Open::create(),
                OpenNewTab::create(),
                OpenNewWindow::create(),
                OpenNewWindowWithHorizontalSplit::create(),
                OpenNewWindowWithVerticalSplit::create(),
                OpenStartMenu::create(),
                PutAfter::create(),
                PutBefore::create(),
                Redo::create(),
                ReloadCurrentWindow::create(),
                RepeatLastChange::create(),
                ReplaceChar::create(),
                ReplaceSequence::create(),
                ResizeWindowHeight::create(),
                ResizeWindowWidth::create(),
                RotateWindows::create(),
                RotateWindowsInReverse::create(),
                Save::create(),
                ScrollDown::create(),
                ScrollDownHalfPage::create(),
                ScrollDownOnePage::create(),
                ScrollLeft::create(),
                ScrollLeftHalfPage::create(),
                ScrollRight::create(),
                ScrollRightHalfPage::create(),
                ScrollUp::create(),
                ScrollUpHalfPage::create(),
                ScrollUpOnePage::create(),
                SearchPattern::create(),
                SelectAll::create(),
                SelectLeftWindow::create(),
                SelectLowerWindow::create(),
                SelectRightWindow::create(),
                SelectUpperWindow::create(),
                ShowConfigGUI::create(),
                SnapCurrentWindow2Bottom::create(),
                SnapCurrentWindow2Left::create(),
                SnapCurrentWindow2Right::create(),
                SnapCurrentWindow2Top::create(),
                StartExplorer::create(),
                StartExternal::create(),
                StartShell::create(),
                Switch2LeftTab::create(),
                Switch2RightTab::create(),
                SwitchCharCase::create(),
                SwitchVDesktop2Left::create(),
                SwitchVDesktop2Right::create(),
                SwitchWindow::create(),
                TaskView::create(),
                TeleportOverVDesktop::create(),
                ToCommand::create(),
                ToEdiNormal::create(),
                ToEdiVisual::create(),
                ToEdiVisualLine::create(),
                ToGUINormal::create(),
                ToGUIVisual::create(),
                ToInsert::create(),
                ToInsertBOL::create(),
                ToInsertEOL::create(),
                ToInsertNLAbove::create(),
                ToInsertNLBelow::create(),
                ToInstantGUINormal::create(),
                ToResident::create(),
                Undo::create(),
                WindowResizer::create(),
                YankHighlightText::create(),
                YankLine::create(),
                YankWithMotion::create(),

                SyscmdSet::create(),
                SyscmdMap::create(),
                SyscmdNoremap::create(),
                SyscmdUnmap::create(),
                SyscmdMapclear::create(),
                SyscmdCommand::create(),
                SyscmdDelcommand::create(),
                SyscmdComclear::create(),
                SyscmdSource::create()
            } ;
            return tmp ;
        }
    }
}
