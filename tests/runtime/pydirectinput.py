"""
This code is based on
https://github.com/learncodebygaming/pydirectinput


MIT License

Copyright (c) 2020 Ben Johnson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""

import ctypes
import functools
import inspect
import time

SendInput = ctypes.windll.user32.SendInput
MapVirtualKey = ctypes.windll.user32.MapVirtualKeyW

# Constants for failsafe check and pause

FAILSAFE = True
FAILSAFE_POINTS = [(0, 0)]
PAUSE = 0.1  # Tenth-second pause by default.

# Constants for the mouse button names
LEFT = "left"
MIDDLE = "middle"
RIGHT = "right"
PRIMARY = "primary"
SECONDARY = "secondary"

# Mouse Scan Code Mappings
MOUSEEVENTF_MOVE = 0x0001
MOUSEEVENTF_ABSOLUTE = 0x8000
MOUSEEVENTF_LEFTDOWN = 0x0002
MOUSEEVENTF_LEFTUP = 0x0004
MOUSEEVENTF_LEFTCLICK = MOUSEEVENTF_LEFTDOWN + MOUSEEVENTF_LEFTUP
MOUSEEVENTF_RIGHTDOWN = 0x0008
MOUSEEVENTF_RIGHTUP = 0x0010
MOUSEEVENTF_RIGHTCLICK = MOUSEEVENTF_RIGHTDOWN + MOUSEEVENTF_RIGHTUP
MOUSEEVENTF_MIDDLEDOWN = 0x0020
MOUSEEVENTF_MIDDLEUP = 0x0040
MOUSEEVENTF_MIDDLECLICK = MOUSEEVENTF_MIDDLEDOWN + MOUSEEVENTF_MIDDLEUP

# KeyBdInput Flags
KEYEVENTF_EXTENDEDKEY = 0x0001
KEYEVENTF_KEYUP = 0x0002
KEYEVENTF_SCANCODE = 0x0008
KEYEVENTF_UNICODE = 0x0004

# MapVirtualKey Map Types
MAPVK_VK_TO_CHAR = 2
MAPVK_VK_TO_VSC = 0
MAPVK_VSC_TO_VK = 1
MAPVK_VSC_TO_VK_EX = 3

# Keyboard Scan Code Mappings
KEYBOARD_MAPPING = {
    'escape': 0x01,
    'esc': 0x01,
    'f1': 0x3B,
    'f2': 0x3C,
    'f3': 0x3D,
    'f4': 0x3E,
    'f5': 0x3F,
    'f6': 0x40,
    'f7': 0x41,
    'f8': 0x42,
    'f9': 0x43,
    'f10': 0x44,
    'f11': 0x57,
    'f12': 0x58,
    'printscreen': 0xB7,
    'prntscrn': 0xB7,
    'prtsc': 0xB7,
    'prtscr': 0xB7,
    'scrolllock': 0x46,
    'pause': 0xC5,
    '`': 0x29,
    '1': 0x02,
    '2': 0x03,
    '3': 0x04,
    '4': 0x05,
    '5': 0x06,
    '6': 0x07,
    '7': 0x08,
    '8': 0x09,
    '9': 0x0A,
    '0': 0x0B,
    '-': 0x0C,
    '=': 0x0D,
    'backspace': 0x0E,
    'insert': 0xD2 + 1024,
    'home': 0xC7 + 1024,
    'pageup': 0xC9 + 1024,
    'pagedown': 0xD1 + 1024,
    # numpad
    'numlock': 0x45,
    'divide': 0xB5 + 1024,
    'multiply': 0x37,
    'subtract': 0x4A,
    'add': 0x4E,
    'decimal': 0x53,
    'numpadenter': 0x9C + 1024,
    'numpad1': 0x4F,
    'numpad2': 0x50,
    'numpad3': 0x51,
    'numpad4': 0x4B,
    'numpad5': 0x4C,
    'numpad6': 0x4D,
    'numpad7': 0x47,
    'numpad8': 0x48,
    'numpad9': 0x49,
    'numpad0': 0x52,
    # end numpad
    'tab': 0x0F,
    'q': 0x10,
    'w': 0x11,
    'e': 0x12,
    'r': 0x13,
    't': 0x14,
    'y': 0x15,
    'u': 0x16,
    'i': 0x17,
    'o': 0x18,
    'p': 0x19,
    '[': 0x1A,
    ']': 0x1B,
    '\\': 0x2B,
    'del': 0xD3 + 1024,
    'delete': 0xD3 + 1024,
    'end': 0xCF + 1024,
    'capslock': 0x3A,
    'a': 0x1E,
    's': 0x1F,
    'd': 0x20,
    'f': 0x21,
    'g': 0x22,
    'h': 0x23,
    'j': 0x24,
    'k': 0x25,
    'l': 0x26,
    ';': 0x27,
    "'": 0x28,
    'enter': 0x1C,
    'return': 0x1C,
    'shift': 0x2A,
    'shiftleft': 0x2A,
    'z': 0x2C,
    'x': 0x2D,
    'c': 0x2E,
    'v': 0x2F,
    'b': 0x30,
    'n': 0x31,
    'm': 0x32,
    ',': 0x33,
    '.': 0x34,
    '/': 0x35,
    'shiftright': 0x36,
    'ctrl': 0x1D,
    'ctrlleft': 0x1D,
    'win': 0xDB + 1024,
    'winleft': 0xDB + 1024,
    'alt': 0x38,
    'altleft': 0x38,
    ' ': 0x39,
    'space': 0x39,
    'altright': 0xB8 + 1024,
    'winright': 0xDC + 1024,
    'apps': 0xDD + 1024,
    'ctrlright': 0x9D + 1024,
    # arrow key scancodes can be different depending on the hardware,
    # so I think the best solution is to look it up based on the virtual key
    # https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-mapvirtualkeya?redirectedfrom=MSDN
    'up': MapVirtualKey(0x26, MAPVK_VK_TO_VSC),
    'left': MapVirtualKey(0x25, MAPVK_VK_TO_VSC),
    'down': MapVirtualKey(0x28, MAPVK_VK_TO_VSC),
    'right': MapVirtualKey(0x27, MAPVK_VK_TO_VSC),
}

# C struct redefinitions

PUL = ctypes.POINTER(ctypes.c_ulong)


class KeyBdInput(ctypes.Structure):
    _fields_ = [("wVk", ctypes.c_ushort),
                ("wScan", ctypes.c_ushort),
                ("dwFlags", ctypes.c_ulong),
                ("time", ctypes.c_ulong),
                ("dwExtraInfo", PUL)]


class HardwareInput(ctypes.Structure):
    _fields_ = [("uMsg", ctypes.c_ulong),
                ("wParamL", ctypes.c_short),
                ("wParamH", ctypes.c_ushort)]


class MouseInput(ctypes.Structure):
    _fields_ = [("dx", ctypes.c_long),
                ("dy", ctypes.c_long),
                ("mouseData", ctypes.c_ulong),
                ("dwFlags", ctypes.c_ulong),
                ("time", ctypes.c_ulong),
                ("dwExtraInfo", PUL)]


class POINT(ctypes.Structure):
    _fields_ = [("x", ctypes.c_long),
                ("y", ctypes.c_long)]


class Input_I(ctypes.Union):
    _fields_ = [("ki", KeyBdInput),
                ("mi", MouseInput),
                ("hi", HardwareInput)]


class Input(ctypes.Structure):
    _fields_ = [("type", ctypes.c_ulong),
                ("ii", Input_I)]


# Fail Safe and Pause implementation

class FailSafeException(Exception):
    pass


def failSafeCheck():
    if FAILSAFE and tuple(position()) in FAILSAFE_POINTS:
        raise FailSafeException(
            "PyDirectInput fail-safe triggered from mouse moving to a corner of the screen. To disable this " \
            "fail-safe, set pydirectinput.FAILSAFE to False. DISABLING FAIL-SAFE IS NOT RECOMMENDED."
        )


def _handlePause(_pause):
    if _pause:
        assert isinstance(PAUSE, int) or isinstance(PAUSE, float)
        time.sleep(PAUSE)


# direct copy of _genericPyAutoGUIChecks()
def _genericPyDirectInputChecks(wrappedFunction):
    @functools.wraps(wrappedFunction)
    def wrapper(*args, **kwargs):
        funcArgs = inspect.getcallargs(wrappedFunction, *args, **kwargs)

        failSafeCheck()
        returnVal = wrappedFunction(*args, **kwargs)
        _handlePause(funcArgs.get("_pause"))
        return returnVal

    return wrapper


# Helper Functions

def _to_windows_coordinates(x=0, y=0):
    display_width, display_height = size()

    # the +1 here prevents exactly mouse movements from sometimes ending up off by 1 pixel
    windows_x = (x * 65536) // display_width + 1
    windows_y = (y * 65536) // display_height + 1

    return windows_x, windows_y


# position() works exactly the same as PyAutoGUI. I've duplicated it here so that moveRel() can use it to calculate
# relative mouse positions.
def position(x=None, y=None):
    cursor = POINT()
    ctypes.windll.user32.GetCursorPos(ctypes.byref(cursor))
    return (x if x else cursor.x, y if y else cursor.y)


# size() works exactly the same as PyAutoGUI. I've duplicated it here so that _to_windows_coordinates() can use it 
# to calculate the window size.
def size():
    return (ctypes.windll.user32.GetSystemMetrics(0), ctypes.windll.user32.GetSystemMetrics(1))


# Main Mouse Functions

# Ignored parameters: duration, tween, logScreenshot
@_genericPyDirectInputChecks
def mouseDown(x=None, y=None, button=PRIMARY, duration=None, tween=None, logScreenshot=None, _pause=True):
    if not x is None or not y is None:
        moveTo(x, y)

    ev = None
    if button == PRIMARY or button == LEFT:
        ev = MOUSEEVENTF_LEFTDOWN
    elif button == MIDDLE:
        ev = MOUSEEVENTF_MIDDLEDOWN
    elif button == SECONDARY or button == RIGHT:
        ev = MOUSEEVENTF_RIGHTDOWN

    if not ev:
        raise ValueError('button arg to _click() must be one of "left", "middle", or "right", not %s' % button)

    extra = ctypes.c_ulong(0)
    ii_ = Input_I()
    ii_.mi = MouseInput(0, 0, 0, ev, 0, ctypes.pointer(extra))
    x = Input(ctypes.c_ulong(0), ii_)
    SendInput(1, ctypes.pointer(x), ctypes.sizeof(x))


# Ignored parameters: duration, tween, logScreenshot
@_genericPyDirectInputChecks
def mouseUp(x=None, y=None, button=PRIMARY, duration=None, tween=None, logScreenshot=None, _pause=True):
    if not x is None or not y is None:
        moveTo(x, y)

    ev = None
    if button == PRIMARY or button == LEFT:
        ev = MOUSEEVENTF_LEFTUP
    elif button == MIDDLE:
        ev = MOUSEEVENTF_MIDDLEUP
    elif button == SECONDARY or button == RIGHT:
        ev = MOUSEEVENTF_RIGHTUP

    if not ev:
        raise ValueError('button arg to _click() must be one of "left", "middle", or "right", not %s' % button)

    extra = ctypes.c_ulong(0)
    ii_ = Input_I()
    ii_.mi = MouseInput(0, 0, 0, ev, 0, ctypes.pointer(extra))
    x = Input(ctypes.c_ulong(0), ii_)
    SendInput(1, ctypes.pointer(x), ctypes.sizeof(x))


# Ignored parameters: duration, tween, logScreenshot
@_genericPyDirectInputChecks
def click(x=None, y=None, clicks=1, interval=0.0, button=PRIMARY, duration=None, tween=None, logScreenshot=None,
          _pause=True):
    if not x is None or not y is None:
        moveTo(x, y)

    ev = None
    if button == PRIMARY or button == LEFT:
        ev = MOUSEEVENTF_LEFTCLICK
    elif button == MIDDLE:
        ev = MOUSEEVENTF_MIDDLECLICK
    elif button == SECONDARY or button == RIGHT:
        ev = MOUSEEVENTF_RIGHTCLICK

    if not ev:
        raise ValueError('button arg to _click() must be one of "left", "middle", or "right", not %s' % button)

    for i in range(clicks):
        failSafeCheck()

        extra = ctypes.c_ulong(0)
        ii_ = Input_I()
        ii_.mi = MouseInput(0, 0, 0, ev, 0, ctypes.pointer(extra))
        x = Input(ctypes.c_ulong(0), ii_)
        SendInput(1, ctypes.pointer(x), ctypes.sizeof(x))

        time.sleep(interval)


def leftClick(x=None, y=None, interval=0.0, duration=0.0, tween=None, logScreenshot=None, _pause=True):
    click(x, y, 1, interval, LEFT, duration, tween, logScreenshot, _pause)


def rightClick(x=None, y=None, interval=0.0, duration=0.0, tween=None, logScreenshot=None, _pause=True):
    click(x, y, 1, interval, RIGHT, duration, tween, logScreenshot, _pause)


def middleClick(x=None, y=None, interval=0.0, duration=0.0, tween=None, logScreenshot=None, _pause=True):
    click(x, y, 1, interval, MIDDLE, duration, tween, logScreenshot, _pause)


def doubleClick(x=None, y=None, interval=0.0, button=LEFT, duration=0.0, tween=None, logScreenshot=None, _pause=True):
    click(x, y, 2, interval, button, duration, tween, logScreenshot, _pause)


def tripleClick(x=None, y=None, interval=0.0, button=LEFT, duration=0.0, tween=None, logScreenshot=None, _pause=True):
    click(x, y, 3, interval, button, duration, tween, logScreenshot, _pause)


# Missing feature: scroll functions


# Ignored parameters: duration, tween, logScreenshot
# PyAutoGUI uses ctypes.windll.user32.SetCursorPos(x, y) for this, which might still work fine in DirectInput 
# environments.
# Use the relative flag to do a raw win32 api relative movement call (no MOUSEEVENTF_ABSOLUTE flag), which may be more 
# appropriate for some applications. Note that this may produce inexact results depending on mouse movement speed.
@_genericPyDirectInputChecks
def moveTo(x=None, y=None, duration=None, tween=None, logScreenshot=False, _pause=True, relative=False):
    if not relative:
        x, y = position(x, y)  # if only x or y is provided, will keep the current position for the other axis
        x, y = _to_windows_coordinates(x, y)
        extra = ctypes.c_ulong(0)
        ii_ = Input_I()
        ii_.mi = MouseInput(x, y, 0, (MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE), 0, ctypes.pointer(extra))
        command = Input(ctypes.c_ulong(0), ii_)
        SendInput(1, ctypes.pointer(command), ctypes.sizeof(command))
    else:
        currentX, currentY = position()
        moveRel(x - currentX, y - currentY, relative=True)


# Ignored parameters: duration, tween, logScreenshot
# move() and moveRel() are equivalent.
# Use the relative flag to do a raw win32 api relative movement call (no MOUSEEVENTF_ABSOLUTE flag), which may be more 
# appropriate for some applications.
@_genericPyDirectInputChecks
def moveRel(xOffset=None, yOffset=None, duration=None, tween=None, logScreenshot=False, _pause=True, relative=False):
    if not relative:
        x, y = position()
        if xOffset is None:
            xOffset = 0
        if yOffset is None:
            yOffset = 0
        moveTo(x + xOffset, y + yOffset)
    else:
        # When using MOUSEEVENTF_MOVE for relative movement the results may be inconsistent.
        # "Relative mouse motion is subject to the effects of the mouse speed and the two-mouse threshold values. A user
        # sets these three values with the Pointer Speed slider of the Control Panel's Mouse Properties sheet. You can 
        # obtain and set these values using the SystemParametersInfo function." 
        # https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-mouseinput
        # https://stackoverflow.com/questions/50601200/pyhon-directinput-mouse-relative-moving-act-not-as-expected
        extra = ctypes.c_ulong(0)
        ii_ = Input_I()
        ii_.mi = MouseInput(xOffset, yOffset, 0, MOUSEEVENTF_MOVE, 0, ctypes.pointer(extra))
        command = Input(ctypes.c_ulong(0), ii_)
        SendInput(1, ctypes.pointer(command), ctypes.sizeof(command))


move = moveRel


# Missing feature: drag functions


# Keyboard Functions


# Ignored parameters: logScreenshot
# Missing feature: auto shift for special characters (ie. '!', '@', '#'...)
@_genericPyDirectInputChecks
def keyDown(key, logScreenshot=None, _pause=True):
    if not key in KEYBOARD_MAPPING or KEYBOARD_MAPPING[key] is None:
        return

    keybdFlags = KEYEVENTF_SCANCODE

    # Init event tracking
    insertedEvents = 0
    expectedEvents = 1

    # arrow keys need the extended key flag
    if key in ['up', 'left', 'down', 'right']:
        keybdFlags |= KEYEVENTF_EXTENDEDKEY
        # if numlock is on and an arrow key is being pressed, we need to send an additional scancode
        # https://stackoverflow.com/questions/14026496/sendinput-sends-num8-when-i-want-to-send-vk-up-how-come
        # https://handmade.network/wiki/2823-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names
        if ctypes.windll.user32.GetKeyState(0x90):
            # We need to press two keys, so we expect to have inserted 2 events when done
            expectedEvents = 2
            hexKeyCode = 0xE0
            extra = ctypes.c_ulong(0)
            ii_ = Input_I()
            ii_.ki = KeyBdInput(0, hexKeyCode, KEYEVENTF_SCANCODE, 0, ctypes.pointer(extra))
            x = Input(ctypes.c_ulong(1), ii_)

            # SendInput returns the number of event successfully inserted into input stream
            # https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput#return-value
            insertedEvents += SendInput(1, ctypes.pointer(x), ctypes.sizeof(x))

    hexKeyCode = KEYBOARD_MAPPING[key]
    extra = ctypes.c_ulong(0)
    ii_ = Input_I()
    ii_.ki = KeyBdInput(0, hexKeyCode, keybdFlags, 0, ctypes.pointer(extra))
    x = Input(ctypes.c_ulong(1), ii_)
    insertedEvents += SendInput(1, ctypes.pointer(x), ctypes.sizeof(x))

    return insertedEvents == expectedEvents


# Ignored parameters: logScreenshot
# Missing feature: auto shift for special characters (ie. '!', '@', '#'...)
@_genericPyDirectInputChecks
def keyUp(key, logScreenshot=None, _pause=True):
    if not key in KEYBOARD_MAPPING or KEYBOARD_MAPPING[key] is None:
        return

    keybdFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP

    # Init event tracking
    insertedEvents = 0
    expectedEvents = 1

    # arrow keys need the extended key flag
    if key in ['up', 'left', 'down', 'right']:
        keybdFlags |= KEYEVENTF_EXTENDEDKEY

    hexKeyCode = KEYBOARD_MAPPING[key]
    extra = ctypes.c_ulong(0)
    ii_ = Input_I()
    ii_.ki = KeyBdInput(0, hexKeyCode, keybdFlags, 0, ctypes.pointer(extra))
    x = Input(ctypes.c_ulong(1), ii_)

    # SendInput returns the number of event successfully inserted into input stream
    # https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput#return-value
    insertedEvents += SendInput(1, ctypes.pointer(x), ctypes.sizeof(x))

    # if numlock is on and an arrow key is being pressed, we need to send an additional scancode
    # https://stackoverflow.com/questions/14026496/sendinput-sends-num8-when-i-want-to-send-vk-up-how-come
    # https://handmade.network/wiki/2823-keyboard_inputs_-_scancodes,_raw_input,_text_input,_key_names
    if key in ['up', 'left', 'down', 'right'] and ctypes.windll.user32.GetKeyState(0x90):
        # We need to press two keys, so we expect to have inserted 2 events when done
        expectedEvents = 2
        hexKeyCode = 0xE0
        extra = ctypes.c_ulong(0)
        ii_ = Input_I()
        ii_.ki = KeyBdInput(0, hexKeyCode, KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP, 0, ctypes.pointer(extra))
        x = Input(ctypes.c_ulong(1), ii_)
        insertedEvents += SendInput(1, ctypes.pointer(x), ctypes.sizeof(x))

    return insertedEvents == expectedEvents


# Ignored parameters: logScreenshot
# nearly identical to PyAutoGUI's implementation
@_genericPyDirectInputChecks
def press(keys, presses=1, interval=0.0, logScreenshot=None, _pause=True):
    if type(keys) == str:
        if len(keys) > 1:
            keys = keys.lower()
        keys = [keys]  # If keys is 'enter', convert it to ['enter'].
    else:
        lowerKeys = []
        for s in keys:
            if len(s) > 1:
                lowerKeys.append(s.lower())
            else:
                lowerKeys.append(s)
        keys = lowerKeys
    interval = float(interval)

    # We need to press x keys y times, which comes out to x*y presses in total
    expectedPresses = presses * len(keys)
    completedPresses = 0

    for i in range(presses):
        for k in keys:
            failSafeCheck()
            downed = keyDown(k)
            upped = keyUp(k)
            # Count key press as complete if key was "downed" and "upped" successfully
            if downed and upped:
                completedPresses += 1

        time.sleep(interval)

    return completedPresses == expectedPresses


# Ignored parameters: logScreenshot
# nearly identical to PyAutoGUI's implementation
@_genericPyDirectInputChecks
def typewrite(message, interval=0.0, logScreenshot=None, _pause=True):
    interval = float(interval)
    for c in message:
        if len(c) > 1:
            c = c.lower()
        press(c, _pause=False)
        time.sleep(interval)
        failSafeCheck()


write = typewrite

# Missing feature: hotkey functions
