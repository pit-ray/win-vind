import time

import pydirectinput as pdi

import logger


def move_cursor_left():
    x = pdi.position()[0]

    pdi.press(['ctrl', ']'])

    start = time.time()
    while time.time() - start < 2:
        pdi.keyDown('h')

    pdi.keyUp('h')
    pdi.press('i')

    delta = pdi.position()[0] - x
    if delta == 0:
        logger.error('move_cursor_left is failed.')
