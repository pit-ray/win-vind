It has differences from the latest release.
`release` is the so-called stable version.

## Fix
- fixed the problem that line's copying functions (e.g. dd, yy) are not working on Microsoft Office Word.
- fixed the problem repeating key-stroke infinity.
- fixed not working after BackSpace in Virtual Command Line

## Change
- The Command Mode was divided as KeyBindings from the inner matching system.
- This version requires C++17
- Improved Error out function.
- Supported appropriate exception-based error system.
- In bindings.json, changed command's syntax from unique one to original-Vim.
- Some classes modified to STL-based simple one.
- Supported file-properties of the installer
- Changed methods of the key eventer to faster one.

## New
- Change Lines And Start Insert (`c->c`, `S`)
- Change Characters And Start Insert (`s`)
- Change Text Until EOL And Start Insert (`C`)
- Delete Text by Motion (`d{Motion}`)
- Change Text by Motion (`c{Motion}`)


## Bugs
- Nothing
