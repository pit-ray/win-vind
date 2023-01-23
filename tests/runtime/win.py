import ctypes
import pydirectinput as pdi


class RECT(ctypes.Structure):
    _fields_ = [
        ("left", ctypes.c_long),
        ("top", ctypes.c_long),
        ("right", ctypes.c_long),
        ("bottom", ctypes.c_long)
    ]


def get_cursor_pos():
    return pdi.position()


def set_cursor_pos(x, y):
    pdi.moveTo(x, y)


def click(x=None, y=None):
    pdi.leftClick(x, y)


def get_maximum_window_size():
    SystemParametersInfo = ctypes.windll.user32.SystemParametersInfoA
    SPI_GETWORKAREA = 0x0030

    rect = RECT()
    res = SystemParametersInfo(SPI_GETWORKAREA, 0, rect, 0)

    width = rect.right - rect.left
    height = rect.bottom - rect.top
    return (width, height)


if __name__ == '__main__':
    print(get_maximum_window_size())
