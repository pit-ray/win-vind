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
                HotkeyBackspace::create(),
                HotkeyCopy::create(),
                HotkeyCut::create(),
                HotkeyDelete::create(),
                HotkeyPaste::create(),
                ToInsertBOL::create(),
                Change2EdiBkInsert::create(),
                ToInsertEOL::create(),
                ToResident::create(),
                ToEdiVisualLine::create(),
                ToInsertNLAbove::create(),
                ToInsertNLBelow::create(),
                ToEdiNormal::create(),
                ToEdiVisual::create(),
                ToInsert::create(),
                ToGUINormal::create(),
                ToGUIVisual::create(),
                ClickLeft::create(),
                ClickRight::create(),
                CloseCurrentTab::create(),
                CloseCurrentVDesktop::create(),
                CloseCurrentWindow::create(),
                ToCommand::create(),
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
                YankHighlightText::create(),
                YankWithMotion::create(),
                ChangeHighlightText::create(),
                ChangeChar::create(),
                DeleteHighlightText::create(),
                ChangeLine::create(),
                DeleteWithMotion::create(),
                ChangeWithMotion::create(),
                ChangeUntilEOL::create(),
                JumpCaretToBOL::create(),
                JumpCaretToEOL::create(),
                MoveCaretDown::create(),
                MoveCaretLeft::create(),
                MoveCaretRight::create(),
                MoveCaretUp::create(),
                YankLine::create(),
                DeleteAfter::create(),
                DeleteBefore::create(),
                DeleteLine::create(),
                DeleteLineUntilEOL::create(),
                DeleteLineUntilEOL::create(),
                JumpCaretToBOF::create(),
                JumpCaretToEOF::create(),
                MoveCaretNonBlankWordBackward::create(),
                MoveCaretNonBlankWordForward::create(),
                MoveCaretWordBackward::create(),
                MoveCaretWordForward::create(),
                PutAfter::create(),
                PutBefore::create(),
                JoinNextLine::create(),
                ReplaceChar::create(),
                ReplaceSequence::create(),
                SwitchCharCase::create(),
                EnableTargetingOfDedicate2Window::create(),
                ExchangeWindowWithNearest::create(),
                ExitConfigGUI::create(),
                ForwardUINavigation::create(),
                IncreaseWindowHeight::create(),
                IncreaseWindowWidth::create(),
                JumpToActiveWindow::create(),
                JumpWithKeybrdLayout::create(),
                JumpToBottom::create(),
                JumpToLeft::create(),
                JumpToRight::create(),
                JumpToTop::create(),
                JumpToHorizontalCenter::create(),
                JumpToVerticalCenter::create(),
                MakeDir::create(),
                MaximizeCurrentWindow::create(),
                MinimizeCurrentWindow::create(),
                GotoNextPage::create(),
                GotoPrevPage::create(),
                MoveCursorDown::create(),
                MoveCursorLeft::create(),
                MoveCursorRight::create(),
                MoveCursorUp::create(),
                OpenNewWindowWithHorizontalSplit::create(),
                OpenNewWindowWithVerticalSplit::create(),
                OpenNewWindow::create(),
                OpenNewTab::create(),
                Open::create(),
                OpenStartMenu::create(),
                ReloadCurrentWindow::create(),
                ResizeWindowHeight::create(),
                ResizeWindowWidth::create(),
                RotateWindows::create(),
                RotateWindowsInReverse::create(),
                Redo::create(),
                Undo::create(),
                Save::create(),
                ScrollDown::create(),
                ScrollLeft::create(),
                ScrollDownHalfPage::create(),
                ScrollLeftHalfPage::create(),
                ScrollRightHalfPage::create(),
                ScrollUpHalfPage::create(),
                ScrollDownOnePage::create(),
                ScrollUpOnePage::create(),
                ScrollRight::create(),
                ScrollUp::create(),
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
                StartExternal::create(),
                StartExplorer::create(),
                StartShell::create(),
                Switch2LeftTab::create(),
                Switch2RightTab::create(),
                SwitchVDesktop2Left::create(),
                SwitchVDesktop2Right::create(),
                SwitchWindow::create(),
                TaskView::create(),

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
