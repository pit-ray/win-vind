import pydirectinput as pdi


def move_cursor_left(handler):
    handler.send_command('<to_insert><ctrl-]>')

    x = pdi.position()[0]
    handler.send_command('h' * 100)
    delta = abs(pdi.position()[0] - x)

    handler.send_command('<to_insert>')
    return delta > 0
