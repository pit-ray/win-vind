It has differences from the latest release.
`release` is the so-called stable version.

## Bugs
- fixed the problem not to call `Switch Virtual Desktop to Right` and `Switch Virtual Desktop to Left`.
- Revised some small bugs.

## Change Logs
- Renamed a menu text.
- Remodeled `Preferences/Bind Lists` for customizing key-bindings to new UI `Preferences/Bindings`.
- Renovated `Preferences/Settings`.
- Removed `Preferences/Options`.
- Supported dynamically changing of settings.
- Changed settings' format from XML to JSON.
- Refactored all codes.
- Renovated Virtual Command Line. Its input system is better and it displays mode when changing mode. There are settings of the latter's fade-out time in `Preferences/Settings/Virtual Command Line/Fade Out Time (s)`.

## Useful Functions
- Added `Dedicating Mode` for the real-mouse user. If you push a left button of the mouse while pressing system keys(Shift, Alt, Ctrl), `win-vind` is only validated on a selected window. You can turn this function on `Preferences/Settings/Options/Dedicate to One Window` checkbox.
- Added `Initial Mode` setting into `Preferences/Settings/Common`. You can select one between `GUI Normal`, `GUI Insert`, `Editor Normal`, and `Editor Insert`.

## New Functions Based Original Vim
- Insert BOL (I/gI)
- Append EOL (A)
- Begin New Line Above Caret (O)