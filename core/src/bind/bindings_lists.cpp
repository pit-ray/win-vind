#include "bind/bindings_lists.hpp"

#include "bind/binded_func.hpp"

#include "bind/ctrl/dedicate_to_window_ctrl.hpp"
#include "bind/ctrl/mywindow_ctrl.hpp"

#include "bind/emu/edi_change.hpp"
#include "bind/emu/edi_change_mode.hpp"
#include "bind/emu/edi_delete.hpp"
#include "bind/emu/edi_jump_caret.hpp"
#include "bind/emu/edi_layout.hpp"
#include "bind/emu/edi_motion.hpp"
#include "bind/emu/edi_move_caret.hpp"
#include "bind/emu/edi_put.hpp"
#include "bind/emu/edi_replace.hpp"
#include "bind/emu/edi_yank.hpp"

#include "bind/file/filer.hpp"
#include "bind/file/mkdir.hpp"

#include "bind/mode/change_mode.hpp"
#include "bind/mode/command_mode.hpp"
#include "bind/mode/instant_mode.hpp"

#include "bind/mouse/click.hpp"
#include "bind/mouse/jump_actwin.hpp"
#include "bind/mouse/jump_cursor.hpp"
#include "bind/mouse/jump_keybrd.hpp"
#include "bind/mouse/move_cursor.hpp"
#include "bind/mouse/scroll.hpp"
#include "bind/mouse/select.hpp"

#include "bind/proc/execute.hpp"
#include "bind/proc/external_app.hpp"
#include "bind/proc/open_window.hpp"

#include "bind/easyclick/easy_click.hpp"

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
#include "bind/window/window_resizer.hpp"

#include "bind/syscmd/command.hpp"
#include "bind/syscmd/map.hpp"
#include "bind/syscmd/set.hpp"
#include "bind/syscmd/source.hpp"

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
