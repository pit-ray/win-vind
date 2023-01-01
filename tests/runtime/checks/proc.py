import pydirectinput as pdi


def exit_process():
    pdi.press(['ctrl', ']'])
    pdi.typewrite([':', 'e', 'x', 'i', 't', 'enter'])
