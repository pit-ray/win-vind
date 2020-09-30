It has differences from the latest release.
`release` is the so-called stable version.

## Fix
- fixed the problem that line's copying functions (e.g. dd, yy) are not working on Microsoft Office Word.

## Change
- The Command Mode was divided as KeyBindings from the inner matching system.
- This version requires C++17
- Improved Error out function.

## New
- Delete Lines And Start Insert (`c->c`, `S`)
- Delete Characters And Start Insert (`s`)
- Delete Text Until EOL And Start Insert (`C`)


## Bugs
- In Editor Mode, infinite typing when repeating keystrokes in some cases.
