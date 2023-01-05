from win import get_cursor_pos, set_cursor_pos, click


def get_cases():
    return [
        jump_cursor_to_active_window,

        move_cursor_left,
        move_cursor_right,
        move_cursor_up,
        move_cursor_down,
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


def jump_cursor_to_active_window(handler, mock_app):
    left, right = mock_app.get_window_pos()
    click(left + 10, right + 10)

    cx, cy = mock_app.get_window_center()

    width, height = mock_app.get_window_size()
    set_cursor_pos(left + width + 20, right + height + 20)

    handler.send_command('<to_gui_normal>t')
    x, y = get_cursor_pos()

    return x == cx and y == cy
