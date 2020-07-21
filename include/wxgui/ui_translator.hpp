#ifndef _UI_TRANSLATOR_HPP
#define _UI_TRANSLATOR_HPP

#include <string>
#include <wx/string.h>

namespace UITrans
{
    enum class Label : unsigned char {
        SysTray_Preferences, //System Tray
        SysTray_About,
        SysTray_Exit,

        Pref_Settings, //Settings Tab
        Pref_Settings_Common,

        Pref_Settings_Common_GUILang,
        Pref_Settings_Common_GUILang_US,
        Pref_Settings_Common_GUILang_JP,

        Pref_Settings_Common_IconStyle,
        Pref_Settings_Common_IconStyle_Dark,
        Pref_Settings_Common_IconStyle_Light,

        Pref_Settings_Common_KBType,
        Pref_Settings_Common_KBType_US,
        Pref_Settings_Common_KBType_JP,
        Pref_Settings_Common_KBType_Custom,

        Pref_Settings_Common_Resolution,

        Pref_Settings_Hotkeys,
        Pref_Settings_Commands,

        Pref_BindList, //Bind List Tab
        Pref_BindList_Hotkeys,
        Pref_BindList_Commands,

        Pref_ShortcutApps, //Shortcut Apps Tab
        Pref_ShortcutApps_Shell,
        Pref_ShortcutApps_Name,
        Pref_ShortcutApps_Path,
        Pref_ShortcutApps_Add,
        Pref_ShortcutApps_Del,

        Pref_Options, //Options Tab

        Pref_Options_Opts,
        Pref_Options_DispCmds,

        Pref_Options_DispCmds_Pos_UpperLeft,
        Pref_Options_DispCmds_Pos_UpperMid,
        Pref_Options_DispCmds_Pos_UpperRight,
        Pref_Options_DispCmds_Pos_MidLeft,
        Pref_Options_DispCmds_Pos_Center,
        Pref_Options_DispCmds_Pos_MidRight,
        Pref_Options_DispCmds_Pos_LowerLeft,
        Pref_Options_DispCmds_Pos_LowerMid,
        Pref_Options_DispCmds_Pos_LowerRight,

        Pref_Return_To_Default, //General Buttons
        Pref_OK,
        Pref_Cancel,
        Pref_Apply
    } ;

    const wxString& trans(const Label index) ;
    const wxString& trans(const std::string id) ;
}

#endif