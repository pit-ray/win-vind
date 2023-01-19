import ctypes
import pydirectinput as pdi


def get_cursor_pos():
    return pdi.position()


def set_cursor_pos(x, y):
    pdi.moveTo(x, y)


def click(x=None, y=None):
    pdi.leftClick(x, y)


def get_maximum_window_size():
    GetSystemMetrics = ctypes.windll.user32.GetSystemMetrics
    SM_CXFULLSCREEN = 16
    SM_CYFULLSCREEN = 17
    w = GetSystemMetrics(SM_CXFULLSCREEN)
    h = GetSystemMetrics(SM_CYFULLSCREEN)
    return (w, h)


if __name__ == '__main__':
    print(get_maximum_window_size())
