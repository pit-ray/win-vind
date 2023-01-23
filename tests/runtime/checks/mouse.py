from win import get_cursor_pos, set_cursor_pos


def get_cases():
    return [
        jump_cursor_to_active_window,
        easy_click_left,

        move_cursor_left,
        move_cursor_right,
        move_cursor_up,
        move_cursor_down,
    ]


def move_cursor_left(handler, mock_app):
    x = get_cursor_pos()[0]
    handler.send_command('h')
    delta = x - get_cursor_pos()[0]

    return delta > 0


def move_cursor_right(handler, mock_app):
    x = get_cursor_pos()[0]
    handler.send_command('l')
    delta = get_cursor_pos()[0] - x
    return delta > 0


def move_cursor_up(handler, mock_app):
    y = get_cursor_pos()[1]
    handler.send_command('k')
    delta = y - get_cursor_pos()[1]

    return delta > 0


def move_cursor_down(handler, mock_app):
    y = get_cursor_pos()[1]
    handler.send_command('j')
    delta = get_cursor_pos()[1] - y

    return delta > 0


def easy_click_left(handler, mock_app):
    mock_app.frame.ready_button()
    mock_app.focus()

    handler.send_command('FFG')
    result = mock_app.frame.check_if_clicked()

    mock_app.frame.ready_button()
    return result


def jump_cursor_to_active_window(handler, mock_app):
    mock_app.focus()

    cx, cy = mock_app.get_window_center()
    width, height = mock_app.get_window_size()
    set_cursor_pos(cx + width + 20, cy + height + 20)

    handler.send_command('t')
    x, y = get_cursor_pos()

    return x == cx and y == cy
