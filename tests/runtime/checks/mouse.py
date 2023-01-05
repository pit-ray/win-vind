from win import get_cursor_pos


def get_cases():
    return [
        move_cursor_left,
        move_cursor_right,
        move_cursor_up,
        move_cursor_down
    ]


def move_cursor_left(handler, mock_app):
    handler.send_command('<to_insert><ctrl-]>')

    x = get_cursor_pos()[0]
    handler.send_command('h' * 100)
    delta = x - get_cursor_pos()[0]

    handler.send_command('<to_insert>')
    return delta > 0


def move_cursor_right(handler, mock_app):
    handler.send_command('<to_insert><ctrl-]>')

    x = get_cursor_pos()[0]
    handler.send_command('l' * 100)
    delta = get_cursor_pos()[0] - x

    handler.send_command('<to_insert>')
    return delta > 0


def move_cursor_up(handler, mock_app):
    handler.send_command('<to_insert><ctrl-]>')

    y = get_cursor_pos()[1]
    handler.send_command('k' * 100)
    delta = y - get_cursor_pos()[1]

    handler.send_command('<to_insert>')
    return delta > 0


def move_cursor_down(handler, mock_app):
    handler.send_command('<to_insert><ctrl-]>')

    y = get_cursor_pos()[1]
    handler.send_command('j' * 100)
    delta = get_cursor_pos()[1] - y

    handler.send_command('<to_insert>')
    return delta > 0
