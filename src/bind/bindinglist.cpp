#include "bindinglist.hpp"

#include "bindedfunc.hpp"

#include "ctrl/guictrl.hpp"
#include "ctrl/sleep.hpp"

#include "emu/changetext.hpp"
#include "emu/deltext.hpp"
#include "emu/dot.hpp"
#include "emu/jumpcaret.hpp"
#include "emu/layouttext.hpp"
#include "emu/motion.hpp"
#include "emu/movecaret.hpp"
#include "emu/moveinsert.hpp"
#include "emu/puttext.hpp"
#include "emu/replacetext.hpp"
#include "emu/wordmotion.hpp"
#include "emu/yanktext.hpp"

#include "file/filer.hpp"
#include "file/mkdir.hpp"

#include "mode/change_mode.hpp"
#include "mode/command_mode.hpp"
#include "mode/instant_mode.hpp"

#include "mouse/click.hpp"
#include "mouse/easyclick.hpp"
#include "mouse/gridmove.hpp"
#include "mouse/jump_actwin.hpp"
#include "mouse/jump_cursor.hpp"
#include "mouse/jump_keybrd.hpp"
#include "mouse/movecursor.hpp"
#include "mouse/scroll.hpp"
#include "mouse/select.hpp"

#include "proc/exapp.hpp"
#include "proc/execute.hpp"
#include "proc/openwin.hpp"

#include "hotkey/clipboard.hpp"
#include "hotkey/pager.hpp"
#include "hotkey/search.hpp"
#include "hotkey/tab.hpp"
#include "hotkey/undo.hpp"
#include "hotkey/vdesktop.hpp"

#include "window/arrange_win.hpp"
#include "window/close_win.hpp"
#include "window/exchange_win.hpp"
#include "window/minmax_win.hpp"
#include "window/reload_win.hpp"
#include "window/resize_win.hpp"
#include "window/rotate_win.hpp"
#include "window/select_win.hpp"
#include "window/snap_win.hpp"
#include "window/split_win.hpp"
#include "window/switch_win.hpp"
#include "window/teleport_vdesktop.hpp"
#include "window/winresizer.hpp"

#include "syscmd/autocmd.hpp"
#include "syscmd/command.hpp"
#include "syscmd/mapvar.hpp"
#include "syscmd/set.hpp"
#include "syscmd/source.hpp"

#include "util/debug.hpp"
#include "util/string.hpp"


namespace vind
{
    namespace bind
    {
        const std::vector<BindedFunc::SPtr>& all_global_binded_funcs() {
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
                ClickMid::create(),
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
                EasyClick::create(),
                EasyClickAll::create(),
                GridMove::create(),
                ExchangeWindowWithNearest::create(),
                Execute::create(),
                ExitConfigGUI::create(),
                Exit::create(),
                Sleep::create(),
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
                MoveBckBigWord::create(),
                MoveBckEndBigWord::create(),
                MoveBckEndWord::create(),
                MoveBckWord::create(),
                MoveBckWordSimple::create(),
                MoveEndBigWord::create(),
                MoveEndWord::create(),
                MoveFwdBigWord::create(),
                MoveFwdWord::create(),
                MoveFwdWordSimple::create(),
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

                Set::create(),
                Command::create(),
                Delcommand::create(),
                Comclear::create(),
                Source::create(),

                Map::create(),
                Noremap::create(),
                Unmap::create(),
                Mapclear::create(),

                GMap::create(),
                GNoremap::create(),
                GUnmap::create(),
                GMapclear::create(),

                EMap::create(),
                ENoremap::create(),
                EUnmap::create(),
                EMapclear::create(),

                NMap::create(),
                NNoremap::create(),
                NUnmap::create(),
                NMapclear::create(),

                VMap::create(),
                VNoremap::create(),
                VUnmap::create(),
                VMapclear::create(),

                GNMap::create(),
                GNNoremap::create(),
                GNUnmap::create(),
                GNMapclear::create(),

                GVMap::create(),
                GVNoremap::create(),
                GVUnmap::create(),
                GVMapclear::create(),

                ENMap::create(),
                ENNoremap::create(),
                ENUnmap::create(),
                ENMapclear::create(),

                EVMap::create(),
                EVNoremap::create(),
                EVUnmap::create(),
                EVMapclear::create(),

                IMap::create(),
                INoremap::create(),
                IUnmap::create(),
                IMapclear::create(),

                RMap::create(),
                RNoremap::create(),
                RUnmap::create(),
                RMapclear::create(),

                CMap::create(),
                CNoremap::create(),
                CUnmap::create(),
                CMapclear::create(),

                AutoCmdAdd::create(),
                AutoCmdRemove::create(),
            } ;
            return tmp ;
        }

        BindedFunc::SPtr search_func(std::size_t id) {
            for(auto& func : all_global_binded_funcs()) {
                if(func->id() == id) {
                    return func ;
                }
            }
            return nullptr ;
        }

        BindedFunc::SPtr search_func(const std::string& name) {
            return search_func(BindedFunc::name_to_id(util::A2a(name))) ;
        }
    }
}
