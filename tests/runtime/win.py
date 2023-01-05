import pydirectinput as pdi


def get_cursor_pos():
    return pdi.position()


def set_cursor_pos(x, y):
    pdi.moveTo(x, y)


def click(x=None, y=None):
    pdi.leftClick(x, y)
